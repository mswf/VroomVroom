#include "importer_imp.h"
#include <fstream>

#include "importer.hpp"
#include "postprocess.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../IO/stb_image.h"

#include "../DataStructure/material.h"

ImporterImp::ImporterImp() :
	importFlags( aiProcessPreset_TargetRealtime_Quality ),
	import_scene_failure_reason( "No errors logged." ),
	import_image_failure_reason( "No errors logged." )
{}

ImporterImp::~ImporterImp()
{}

uint8* ImporterImp::ImportImage( const char* filename, uint32& width, uint32& height, uint32 requiring_components, IMPORTER_MESSAGE& err_msg, bool vertical_flip )
{
	stbi_set_flip_vertically_on_load(vertical_flip);
	int32 comp, w, h;
	int32 rc = STBI_rgb_alpha;

	uint8* image = stbi_load( filename, &w, &h, &comp, rc);

	width = w;
	height = h;

	if ( image == nullptr )
	{
		import_image_failure_reason = std::string(stbi__g_failure_reason);
		err_msg = IMPORTER_MESSAGE::IMAGE_FAILED_TO_LOAD;
		return NULL;
	}

	size_t size = w * h * requiring_components;
	uint8* data = new uint8[size];
	memcpy(data, image, size);
	stbi_image_free(image);

	err_msg = IMPORTER_MESSAGE::IMAGE_LOAD_OK;
	return data;
}

aiScene* ImporterImp::ImportObjFile( const string& pFile, IMPORTER_MESSAGE& msg )
{
	Assimp::Importer importer;
	std::ifstream fin( pFile.c_str() );
	//check if file exists
	if ( !fin.fail() )
	{
		fin.close();
	}
	else
	{
		msg = IMPORTER_MESSAGE::FILE_NOT_FOUND;
		return NULL;
	}

	const aiScene* sc = importer.ReadFile( pFile, importFlags );

	if ( !sc )
	{
		// If the import failed, save error for request
		import_scene_failure_reason = importer.GetErrorString();
		msg = IMPORTER_MESSAGE::FILE_CORRUPT;
		return NULL;
	}

	msg = IMPORTER_MESSAGE::FILE_OK;
	return importer.GetOrphanedScene();
}

void ImporterImp::FreeScene( aiScene* sc )
{
	if ( sc != NULL )
	{
		delete sc;
		sc = NULL;
	}
}

void ImporterImp::ExtractMesh( const aiMesh* mesh, Mesh* m )
{
	// buffer for indices
	//m = new Mesh();
	uint32 i;
	m->numIndices = mesh->mNumFaces * 3;
	for ( i = 0; i < mesh->mNumFaces; ++i )
	{
		const aiFace* face = &mesh->mFaces[i];
		m->indices.push_back( face->mIndices[0] );
		m->indices.push_back( face->mIndices[1] );
		m->indices.push_back( face->mIndices[2] );
	}

	// buffer for vertex positions
	if ( mesh->HasPositions() )
	{
		m->hasPositions = true;
		for ( i = 0; i < mesh->mNumVertices; ++i )
		{
			aiVector3D v = mesh->mVertices[i];
			glm::vec3 vertex( v.x, v.y, v.z );
			m->vertices.push_back( vertex );
		}
	}

	// buffer for vertex normals
	if ( mesh->HasNormals() )
	{
		m->hasNormals = true;
		for ( i = 0; i < mesh->mNumVertices; ++i )
		{
			aiVector3D n = mesh->mNormals[i];
			glm::vec3 normal( n.x, n.y, n.z );
			m->normals.push_back( normal );
		}
	}

	// buffer for bitangents & tangents
	if ( mesh->HasTangentsAndBitangents() )
	{
		m->hasTangentsAndBitangets = true;
		for ( i = 0; i < mesh->mNumVertices; ++i )
		{
			aiVector3D bi = mesh->mBitangents[i];
			glm::vec3 bitangent( bi.x, bi.y, bi.z );
			m->bitangents.push_back( bitangent );

			aiVector3D ta = mesh->mTangents[i];
			glm::vec3 tangent( ta.x, ta.y, ta.z );
			m->tangents.push_back( tangent );
		}
	}

	// buffer for vertex texture coordinates
	if ( mesh->HasTextureCoords(0) )
	{
		m->hasUVs = true;
		for ( i = 0; i < mesh->mNumVertices; ++i )
		{
			float x = mesh->mTextureCoords[0][i].x;
			float y = mesh->mTextureCoords[0][i].y;
			glm::vec2 uv( x, y );
			m->uvs.push_back( uv );
		}
	}

}

void ImporterImp::ExtractMaterial( const aiMaterial* mtl, Material* material, std::vector< string >* textureIdMap )
{
	//printf( "Number of properties: %i, expected 13. \n", material->mNumProperties );
	uint32 max = 1;
	int32 wireframe = 0;
	int32 two_sided = 0;
	aiString name( "No name" );
	aiColor4D diffuse, ambient, specular, emissive, transparent, reflective;
	float opacity = 0.0;
	float bumpScaling = 0.0;
	float shininess = 0.0;
	float shininess_strenght = 0.0;
	float color[4];



	// A COUPLE OF HELPER CLOSURES/LAMBDAS
	auto SetFloat4 = []( float * c, float r, float g, float b, float a )
	{
		c[0] = r;
		c[1] = g;
		c[2] = b;
		c[3] = a;
	};

	auto aiColorToFloat4 = []( aiColor4D * aiColor, float * color )
	{
		color[0] = aiColor->r;
		color[1] = aiColor->g;
		color[2] = aiColor->b;
		color[3] = aiColor->a;
	};

	/*
	auto printFloat4 = []( float* color, const char* name )
	{
		printf("Material %s color: %f , %f , %f , %f \n", name, color[0], color[1], color[2], color[3] );
	};
	*/

	// END OF HELPERS


	if ( AI_SUCCESS == aiGetMaterialString( mtl, AI_MATKEY_NAME, &name) )
	{
		material->name = name.C_Str();
	}

	aiGetMaterialIntegerArray( mtl, AI_MATKEY_ENABLE_WIREFRAME, &wireframe, &max );
	material->wireframe_enabled = wireframe;

	if ( AI_SUCCESS == aiGetMaterialIntegerArray( mtl, AI_MATKEY_TWOSIDED, &two_sided, &max) )
	{
		material->two_sided = two_sided;
	}


	aiGetMaterialFloatArray( mtl, AI_MATKEY_OPACITY, &opacity, &max);
	material->opacity = opacity;

	aiGetMaterialFloatArray( mtl, AI_MATKEY_BUMPSCALING, &bumpScaling, &max);
	material->bump_scaling = bumpScaling;

	aiGetMaterialFloatArray( mtl, AI_MATKEY_SHININESS, &shininess, &max);
	material->shininess = shininess;

	aiGetMaterialFloatArray( mtl, AI_MATKEY_SHININESS_STRENGTH, &shininess_strenght, &max);
	material->shininess_strenght = shininess_strenght;

	SetFloat4(color, 0.8f, 0.8f, 0.8f, 1.0f);
	if ( AI_SUCCESS == aiGetMaterialColor( mtl, AI_MATKEY_COLOR_DIFFUSE, &diffuse ) )
	{
		aiColorToFloat4(&diffuse, color);
	}
	memcpy(material->diffuse, color, sizeof(color));

	SetFloat4(color, 0.2f, 0.2f, 0.2f, 1.0f);
	if ( AI_SUCCESS == aiGetMaterialColor( mtl, AI_MATKEY_COLOR_AMBIENT, &ambient ) )
	{
		aiColorToFloat4(&ambient, color);
	}
	memcpy(material->ambient, color, sizeof(color));

	SetFloat4(color, 0.0f, 0.0f, 0.0f, 1.0f);
	if ( AI_SUCCESS == aiGetMaterialColor( mtl, AI_MATKEY_COLOR_SPECULAR, &specular ) )
	{
		aiColorToFloat4(&specular, color);
	}
	memcpy(material->specular, color, sizeof(color));

	SetFloat4(color, 0.0f, 0.0f, 0.0f, 1.0f);
	if ( AI_SUCCESS == aiGetMaterialColor( mtl, AI_MATKEY_COLOR_EMISSIVE, &emissive ) )
	{
		aiColorToFloat4(&emissive, color);
	}
	memcpy(material->emissive, color, sizeof(color));

	SetFloat4(color, 0.0f, 0.0f, 0.0f, 1.0f);
	if ( AI_SUCCESS == aiGetMaterialColor( mtl, AI_MATKEY_COLOR_TRANSPARENT, &transparent ) )
	{
		aiColorToFloat4(&transparent, color);
	}
	memcpy(material->transparent, color, sizeof(color));

	SetFloat4(color, 0.0f, 0.0f, 0.0f, 1.0f);
	if ( AI_SUCCESS == aiGetMaterialColor( mtl, AI_MATKEY_COLOR_REFLECTIVE, &reflective ) )
	{
		aiColorToFloat4(&reflective, color);
	}
	memcpy(material->reflective, color, sizeof(color));

	auto FindTextures = []( const aiMaterial * mtl, aiTextureType type, uint32& textureCount, std::vector< string >* textureIdMap )
	{
		uint32 i;
		aiString path;
		for ( i = 0; i < mtl->GetTextureCount(type); ++i )
		{
			if ( AI_SUCCESS == mtl->GetTexture( type, i, &path ) )
			{
				textureIdMap->push_back( std::string( &path.data[0] ) );
				++textureCount;
			}
		}
	};

	FindTextures( mtl, aiTextureType_DIFFUSE, material->textureCount, textureIdMap );
	FindTextures( mtl, aiTextureType_SPECULAR, material->textureCount, textureIdMap );
	FindTextures( mtl, aiTextureType_HEIGHT, material->textureCount, textureIdMap );
}
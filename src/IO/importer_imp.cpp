#include "importer_imp.h"
#include <fstream>

#include "importer.hpp"
#include "postprocess.h"

#ifdef WORKING
	#define STB_IMAGE_IMPLEMENTATION
	#include "../IO/stb_image.h"
#endif

#include "../glm/vec2.hpp"
#include "../glm/vec3.hpp"

ImporterImp::ImporterImp():
	importFlags(aiProcessPreset_TargetRealtime_Quality),
	import_scene_failure_reason("No errors logged."),
	import_image_failure_reason("No errors logged.")
{}

ImporterImp::~ImporterImp()
{}

int ImporterImp::ImportImage( const char* filename, unsigned char* image, int& width, int& height )
{
#ifdef WORKING
	//stbi_set_flip_vertically_on_load(true);
	int comp;
	//int req_comp = STBI_rgb_alpha;
	
	FreeImage(image);
	//image = stbi_load( filename, &width, &height, &comp, req_comp);
	
	if( image == nullptr )
	{
		//import_image_failure_reason = std::string(stbi__g_failure_reason);
		return (int)IMPORTER_MESSAGE::TEXTURE_FAILED_TO_LOAD;
	}
#endif
	return (int)IMPORTER_MESSAGE::TEXTURE_LOAD_OK;
}

void ImporterImp::FreeImage( unsigned char* img )
{
	if (img != NULL)
	{
#ifdef WORKING
		//stbi_image_free(img);
#endif
		img = NULL;
	}
}

int ImporterImp::ImportObjFile( const std::string& pFile, aiScene* scene )
{
	Assimp::Importer importer;
	std::ifstream fin( pFile.c_str() );
	//check if file exists
	if( !fin.fail() )
	{
		fin.close();
	}
	else
	{
		return (int)IMPORTER_MESSAGE::FILE_NOT_FOUND;
	}
	
	const aiScene* sc = importer.ReadFile( pFile, importFlags );
 
	if( !sc )
	{
		// If the import failed, save error for request
		import_scene_failure_reason = importer.GetErrorString();
		return (int)IMPORTER_MESSAGE::FILE_CORRUPT;
	}
 
	FreeScene( scene );
	scene = importer.GetOrphanedScene();
	return (int)IMPORTER_MESSAGE::FILE_OK;
}

void ImporterImp::FreeScene( aiScene* sc )
{
	if ( sc != NULL )
	{
		delete sc;
		sc = NULL;
	}
}

//TODO(Valentinas): Use C-Arrays instead of GLM library vectors for portability
void ImporterImp::ExtractMesh( const aiMesh* mesh )
{
	
	std::vector< glm::vec3 > vertices;
	std::vector< glm::vec3 > normals;
	std::vector< glm::vec3 > bitangents;
	std::vector< glm::vec3 > tangents;
	std::vector< glm::vec2 > uvs;
	std::vector< unsigned char > indices;
	unsigned int materialIndex = 0;
	bool hasPositions = false;
 	bool hasNormals = false;
	bool hasTangentsAndBitangets = false;
	bool hasUVs = false;
	
	materialIndex = mesh->mMaterialIndex;
	
	// buffer for indices
	int numIndices = mesh->mNumFaces * 3;
	indices.resize(numIndices);
	for (unsigned int t = 0; t < mesh->mNumFaces; ++t)
	{
		const aiFace* face = &mesh->mFaces[t];
		indices.push_back( face->mIndices[0] );
		indices.push_back( face->mIndices[1] );
		indices.push_back( face->mIndices[2] );
	}
	
	// buffer for vertex positions
	if ( mesh->HasPositions() )
	{
		hasPositions = true;
		vertices.resize(mesh->mNumVertices);
		for ( int i = 0; i < mesh->mNumVertices; ++i )
		{
			aiVector3D v = mesh->mVertices[i];
			glm::vec3 vertex( v.x, v.y, v.z );
			vertices.push_back( vertex );
		}
	}
	
	// buffer for vertex normals
	if ( mesh->HasNormals() )
	{
		hasNormals = true;
		normals.resize(mesh->mNumVertices);
		for ( int i = 0; i < mesh->mNumVertices; ++i )
		{
			aiVector3D n = mesh->mNormals[i];
			glm::vec3 normal( n.x, n.y, n.z );
			normals.push_back( normal );
		}
	}
	
	// buffer for bitangents & tangents
	if ( mesh->HasTangentsAndBitangents() )
	{
		hasTangentsAndBitangets = true;
		bitangents.resize(mesh->mNumVertices);
		tangents.resize(mesh->mNumVertices);
		for (int i = 0; i < mesh->mNumVertices; ++i)
		{
			aiVector3D bi = mesh->mBitangents[i];
			glm::vec3 bitangent( bi.x, bi.y, bi.z );
			bitangents.push_back( bitangent );
			
			aiVector3D ta = mesh->mTangents[i];
			glm::vec3 tangent( ta.x, ta.y, ta.z );
			tangents.push_back( tangent );
		}
	}
	
	// buffer for vertex texture coordinates
	if ( mesh->HasTextureCoords(0) )
	{
		hasUVs = true;
		uvs.resize( mesh->mNumVertices );
		for ( unsigned int i = 0; i < mesh->mNumVertices; ++i )
		{
			float x = mesh->mTextureCoords[0][i].x;
			float y = mesh->mTextureCoords[0][i].y;
			glm::vec2 uv( x, y );
			uvs.push_back( uv );
		}
	}

}

void ImporterImp::ExtractMaterial( const aiMaterial* material, MaterialProperties* prop )
{
	unsigned int max = 1;
	int wireframe = 0;
	int two_sided = 0;
	aiString name( "Undefined" );
	aiColor4D diffuse, ambient, specular, emissive, transparent, reflective;
	float opacity = 0.0;
	float bumpScaling = 0.0;
	float shininess = 0.0;
	float shininess_strenght = 0.0;
	float color[4];
	
	//printf( "Number of properties: %i, expected 13. \n", material->mNumProperties );
	
	// A COUPLE OF HELPER CLOSURES/LAMBDAS
	auto SetFloat4 = []( float* c, float r, float g, float b, float a )
	{
		c[0] = r; c[1] = g; c[2] = b; c[3] = a;
	};
	
	auto aiColorToFloat4 = []( aiColor4D* aiColor, float* color )
	{
		color[0] = aiColor->r; color[1] = aiColor->g; color[2] = aiColor->b; color[3] = aiColor->a;
	};
	
	/*
	auto printFloat4 = []( float* color, const char* name )
	{
		printf("Material %s color: %f , %f , %f , %f \n", name, color[0], color[1], color[2], color[3] );
	};
	*/
	// END OF HELPERS
	
	if( AI_SUCCESS == aiGetMaterialString(material, AI_MATKEY_NAME, &name) )
	{
		prop->name = name.C_Str();
		//printf("Material name: %s \n", name.C_Str() );
	}

	if( AI_SUCCESS == aiGetMaterialIntegerArray( material, AI_MATKEY_ENABLE_WIREFRAME, &wireframe, &max ) )
	{
		//fill_mode = wireframe ? GL_LINE : GL_FILL;
		prop->wireframe_enabled = wireframe;
		//printf("Material wireframe_mode: %i \n", wireframe );
	}
	
	if( ( AI_SUCCESS == aiGetMaterialIntegerArray( material, AI_MATKEY_TWOSIDED, &two_sided, &max) ) && two_sided )
	{
		// two_sided ? glEnable(CULL_FACE) : glDisable(CULL_FACE)
		prop->two_sided = two_sided;
		//printf("Material is two_sided?: %i \n", two_sided );
	}

	
	aiGetMaterialFloatArray(material, AI_MATKEY_OPACITY, &opacity, &max);
	prop->opacity = opacity;
	//printf("Material opacity: %f \n", opacity );

	
	aiGetMaterialFloatArray(material, AI_MATKEY_BUMPSCALING, &bumpScaling, &max);
	prop->bump_scaling = bumpScaling;
	//printf("Material bumpScaling: %f \n", bumpScaling );

	
	aiGetMaterialFloatArray(material, AI_MATKEY_SHININESS, &shininess, &max);
	prop->shininess = shininess;
	//printf("Material shininess: %f \n", shininess );

	
	aiGetMaterialFloatArray(material, AI_MATKEY_SHININESS_STRENGTH, &shininess_strenght, &max);
	prop->shininess_strenght = shininess_strenght;
	//printf("Material shininess_strenght: %f \n", shininess_strenght );

	
	SetFloat4(color, 0.8f, 0.8f, 0.8f, 1.0f);
	if( AI_SUCCESS == aiGetMaterialColor( material, AI_MATKEY_COLOR_DIFFUSE, &diffuse ) )
	{
		aiColorToFloat4(&diffuse, color);
	}
	memcpy(prop->diffuse, color, sizeof(color));
	//printFloat4( prop->diffuse, "diffuse" );
	
	SetFloat4(color, 0.2f, 0.2f, 0.2f, 1.0f);
	if( AI_SUCCESS == aiGetMaterialColor( material, AI_MATKEY_COLOR_AMBIENT, &ambient ) )
	{
		aiColorToFloat4(&ambient, color);
	}
	memcpy(prop->ambient, color, sizeof(color));
	//printFloat4( prop->ambient, "ambient" );
	
	SetFloat4(color, 0.0f, 0.0f, 0.0f, 1.0f);
	if( AI_SUCCESS == aiGetMaterialColor( material, AI_MATKEY_COLOR_SPECULAR, &specular ) )
	{
		aiColorToFloat4(&specular, color);
	}
	memcpy(prop->specular, color, sizeof(color));
	//printFloat4( prop->specular, "specular" );
	
	SetFloat4(color, 0.0f, 0.0f, 0.0f, 1.0f);
	if( AI_SUCCESS == aiGetMaterialColor( material, AI_MATKEY_COLOR_EMISSIVE, &emissive ) )
	{
		aiColorToFloat4(&emissive, color);
	}
	memcpy(prop->emissive, color, sizeof(color));
	//printFloat4( prop->emissive, "emissive" );

	SetFloat4(color, 0.0f, 0.0f, 0.0f, 1.0f);
	if( AI_SUCCESS == aiGetMaterialColor( material, AI_MATKEY_COLOR_TRANSPARENT, &transparent ) )
	{
		aiColorToFloat4(&transparent, color);
	}
	memcpy(prop->transparent, color, sizeof(color));
	//printFloat4( prop->transparent, "transparent" );
	
	SetFloat4(color, 0.0f, 0.0f, 0.0f, 1.0f);
	if( AI_SUCCESS == aiGetMaterialColor( material, AI_MATKEY_COLOR_REFLECTIVE, &reflective ) )
	{
		aiColorToFloat4(&reflective, color);
	}
	memcpy(prop->reflective, color, sizeof(color));
	//printFloat4( prop->reflective, "reflective" );
	
	
	//FINDING TEXTURES
	
	std::vector<unsigned int> textureIdMap;
	
	auto FindTextures1 = []( const aiMaterial* mtl, aiTextureType t, std::vector<unsigned int>& textureIdMap )
	{
		printf("FindTexture 1: \n");
		int i;
		for ( i = 0; i < mtl->GetTextureCount(t); ++i )
		{
			aiString path;
			if ( AI_SUCCESS == mtl->GetTexture( t, i, &path ) )
			{
				printf("Texture: %s \n", path.C_Str() );
				//TEXTURE INDEX unsigned int texId = textureIdMap[texPath.data];
				//TEXTURE INDEX aMesh.texIndex = texId;
				//TEXTURE COUNT aMat.texCount = 1;
			}
		}
	};
	
	auto FindTextures2 = []( const aiMaterial* mtl, aiTextureType t, std::vector<unsigned int>& textureIdMap )
	{
		printf("FindTexture 2: \n");
		int index = 0;
		aiString path;
		aiReturn texFound = mtl->GetTexture(t, index, &path);
		while (texFound == AI_SUCCESS)
		{
			printf("Texture: %s \n", path.C_Str() );
			//fill map with textures, OpenGL image ids set to 0
			//textureIdMap[path.data] = 0;
			++index;
			texFound = mtl->GetTexture(aiTextureType_DIFFUSE, index, &path);
		}
	};
	
	//TEXTURE TYPES
	/*
		aiTextureType_DIFFUSE = 0x1,
		aiTextureType_SPECULAR = 0x2,
		aiTextureType_AMBIENT = 0x3,
		aiTextureType_EMISSIVE = 0x4,
		aiTextureType_HEIGHT = 0x5,
		aiTextureType_NORMALS = 0x6,
		aiTextureType_SHININESS = 0x7,
		aiTextureType_OPACITY = 0x8,
		aiTextureType_DISPLACEMENT = 0x9,
		aiTextureType_LIGHTMAP = 0xA,
		aiTextureType_REFLECTION = 0xB,
	*/
	
	/*
	FindTextures1(material, aiTextureType_DIFFUSE, textureIdMap);
	FindTextures1(material, aiTextureType_SPECULAR, textureIdMap);
	FindTextures1(material, aiTextureType_HEIGHT, textureIdMap);
	
	FindTextures2(material, aiTextureType_DIFFUSE, textureIdMap);
	FindTextures2(material, aiTextureType_SPECULAR, textureIdMap);
	FindTextures2(material, aiTextureType_HEIGHT, textureIdMap);
	*/
}
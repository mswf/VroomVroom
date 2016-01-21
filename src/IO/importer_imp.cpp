#include "importer_imp.h"
#include <fstream>

#include "importer.hpp"
#include "postprocess.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../IO/stb_image.h"

#include "../DataStructure/material.h"

ImporterImp::ImporterImp() :
	import_scene_failure_reason( "No errors logged." ),
	import_image_failure_reason( "No errors logged." ),
	importFlags( aiProcessPreset_TargetRealtime_Quality )
{}

ImporterImp::~ImporterImp()
{}

uint8* ImporterImp::ImportImage( const char* filename, uint32& width, uint32& height, uint32 requiring_components, IMPORTER_MESSAGE& err_msg, bool vertical_flip )
{
	stbi_set_flip_vertically_on_load(vertical_flip);
	int32 comp, w, h;
	int32 rc = STBI_rgb;

	uint8* image = stbi_load( filename, &w, &h, &comp, rc);

	width = w;
	height = h;

	if ( image == nullptr )
	{
		import_image_failure_reason = std::string(stbi__g_failure_reason);
		err_msg = IMPORTER_MESSAGE::IMAGE_FAILED_TO_LOAD;
		return nullptr;
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
		return nullptr;
	}

	const aiScene* sc = importer.ReadFile( pFile, importFlags );

	if ( !sc )
	{
		// If the import failed, save error for request
		import_scene_failure_reason = importer.GetErrorString();
		msg = IMPORTER_MESSAGE::FILE_CORRUPT;
		return nullptr;
	}

	msg = IMPORTER_MESSAGE::FILE_OK;
	return importer.GetOrphanedScene();
}

void ImporterImp::FreeScene( aiScene*& sc )
{
	if ( sc != nullptr )
	{
		delete sc;
		sc = nullptr;
	}
}

void ImporterImp::ExtractMesh( const aiMesh* mesh, Mesh*& m )
{
	m = new Mesh();

	// buffer for indices
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
			PushToList( mesh->mVertices[i], m->vertices );
		}
	}

	// buffer for vertex normals
	if ( mesh->HasNormals() )
	{
		m->hasNormals = true;
		for ( i = 0; i < mesh->mNumVertices; ++i )
		{
			PushToList( mesh->mNormals[i], m->normals );
		}
	}

	// buffer for bitangents & tangents
	if ( mesh->HasTangentsAndBitangents() )
	{
		m->hasTangentsAndBitangets = true;
		for ( i = 0; i < mesh->mNumVertices; ++i )
		{
			PushToList( mesh->mBitangents[i], m->bitangents );
			PushToList( mesh->mTangents[i], m->tangents );
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

void ImporterImp::PushToList( aiVector3D v, std::vector< glm::vec3 >& list )
{
	glm::vec3 vec( v.x, v.y, v.z );
	list.push_back( vec );
}

void ImporterImp::ExtractMaterial( const aiMaterial* mtl, Material*& material, std::vector< string >* textureIdMap )
{
	material = new Material();
	uint32 max = 1;
	int32 wireframe = 0;
	int32 two_sided = 0;
	aiString name( "No name" );
	float opacity = 0.0;
	float bumpScaling = 0.0;
	float shininess = 0.0;
	float shininess_strenght = 0.0;

	aiGetMaterialString( mtl, AI_MATKEY_NAME, &name);
	material->name = name.C_Str();

	aiGetMaterialIntegerArray( mtl, AI_MATKEY_ENABLE_WIREFRAME, &wireframe, &max );
	material->wireframe_enabled = wireframe;

	aiGetMaterialIntegerArray( mtl, AI_MATKEY_TWOSIDED, &two_sided, &max);
	material->two_sided = two_sided;

	aiGetMaterialFloatArray( mtl, AI_MATKEY_OPACITY, &opacity, &max);
	material->opacity = opacity;

	aiGetMaterialFloatArray( mtl, AI_MATKEY_BUMPSCALING, &bumpScaling, &max);
	material->bump_scaling = bumpScaling;

	aiGetMaterialFloatArray( mtl, AI_MATKEY_SHININESS, &shininess, &max);
	material->shininess = shininess;

	aiGetMaterialFloatArray( mtl, AI_MATKEY_SHININESS_STRENGTH, &shininess_strenght, &max);
	material->shininess_strenght = shininess_strenght;

	SetColour(mtl, "$clr.diffuse", material->diffuse );
	SetColour(mtl, "$clr.ambient", material->ambient );
	SetColour(mtl, "$clr.specular", material->specular );
	SetColour(mtl, "$clr.emissive", material->emissive );
	SetColour(mtl, "$clr.transparent", material->transparent );
	SetColour(mtl, "$clr.reflective", material->reflective );

	auto FindTextures = []( const aiMaterial * material_to_match, aiTextureType type, uint32 & textureCount, std::vector< string >* textureIdMap_to_find )
	{
		uint32 i;
		aiString path;
		int materialCount = material_to_match->GetTextureCount(type);
		for (i = 0; i < materialCount; ++i)
		{
			if ( AI_SUCCESS == material_to_match->GetTexture( type, i, &path ) )
			{
				textureIdMap_to_find->push_back( std::string( &path.data[0] ) );
				++textureCount;
			}
		}
	};

	FindTextures( mtl, aiTextureType_DIFFUSE, material->textureCount, textureIdMap );
	FindTextures( mtl, aiTextureType_SPECULAR, material->textureCount, textureIdMap );
	FindTextures( mtl, aiTextureType_HEIGHT, material->textureCount, textureIdMap );
}

void ImporterImp::SetColour( const aiMaterial* mtl, const char* attribute, float* mtl_colour )
{
	float assign_color[4] = { 0.0, 0.0, 0.0, 1.0 };
	aiColor4D colour;
	if ( AI_SUCCESS == aiGetMaterialColor( mtl, attribute, 0, 0, &colour ) )
	{
		assign_color[0] = colour.r;
		assign_color[1] = colour.g;
		assign_color[2] = colour.b;
		assign_color[3] = colour.a;
	}
	memcpy( mtl_colour, assign_color, sizeof(assign_color) );
}

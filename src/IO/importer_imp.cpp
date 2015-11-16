#include "importer_imp.h"
#include <fstream>

#include "importer.hpp"
#include "postprocess.h"

//#define STB_IMAGE_IMPLEMENTATION
//#include "../IO/stb_image.h"

#include "../glm/vec2.hpp"
#include "../glm/vec3.hpp"

ImporterImp::ImporterImp():
	importFlags(aiProcessPreset_TargetRealtime_Quality),
	loadedScene(NULL),
	loadedImage(NULL),
	importSceneError("No errors logged."),
	importImageError("No errors logged.")
{}

ImporterImp::~ImporterImp()
{
	FreeScene( loadedScene );
	FreeImage( loadedImage );
}

int ImporterImp::ImportImage( const char* filename )
{
	//stbi_set_flip_vertically_on_load(true);
	int w, h, comp;
	//int req_comp = STBI_rgb_alpha;
	
	FreeImage(loadedImage);
	//loadedImage = stbi_load( filename, &w, &h, &comp, req_comp);
	
	if( loadedImage == nullptr )
	{
		//importImageError = std::string(stbi__g_failure_reason);
		return (int)IMPORTER_MESSAGE::TEXTURE_FAILED_TO_LOAD;
	}
	
	return (int)IMPORTER_MESSAGE::TEXTURE_LOAD_OK;
}

void ImporterImp::FreeImage( unsigned char* img )
{
	if (img != NULL)
	{
		//stbi_image_free(img);
		img = NULL;
	}
}

int ImporterImp::ImportObjFile( const std::string& pFile )
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
	
	const aiScene* scene = importer.ReadFile( pFile, importFlags );
 
	if( !scene )
	{
		// If the import failed, save error for request
		importSceneError = importer.GetErrorString();
		return (int)IMPORTER_MESSAGE::FILE_CORRUPT;
	}
 
	FreeScene( loadedScene );
	loadedScene = importer.GetOrphanedScene();
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

void ImporterImp::ExtractMaterial( const aiMaterial* material )
{
	int materialTextureCount = 0;
	
	// 0: Name
	// 1: Shading mode
	// 2: Color ambient
	// 3: Color diffuse
	// 4: Color specular
	// 5: Color emissive
	// 6: Shininess
	// 7: Opacity
	// 8: Refractive
	// 9: Texture file
	
	int i, j, k;
	
	// DIFFUSE
	for ( i = 0; i < material->GetTextureCount(aiTextureType_DIFFUSE); ++i )
	{
		aiString path;
		if ( AI_SUCCESS == material->GetTexture( aiTextureType_DIFFUSE, i, &path ) )
		{
			//TEXTURE INDEX unsigned int texId = textureIdMap[texPath.data];
			//TEXTURE INDEX aMesh.texIndex = texId;
			
			//TEXTURE COUNT aMat.texCount = 1;
			++materialTextureCount;
		}
	}
	
	// SPECULAR
	for ( j = 0; j < material->GetTextureCount(aiTextureType_SPECULAR); ++j )
	{
		aiString path;
		if ( AI_SUCCESS == material->GetTexture( aiTextureType_SPECULAR, j, &path ) )
		{
			//bind texture
			//TEXTURE INDEX unsigned int texId = textureIdMap[texPath.data];
			//TEXTURE INDEX aMesh.texIndex = texId;
			//TEXTURE COUNT aMat.texCount = 1;
		}
	}
	
	// HEIGHT / NORMAL
	for ( k = 0; k < material->GetTextureCount(aiTextureType_HEIGHT); ++k )
	{
		aiString path;
		if ( AI_SUCCESS == material->GetTexture( aiTextureType_HEIGHT, k, &path ) )
		{
			//bind texture
			//TEXTURE INDEX unsigned int texId = textureIdMap[texPath.data];
			//TEXTURE INDEX aMesh.texIndex = texId;
			//TEXTURE COUNT aMat.texCount = 1;
		}
	}
	
}

void ImporterImp::SetSceneImportFlags( int flags )
{
	importFlags = flags;
}

const std::string& ImporterImp::GetError( IMPORTER_ERROR_TYPE type ) const
{
	switch (type)
	{
		case IMPORTER_ERROR_TYPE::SCENE:
		{
			return importSceneError;
			break;
		}
		case IMPORTER_ERROR_TYPE::IMAGE:
		{
			return importImageError;
			break;
		}
		default:
		{
			return unknownError;
			break;
		}
	}
}
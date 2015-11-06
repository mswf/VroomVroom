#include "resource_manager.h"
#include "importer.hpp"
#include "../console.h"
#include <strstream>
#include <fstream>

ResourceManager::ResourceManager() :
	rMeshes( NULL ),
	rTextures( NULL )
{
	rMeshes = new std::vector< Mesh* >();
}

ResourceManager::~ResourceManager()
{
	//delete rMeshes;
	//delete rTextures;
}

bool ResourceManager::ImportObjFile( const std::string& pFile, int flags )
{
	Assimp::Importer importer;
	
	//check if file exists
	std::ifstream fin( pFile.c_str() );
	if( !fin.fail() )
	{
		fin.close();
	}
	else
	{
		Terminal.Log( string( "Couldn't open file: " + pFile ) );
		return false;
	}

	const aiScene* scene = importer.ReadFile( pFile, flags );
 
	// If the import failed, report it
	if( !scene)
	{
		Terminal.Log( importer.GetErrorString() );
		return false;
	}
 
	// Now we can access the file's contents.
	Terminal.Log("Import of scene succeeded.");
	LoadMesh(scene);
	return true;
}

void ResourceManager::LoadMesh(const aiScene* sc)
{
	Mesh* mesh = new Mesh();
	// For each mesh
	for (unsigned int n = 0; n < sc->mNumMeshes; ++n)
	{
		const aiMesh* m = sc->mMeshes[n];
		
		// create array with faces
		// have to convert from Assimp format to array
		unsigned int *faceArray;
		faceArray = (unsigned int *)malloc(sizeof(unsigned int) * m->mNumFaces * 3);
		unsigned int faceIndex = 0;
		
		for (unsigned int t = 0; t < m->mNumFaces; ++t)
		{
			const aiFace* face = &m->mFaces[t];
			
			memcpy(&faceArray[faceIndex], face->mIndices,3 * sizeof(unsigned int));
			faceIndex += 3;
		}
//#warning "Get Num of faces"
		//aMesh.numFaces = sc->mMeshes[n]->mNumFaces;
		
		// buffer for vertex positions
		if (m->HasPositions())
		{
			for (int i = 0; i < m->mNumVertices; ++i)
			{
				aiVector3D v =m->mVertices[i];
				mesh->vertices.push_back( glm::vec3( v.x, v.y, v.z ) );
			}
		}
		
		// buffer for vertex normals
		if (m->HasNormals())
		{
			for (int i = 0; i < m->mNumVertices; ++i)
			{
				aiVector3D n =m->mNormals[i];
				mesh->vertices.push_back( glm::vec3( n.x, n.y, n.z ) );
			}
		}
		
//#warning "UVs not imported yet"
		/*
		// buffer for vertex texture coordinates
		if (m->HasTextureCoords(0))
		{
			float *texCoords = (float *)malloc( sizeof(float) * 2 * m->mNumVertices );
			for (unsigned int k = 0; k < m->mNumVertices; ++k) {
				
				texCoords[k*2]   = m->mTextureCoords[0][k].x;
				texCoords[k*2+1] = m->mTextureCoords[0][k].y;
				
			}
		}
		*/
		
		// MATERIAL IMPORTING HERE ALSO???
		
		rMeshes->push_back( mesh );
	}
	std::strstream s;
	s << "Number of meshs: " << (uint32)rMeshes->size();
	Terminal.Log( s.str() );
}

void ResourceManager::LoadTexture()
{
	
}

const Mesh* ResourceManager::CreateTriangleMesh()
{
	Mesh* triangleMesh = new Mesh();
	
	std::vector< glm::vec3 > verts;
	std::vector< glm::vec2 > uv;
	std::vector< glm::vec3 > normals;
	std::vector< unsigned char > ind;
	
	verts.push_back(glm::vec3( 0.0f, 0.0f, 0.0f ));
	verts.push_back(glm::vec3( 0.5f, 0.0f, 0.0f ));
	verts.push_back(glm::vec3( 0.25f, 0.5f, 0.0f ));
	
	uv.push_back(glm::vec2( 0.0f, 0.0f ));
	uv.push_back(glm::vec2( 1.0f, 0.0f ));
	uv.push_back(glm::vec2( 0.5f, 1.0f ));
	
	normals.push_back(glm::vec3( 0.0f, 1.0f, 0.0f ));
	normals.push_back(glm::vec3( 0.0f, 1.0f, 0.0f ));
	normals.push_back(glm::vec3( 0.0f, 1.0f, 0.0f ));
	
	ind.push_back(0);
	ind.push_back(1);
	ind.push_back(2);

	triangleMesh->vertices = verts;
	triangleMesh->uvs = uv;
	triangleMesh->normals = normals;
	triangleMesh->indices = ind;
	
	rMeshes->push_back( triangleMesh );
	
	return rMeshes->back();
}

void ResourceManager::CreateCubeMesh( bool centered )
{
	Mesh* triangleMesh = new Mesh();
	
	std::vector< glm::vec3 > vertices;
	//std::vector< glm::vec2 > uv;
	//std::vector< glm::vec3 > normals;
	std::vector< unsigned char > indices;
	
	float offset = centered ? 0.5f : 0.0f;
	int i, j;
	
	// VERTICES
	for ( i = 0; i < 8; i++ )
	{
		float x = ( (i & 1) == 0 ? 0.0f : 1.0f );
		float y = ( (i & 2) == 0 ? 0.0f : 1.0f );
		float z = ( (i & 4) == 0 ? 0.0f : 1.0f );
		vertices.push_back( glm::vec3( x, y, z ) - offset );
		triangleMesh->vertices = vertices;
	}
	
	// INDICES
	const int indiceCount = 36;
	const unsigned char indiceArray[indiceCount] =
	{
		0,2,3, 0,3,1, // Bottom
		
		1,3,7, 1,7,5, // Right
		
		5,7,6, 5,6,4, // Top
		
		4,6,2, 4,2,0, // Left
		
		2,6,7, 2,7,3,  // Back
		
		1,5,4, 1,4,0  // Front
	};
	
	for (j = 0; j < indiceCount; ++j)
	{
		indices.push_back( indiceArray[j] );
		triangleMesh->indices = indices;
	}
	
	rMeshes->push_back( triangleMesh );
	
}

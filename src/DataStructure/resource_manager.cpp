#include "resource_manager.h"
#include "importer.hpp"
#include "../console.h"
#include <fstream>

ResourceManager::ResourceManager() :
	rMeshes( NULL ),
	rTextures( NULL )
{
	rMeshes = new std::vector< Mesh* >();
}

ResourceManager::~ResourceManager()
{
	//TODO(Valentinas): Clean up the resources
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
	Terminal.Log( "Import of scene succeeded." );
	LoadMesh(scene);
	return true;
}

void ResourceManager::LoadMesh(const aiScene* sc)
{
	Mesh* mesh = new Mesh();
	Terminal.Log( "Num of children: " + std::to_string( sc->mRootNode->mNumChildren ) );
	
	// For each mesh
	for (unsigned int n = 0; n < sc->mNumMeshes; ++n)
	{
		const aiMesh* m = sc->mMeshes[n];
		
		// buffer for indices
		mesh->numIndices = sc->mMeshes[n]->mNumFaces * 3;
		for (unsigned int t = 0; t < m->mNumFaces; ++t)
		{
			const aiFace* face = &m->mFaces[t];
			mesh->indices.push_back(face->mIndices[0]);
			mesh->indices.push_back(face->mIndices[1]);
			mesh->indices.push_back(face->mIndices[2]);
		}
		
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
			mesh->hasNormals = true;
			for (int i = 0; i < m->mNumVertices; ++i)
			{
				aiVector3D n =m->mNormals[i];
				mesh->normals.push_back( glm::vec3( n.x, n.y, n.z ) );
			}
		}
		mesh->hasUVs = false;
		
		// buffer for vertex texture coordinates
		if (m->HasTextureCoords(0))
		{
			 mesh->hasUVs = true;
			//float *texCoords = (float *)malloc( sizeof(float) * 2 * m->mNumVertices );
			for (unsigned int k = 0; k < m->mNumVertices; ++k)
			{
				mesh->uvs.push_back( glm::vec2( m->mTextureCoords[0][k].x, m->mTextureCoords[0][k].y ) );
				//texCoords[k*2]   = m->mTextureCoords[0][k].x;
				//texCoords[k*2+1] = m->mTextureCoords[0][k].y;
			}
		}
		
		// MATERIAL IMPORTING HERE ALSO???
		
		rMeshes->push_back( mesh );
	}
	
	tempMesh = rMeshes->front();
	//Terminal.Log( std::string( "Number of meshs: " + std::to_string(rMeshes->size()) ) );
}

void ResourceManager::LoadTexture()
{
	
}

const Mesh* ResourceManager::CreateTriangleMesh()
{	
	Mesh* triangleMesh = new Mesh();
	
	std::vector< glm::vec3 > vertices;
	std::vector< glm::vec2 > uv;
	std::vector< glm::vec3 > normals;
	std::vector< unsigned int > indices;
	
	vertices.push_back(glm::vec3( 0.0f, 0.0f, 0.0f ));
	vertices.push_back(glm::vec3( 0.5f, 0.0f, 0.0f ));
	vertices.push_back(glm::vec3( 0.25f, 0.5f, 0.0f ));
	
	uv.push_back(glm::vec2( 0.0f, 0.0f ));
	uv.push_back(glm::vec2( 1.0f, 0.0f ));
	uv.push_back(glm::vec2( 0.5f, 1.0f ));
	triangleMesh->hasUVs = true;
	
	normals.push_back(glm::vec3( 0.0f, 1.0f, 0.0f ));
	normals.push_back(glm::vec3( 0.0f, 1.0f, 0.0f ));
	normals.push_back(glm::vec3( 0.0f, 1.0f, 0.0f ));
	triangleMesh->hasNormals = true;
	
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);

	triangleMesh->vertices = vertices;
	triangleMesh->uvs = uv;
	triangleMesh->normals = normals;
	triangleMesh->indices = indices;
	triangleMesh->numIndices = 3;
	
	rMeshes->push_back( triangleMesh );
	
	return rMeshes->back();
}

/*
	 0		  3
     *-------*			+y
  1	/|	  2	/|			 ^	Right hand systen
   *-------* |			 |
   | |4	   | |7			 |
   | *-----|-*			 +-----> +x
   |/	   |/           /
   *-------*		   v    CCW is positive rotation
  5		  6			 +z
 
 */


const Mesh* ResourceManager::CreateCubeMesh( bool centered )
{
	printf("Creating Cube mesh \n");
	
	Mesh* triangleMesh = new Mesh();
	
	std::vector< glm::vec3 > vertices;
	//std::vector< glm::vec2 > uv;
	//std::vector< glm::vec3 > normals;
	std::vector< unsigned int > indices;
	
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
	const unsigned int indiceArray[indiceCount] =
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
	}
	triangleMesh->indices = indices;
	triangleMesh->numIndices = 36;
	
	rMeshes->push_back( triangleMesh );
	
	return rMeshes->back();
}

#ifndef mesh_generator_h
#define mesh_generator_h

#include "data_types.h"
/*
 const Mesh* CreateTriangleMesh()
 {
	Mesh triangleMesh;
	
	triangleMesh.vertices.push_back(glm::vec3( 0.0f, 0.0f, 0.0f ));
	triangleMesh.vertices.push_back(glm::vec3( 0.5f, 0.0f, 0.0f ));
	triangleMesh.vertices.push_back(glm::vec3( 0.25f, 0.5f, 0.0f ));
	
	triangleMesh.uvs.push_back(glm::vec2( 0.0f, 0.0f ));
	triangleMesh.uvs.push_back(glm::vec2( 1.0f, 0.0f ));
	triangleMesh.uvs.push_back(glm::vec2( 0.5f, 1.0f ));
	triangleMesh.hasUVs = true;
	
	triangleMesh.normals.push_back(glm::vec3( 0.0f, 1.0f, 0.0f ));
	triangleMesh.normals.push_back(glm::vec3( 0.0f, 1.0f, 0.0f ));
	triangleMesh.normals.push_back(glm::vec3( 0.0f, 1.0f, 0.0f ));
	triangleMesh.hasNormals = true;
	
	triangleMesh.indices.push_back(0);
	triangleMesh.indices.push_back(1);
	triangleMesh.indices.push_back(2);
	triangleMesh.numIndices = 3;
	
	meshes.push_back( triangleMesh );
	
	return &meshes.back();
 }
 
 const Mesh* CreateTetrahedronMesh()
 {
	Mesh* tetrahedronMesh = new Mesh();
	
	// VERTICES
	tetrahedronMesh->vertices.push_back( glm::vec3( 0.0f, 0.0f, 0.0f ) );
	tetrahedronMesh->vertices.push_back( glm::vec3( 1.0f, 0.0f, 0.0f ) );
	tetrahedronMesh->vertices.push_back( glm::vec3( 0.0f, 1.0f, 0.0f ) );
	tetrahedronMesh->vertices.push_back( glm::vec3( 0.0f, 0.0f, 1.0f ) );
	
	// INDICES
	const int indiceCount = 12;
	const unsigned int indiceArray[indiceCount] =
	{
 1,3,2,
 1,4,3,
 1,2,4,
 2,3,4
	};
	
	int i;
	for ( i = 0; i < indiceCount; ++i )
	{
 tetrahedronMesh->indices.push_back( indiceArray[i] );
	}
	tetrahedronMesh->numIndices = indiceCount;
	
 #warning NO CACHING MESH IN GENERATOR
	//meshes.push_back( tetrahedronMesh );
	
	return &meshes.back();
 }
 */

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


void CreateCube( Mesh* mesh, const float offset = 0.0f )
{
	int i, j;

	// VERTICES
	for ( i = 0; i < 8; i++ )
	{
		float x = ( (i & 1) == 0 ? 0.0f : 1.0f );
		float y = ( (i & 2) == 0 ? 0.0f : 1.0f );
		float z = ( (i & 4) == 0 ? 0.0f : 1.0f );
		mesh->vertices.push_back( glm::vec3( x, y, z ) - offset );
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
		mesh->indices.push_back( indiceArray[j] );
	}
	mesh->numIndices = indiceCount;
}




#endif /* mesh_generator_h */

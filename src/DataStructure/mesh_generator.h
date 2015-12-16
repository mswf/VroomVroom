#ifndef mesh_generator_h
#define mesh_generator_h

#include "data_types.h"
#include "resource_manager.h"

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

void EnvironmentCube()
{
	ModelInstance* instance = new ModelInstance();
	float scale = 20.0f;
	glm::vec3 points[] =
	{
		glm::vec3(-1.0f,  1.0f, -1.0f ) * scale,
		glm::vec3(-1.0f, -1.0f, -1.0f ) * scale,
		glm::vec3( 1.0f, -1.0f, -1.0f ) * scale,
		glm::vec3( 1.0f, -1.0f, -1.0f ) * scale,
		glm::vec3( 1.0f,  1.0f, -1.0f ) * scale,
		glm::vec3(-1.0f,  1.0f, -1.0f ) * scale,

		glm::vec3(-1.0f, -1.0f,  1.0f ) * scale,
		glm::vec3(-1.0f, -1.0f, -1.0f ) * scale,
		glm::vec3(-1.0f,  1.0f, -1.0f ) * scale,
		glm::vec3(-1.0f,  1.0f, -1.0f ) * scale,
		glm::vec3(-1.0f,  1.0f,  1.0f ) * scale,
		glm::vec3(-1.0f, -1.0f,  1.0f ) * scale,

		glm::vec3( 1.0f, -1.0f, -1.0f ) * scale,
		glm::vec3( 1.0f, -1.0f,  1.0f ) * scale,
		glm::vec3( 1.0f,  1.0f,  1.0f ) * scale,
		glm::vec3( 1.0f,  1.0f,  1.0f ) * scale,
		glm::vec3( 1.0f,  1.0f, -1.0f ) * scale,
		glm::vec3( 1.0f, -1.0f, -1.0f ) * scale,

		glm::vec3(-1.0f, -1.0f,  1.0f ) * scale,
		glm::vec3(-1.0f,  1.0f,  1.0f ) * scale,
		glm::vec3( 1.0f,  1.0f,  1.0f ) * scale,
		glm::vec3( 1.0f,  1.0f,  1.0f ) * scale,
		glm::vec3( 1.0f, -1.0f,  1.0f ) * scale,
		glm::vec3(-1.0f, -1.0f,  1.0f ) * scale,

		glm::vec3(-1.0f,  1.0f, -1.0f ) * scale,
		glm::vec3( 1.0f,  1.0f, -1.0f ) * scale,
		glm::vec3( 1.0f,  1.0f,  1.0f ) * scale,
		glm::vec3( 1.0f,  1.0f,  1.0f ) * scale,
		glm::vec3(-1.0f,  1.0f,  1.0f ) * scale,
		glm::vec3(-1.0f,  1.0f, -1.0f ) * scale,

		glm::vec3(-1.0f, -1.0f, -1.0f ) * scale,
		glm::vec3(-1.0f, -1.0f,  1.0f ) * scale,
		glm::vec3( 1.0f, -1.0f, -1.0f ) * scale,
		glm::vec3( 1.0f, -1.0f, -1.0f ) * scale,
		glm::vec3(-1.0f, -1.0f,  1.0f ) * scale,
		glm::vec3( 1.0f, -1.0f,  1.0f ) * scale
	};
	
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 36 * sizeof (glm::vec3), &points, GL_STATIC_DRAW);
	
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	
	instance->vao = vao;
	instance->vbo = vbo;
	instance->numIndices = 36;
	Assets.InsertModelInstance( "__Skybox_model", instance );
}

void Quad()
{
	ModelInstance* instance = new ModelInstance();
	glm::vec2 texcoords[4] =
	{
		glm::vec2( 0.0, 1.0 ),
		glm::vec2( 1.0, 1.0 ),
		glm::vec2( 1.0, 0.0 ),
		glm::vec2( 0.0, 0.0 )
		
	};
	
	glm::vec3 vertices[4] =
	{
		glm::vec3( -1.0, -1.0, 0.0 ),
		glm::vec3(  1.0, -1.0, 0.0 ),
		glm::vec3(  1.0,  1.0, 0.0 ),
		glm::vec3( -1.0,  1.0, 0.0 )
	};
	
	unsigned int indices[6] = { 0,1,2, 2,3,0 };
	
	GLuint vao;
	glGenVertexArrays (1, &vao);
	glBindVertexArray (vao);
	
	
	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), &indices, GL_STATIC_DRAW);
	
	GLuint vbo;
	glGenBuffers (1, &vbo);
	glBindBuffer (GL_ARRAY_BUFFER, vbo);
	glBufferData (GL_ARRAY_BUFFER, 4 * sizeof(glm::vec3) + 4 * sizeof(glm::vec2), NULL, GL_STATIC_DRAW);
	
	glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * 4, &vertices );
	glEnableVertexAttribArray (0);
	glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	
	
	glBufferSubData( GL_ARRAY_BUFFER, sizeof(glm::vec3) * 4, sizeof(glm::vec2) * 4, &texcoords );
	glEnableVertexAttribArray (1);
	glVertexAttribPointer (1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(glm::vec3) * 4));
	
	instance->vao = vao;
	instance->vbo = vbo;
	instance->indiceBuffer = ebo;
	instance->numIndices = 6;
	Assets.InsertModelInstance( "__Quad_model", instance );
}


#endif /* mesh_generator_h */

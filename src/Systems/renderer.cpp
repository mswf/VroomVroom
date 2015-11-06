#include "../DataStructure/shader.h"
#include "../Systems/renderer.h"
#include "../Components/cTransform.h"
#include "../Components/cCamera.h"
#include "console.h"

namespace Renderer
{
	
	void Render( glm::uint32 time, Entity* camera, Entity* object )
	{
		
		Shader* s =			Entity::GetComponent<CMaterial>(object)->shader;
		CTransform* trans = Entity::GetComponent<CTransform>(object);
		CMesh* mesh =		Entity::GetComponent<CMesh>(object);
		CCamera* cam =		Entity::GetComponent<CCamera>(camera);
		
		glBindVertexArray( mesh->vao );
		s->ValidateProgram();
		
		glUseProgram(s->program);
		
		s->SetUniformMat4( "model", trans->GetTransform() );
		s->SetUniformMat4( "view", cam->GetViewMatrix() );
		s->SetUniformMat4( "projection", cam->GetProjectionMatrix() );
		s->SetUniformFloat( "time", (float)time );

		glDrawElements( GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, 0 );
		
		glBindVertexArray( 0 );
		glUseProgram(0);
	}

	/*
		0		3
		 *-------*			+y
	 1	/|	  2	/|			 ^	Right hand systen
	   *-------* |			 |
	   | |4	   | |7			 |
	   | *-----|-*			 +-----> +x
	   |/	   |/           /
	   *-------*		   v    CCW is positive rotation
	 5		  6			 +z
	 
	 */
	
	void GenerateTriangle( Entity* e )
	{
		Vertex triangle[3];
		
		glm::vec3 v0( 0.0f, 0.0f, 0.0f );
		glm::vec3 v1( 0.5f, 0.0f, 0.0f );
		glm::vec3 v2( 0.5f, 0.5f, 0.0f );
		
		triangle[0] = { v0 };
		triangle[1] = { v1 };
		triangle[2] = { v2 };
		
		const GLubyte indices[3] = { 0,1,2 };
		
		GenerateBuffers( e, triangle, 3, indices, 3 );
	}
	
	void GenerateCube( Entity* e, bool centered )
	{
		float offset = centered ? 0.5f : 0.0f;
		Vertex cube[8];
		int i;
		for ( i = 0; i < 8; i++ )
		{
			float x = ( (i & 1) == 0 ? 0.0f : 1.0f );
			float y = ( (i & 2) == 0 ? 0.0f : 1.0f );
			float z = ( (i & 4) == 0 ? 0.0f : 1.0f );
			
			Vertex vert = { glm::vec3( x, y, z ) - offset, glm::vec3( x, y, z ) };
			cube[i] = vert;
		}
		 
		const GLubyte indices[36] =
		{
			0,2,3, 0,3,1, // Bottom
			
			1,3,7, 1,7,5, // Right
			
			5,7,6, 5,6,4, // Top
			
			4,6,2, 4,2,0, // Left
			
			2,6,7, 2,7,3,  // Back
			
			1,5,4, 1,4,0  // Front
		};

		GenerateBuffers( e, cube, 8, indices, 36 );
	}
	
	void GenerateBuffers( Entity* e, const Vertex* vertices, GLuint verticeCount, const GLubyte* indices, GLuint indiceCount )
	{
		CMesh* mesh = Entity::GetComponent<CMesh>(e);
		CMaterial* mat = Entity::GetComponent<CMaterial>(e);
		
		GLuint vao;
		GLuint vbo;
		GLuint eab;
		
		//VAO always goes first before loading data to VBO
		glGenVertexArrays( 1, &vao );
		glBindVertexArray( vao );
		
		//VBO, allocate data and upload data from CPU to GPU
		glGenBuffers( 1, &vbo );
		glBindBuffer( GL_ARRAY_BUFFER, vbo );
		glBufferData( GL_ARRAY_BUFFER, verticeCount * sizeof(Vertex), vertices, GL_STATIC_DRAW);
		
		//EBO, storing indices
		glGenBuffers( 1, &eab);
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, eab );
		glBufferData( GL_ELEMENT_ARRAY_BUFFER, indiceCount, indices, GL_STATIC_DRAW);
		
		GLuint position = glGetAttribLocation( mat->shader->program, "position" );
		GLuint texcoord = glGetAttribLocation( mat->shader->program, "texcoord" );
		
		glEnableVertexAttribArray( position );
		glEnableVertexAttribArray( texcoord );
		
		glVertexAttribPointer( position, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0 );
		glVertexAttribPointer( texcoord, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*) sizeof(glm::vec3) );
		
		//mesh->vertexLoc = position;
		//mesh->texCoordLoc = texcoord;
		mesh->vao = vao;
		mesh->eab = eab;
		mesh->vbo = vbo;
		
		glBindVertexArray( 0 );
		glBindBuffer( GL_ARRAY_BUFFER, 0 );
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
	}
	
	void ClearBuffers( CMesh* mesh )
	{
		//glDisableVertexArrayAttrib( 1, mesh->vertexLoc );
		glDeleteVertexArrays( 1, &mesh->vao );
		glDeleteBuffers( GL_ARRAY_BUFFER, &mesh->vbo );
		glDeleteBuffers( GL_ELEMENT_ARRAY_BUFFER, &mesh->eab );
	}
	
} // NAMESPACE END
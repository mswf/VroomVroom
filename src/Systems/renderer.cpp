#include "../DataStructure/shader.h"
#include "../Systems/renderer.h"
#include "../Components/cMaterial.h"
#include "../Components/cTransform.h"
#include "../Components/cCamera.h"
#include <iostream>


namespace Renderer
{
	
	void Render( glm::uint32 time, Entity* camera, Entity* object )
	{
		
		CMaterial* mat =	Entity::GetComponent<CMaterial>(object);
		CTransform* trans = Entity::GetComponent<CTransform>(object);
		CMesh* mesh =		Entity::GetComponent<CMesh>(object);
		CCamera* cam =		Entity::GetComponent<CCamera>(camera);
		
		GLuint program = mat->shader->program;
		
		glClearColor( 0.2, 0.2, 0.2, 1.0 );
		glClear(GL_COLOR_BUFFER_BIT);
		glEnable( GL_CULL_FACE );
		glCullFace( GL_BACK );
		
		glUseProgram(program);
		
		mat->shader->SetUniformMat4( "model", trans->transform );
		mat->shader->SetUniformMat4( "view", cam->GetViewMatrix() );
		mat->shader->SetUniformMat4( "projection", cam->GetProjectionMatrix() );
		mat->shader->SetUniformFloat( "time", (float)time );

		// DRAWING PART
		
		glBindBuffer( GL_ARRAY_BUFFER, mesh->vertexBufferObject );
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER,  mesh->indexBufferObject );
		glBindVertexArray( mesh->vertexArrayObject );
		
		mat->shader->ValidateProgram();
		
		GLuint position = glGetAttribLocation( program, "position" );
		GLuint texcoord = glGetAttribLocation( program, "texcoord" );

		glEnableVertexAttribArray( position );
		glEnableVertexAttribArray( texcoord );
		
		glVertexAttribPointer( position, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0 );
		glVertexAttribPointer( texcoord, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) sizeof(glm::vec3) );
		
		glDrawElements( GL_TRIANGLES, mesh->numFaces, GL_UNSIGNED_BYTE, 0 );
		
		glBindVertexArray( 0 );
		glBindBuffer( GL_ARRAY_BUFFER, 0 );
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
		
		// DRAWING END
		
		glUseProgram(0);
	}

	void GenerateCube( CMesh* mesh )
	{
		mesh->numFaces = 36;
		
		Vertex cube[8];

		for (int i = 0; i < 8; i++)
		{
			float x = ( (i & 1) == 0 ? 0 : 1 );
			float y = ( (i & 2) == 0 ? 0 : 1 );
			float z = ( (i & 4) == 0 ? 0 : 1 );
			
			Vertex vert = { glm::vec3( x, y, z ) - 0.5f, glm::vec4( x, y, z, 1.0f ) };
			cube[i] = vert;
		}
		
		GLubyte indices[36] =
		{
			0,2,3, 0,3,1, // Bottom
			
			1,3,7, 1,7,5, // Right
			
			5,7,6, 5,6,4, // Top
			
			4,6,2, 4,2,0, // Left
			
			2,6,7, 2,7,3,  // Back
			
			1,5,4, 1,4,0  // Front
			
		};
		
		GLuint vao;
		GLuint vbo;
		GLuint ebo;
		
		std::cout << "Size of vertex: " << sizeof(Vertex) << std::endl;
		std::cout << "Size of 8 vertices: " << 8 * sizeof(Vertex) << std::endl;
		std::cout << "Size of indices: " << sizeof(indices) << std::endl;
		
		//VAO
		glGenVertexArrays( 1, &vao );
		glBindVertexArray( vao );
		
		//EBO
		glGenBuffers( 1, &ebo);
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ebo );
		glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		
		//VBO
		glGenBuffers( 1, &vbo );
		glBindBuffer( GL_ARRAY_BUFFER, vbo );
		glBufferData( GL_ARRAY_BUFFER, 8 * sizeof(Vertex), mesh, GL_STATIC_DRAW);
		
		
		
		mesh->vertexArrayObject = vao;
		mesh->indexBufferObject = ebo;
		mesh->vertexBufferObject = vbo;
		
		glBindVertexArray( 0 );
		glBindBuffer( GL_ARRAY_BUFFER, 0 );
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
	}
	
} // NAMESPACE END
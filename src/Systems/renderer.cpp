#include "../DataStructure/shader.h"
#include "../Systems/renderer.h"
#include "../Components/cTransform.h"
#include "../Components/cCamera.h"
#include "console.h"

namespace Renderer
{
	
	void Render( glm::uint32 time, Entity* camera, Entity* object )
	{
		
		CMaterial* mat =	Entity::GetComponent<CMaterial>(object);
		CTransform* trans = Entity::GetComponent<CTransform>(object);
		CMesh* mesh =		Entity::GetComponent<CMesh>(object);
		CCamera* cam =		Entity::GetComponent<CCamera>(camera);
		
		
		float offset = 0.5f;
		Vertex cube[8];
		int i;
		for ( i = 0; i < 8; i++ )
		{
			float x = ( (i & 1) == 0 ? 0 : 1 );
			float y = ( (i & 2) == 0 ? 0 : 1 );
			float z = ( (i & 4) == 0 ? 0 : 1 );
			
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
		
		GLuint program = mat->shader->program;
		
		GLuint vao = mesh->vertexArrayObject;
		GLuint vbo = mesh->vertexBufferObject;
		GLuint eab = mesh->elementArratBuffer;
		
		//VAO always goes first before loading data to VBO
		//glGenVertexArrays( 1, &vao );
		glBindVertexArray( vao );
		
		//VBO, allocate data and upload data from CPU to GPU
		//glGenBuffers( 1, &vbo );
		glBindBuffer( GL_ARRAY_BUFFER, vbo );
		//glBufferData( GL_ARRAY_BUFFER, 8 * sizeof(Vertex), cube, GL_STATIC_DRAW);
		
		//EBO, storing indices
		//glGenBuffers( 1, &eab);
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, eab );
		//glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices, GL_STATIC_DRAW);
		
		//glBindVertexArray( mesh->vertexArrayObject );
		//glBindBuffer( GL_ARRAY_BUFFER, mesh->vertexBufferObject );
		//glBindBuffer( GL_ELEMENT_ARRAY_BUFFER,  mesh->elementArratBuffer );
		
		GLuint position = glGetAttribLocation( mat->shader->program, "position" );
		GLuint texcoord = glGetAttribLocation( mat->shader->program, "texcoord" );
		
		glEnableVertexAttribArray( position );
		glEnableVertexAttribArray( texcoord );
		
		glVertexAttribPointer( position, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0 );
		glVertexAttribPointer( texcoord, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*) sizeof(glm::vec3) );

		//glBindVertexArray( 0 );
		//glBindBuffer( GL_ARRAY_BUFFER, 0 );
		//glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
		
		std::cout << vao << "," << vbo << "," << eab << std::endl;
		
		//glBindVertexArray( mesh->vertexArrayObject );
		//glBindBuffer( GL_ARRAY_BUFFER, mesh->vertexBufferObject );
		//glBindBuffer( GL_ELEMENT_ARRAY_BUFFER,  mesh->elementArratBuffer );

		glBindVertexArray( vao );
		glBindBuffer( GL_ARRAY_BUFFER, vbo );
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, eab );
		
		mat->shader->ValidateProgram();
		
		glUseProgram(program);
		
		mat->shader->SetUniformMat4( "model", trans->GetTransform() );
		mat->shader->SetUniformMat4( "view", cam->GetViewMatrix() );
		mat->shader->SetUniformMat4( "projection", cam->GetProjectionMatrix() );
		mat->shader->SetUniformFloat( "time", (float)time );

		glDrawElements( GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, 0 );
		
		glBindVertexArray( 0 );
		glBindBuffer( GL_ARRAY_BUFFER, 0 );
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
		
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
		Vertex triangle[4];
		
		glm::vec3 v0( 0.0f, 0.0f, 0.0f );
		glm::vec3 v1( 0.5f, 0.0f, 0.0f );
		glm::vec3 v2( 0.0f, 0.5f, 0.0f );
		glm::vec3 v3( 0.5f, 0.5f, 0.0f );
		
		triangle[0] = { v0 };
		triangle[1] = { v1 };
		triangle[2] = { v2 };
		triangle[3] = { v3 };
		
		const GLubyte indices[6] = { 0,1,2, 0,2,3 };
		
		GenerateBuffers( e, triangle, 4, 1, indices );
	}
	
	void GenerateCube( Entity* e, bool centered )
	{
		float offset = centered ? 0.5f : 0.0f;
		Vertex cube[8];
		int i;
		for ( i = 0; i < 8; i++ )
		{
			float x = ( (i & 1) == 0 ? 0 : 1 );
			float y = ( (i & 2) == 0 ? 0 : 1 );
			float z = ( (i & 4) == 0 ? 0 : 1 );
			
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

		GenerateBuffers( e, cube, 8, 6, indices );
	}
	
	void GenerateBuffers( Entity* e, const Vertex* verts, GLuint verticeCount, GLuint faceCount, const GLubyte* indices )
	{
		CMesh* mesh = Entity::GetComponent<CMesh>(e);
		//CMaterial* mat = Entity::GetComponent<CMaterial>(e);
		
		// faces * (triangles per face) * (points per triangle)
		mesh->numFaces = (faceCount * 2) * 3;
		
		GLuint vao;
		GLuint vbo;
		GLuint eab;
		/*
		std::cout << "Size of "<< verticeCount << " vertices: " << verticeCount * sizeof(Vertex) << std::endl;
		std::cout << "Size of indices: " << sizeof(indices) << std::endl;
		std::cout << "Indices: " << std::endl;
		int i;
		for ( i = 0; i < mesh->numFaces; i += 6 )
		{
			std::cout << "- " << indices[i] << ", " << indices[i+1] << ", " << indices[i+2] << ",  " <<
						 indices[i+3] << ", " << indices[i+4] << ", " << indices[i+5] << std::endl;
		}
		*/
		//VAO always goes first before loading data to VBO
		glGenVertexArrays( 1, &vao );
		glBindVertexArray( vao );
		
		//VBO, allocate data and upload data from CPU to GPU
		glGenBuffers( 1, &vbo );
		glBindBuffer( GL_ARRAY_BUFFER, vbo );
		glBufferData( GL_ARRAY_BUFFER, verticeCount * sizeof(Vertex), verts, GL_STATIC_DRAW);
		
		//EBO, storing indices
		glGenBuffers( 1, &eab);
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, eab );
		glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices, GL_STATIC_DRAW);
		
		//GLuint position = glGetAttribLocation( mat->shader->program, "position" );
		//GLuint texcoord = glGetAttribLocation( mat->shader->program, "texcoord" );
		
		//glEnableVertexAttribArray( position );
		//glEnableVertexAttribArray( texcoord );
		
		//glVertexAttribPointer( position, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0 );
		//glVertexAttribPointer( texcoord, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*) sizeof(glm::vec3) );
		
		//mesh->vertexLoc = position;
		//mesh->texCoordLoc = texcoord;
		mesh->vertexArrayObject = vao;
		mesh->elementArratBuffer = eab;
		mesh->vertexBufferObject = vbo;
		
		glBindVertexArray( 0 );
		glBindBuffer( GL_ARRAY_BUFFER, 0 );
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
	}
	
	void ClearBuffers( CMesh* mesh )
	{
		//glDisableVertexArrayAttrib( 1, mesh->vertexLoc );
		glDeleteVertexArrays( 1, &mesh->vertexArrayObject );
		glDeleteBuffers( GL_ARRAY_BUFFER, &mesh->vertexBufferObject );
		glDeleteBuffers( GL_ELEMENT_ARRAY_BUFFER, &mesh->elementArratBuffer );
	}
	
} // NAMESPACE END
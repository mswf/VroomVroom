#include "../DataStructure/shader.h"
#include "../Systems/renderer.h"
#include "../Components/cTransform.h"
#include "../Components/cCamera.h"
#include "console.h"

namespace Renderer
{
	
	void Render( glm::uint32 time, Entity* camera, Entity* object, Shader* s )
	{
		CTransform* trans = Entity::GetComponent<CTransform>(object);
		CCamera* cam =		Entity::GetComponent<CCamera>(camera);
		CMeshRenderer* rend = Entity::GetComponent<CMeshRenderer>(object);
		
		glBindVertexArray( rend->meshFilter->vao );
		//s->ValidateProgram();
		
		glUseProgram(s->program);
		
		s->SetUniformMat4( "model", trans->GetTransform() );
		s->SetUniformMat4( "view", cam->GetViewMatrix() );
		s->SetUniformMat4( "projection", cam->GetProjectionMatrix() );
		s->SetUniformFloat( "time", (float)time );

		glDrawElements( GL_TRIANGLES, rend->meshFilter->numIndices, GL_UNSIGNED_BYTE, 0 );
		
		glBindVertexArray( 0 );
		glBindBuffer( GL_ARRAY_BUFFER, 0 );
		glUseProgram(0);
	}
	
	/*
	 GLuint position = glGetAttribLocation( s->program, "position" );
	 GLuint texcoord = glGetAttribLocation( s->program, "texcoord" );
	 GLuint normal = glGetAttribLocation( s->program, "normal" );
	 glEnableVertexAttribArray( position );
	 glEnableVertexAttribArray( texcoord );
	 glEnableVertexAttribArray( normal );
	 glVertexAttribPointer( position, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0 );
	 glVertexAttribPointer( texcoord, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*) sizeof(glm::vec3) );
	 glVertexAttribPointer( normal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*) (sizeof(glm::vec3) + sizeof(glm::vec2)) );
		*/

	
} // NAMESPACE END
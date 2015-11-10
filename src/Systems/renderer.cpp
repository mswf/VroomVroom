#include "../DataStructure/shader.h"
#include "../Systems/renderer.h"
#include "../Components/cTransform.h"
#include "../Components/cCamera.h"
#include "console.h"
#include <iostream>

namespace Renderer
{
	
	void Render( glm::uint32 time, Entity* camera, Entity* object )
	{
		CTransform* trans = Entity::GetComponent<CTransform>(object);
		CCamera* cam =		Entity::GetComponent<CCamera>(camera);
		CMeshRenderer* rend = Entity::GetComponent<CMeshRenderer>(object);
		Shader* s = rend->material->shader;
		
		//glEnable(GL_PROGRAM_POINT_SIZE);
		glEnable(GL_CULL_FACE);
		glEnable(GL_BACK);
		glEnable(GL_DEPTH_TEST);
		
		glBindVertexArray( rend->vao );
		
		s->ValidateProgram();
		
		glUseProgram(s->program);
		
		s->SetUniformMat4( "model", trans->GetTransform() );
		s->SetUniformMat4( "view", cam->GetViewMatrix() );
		s->SetUniformMat4( "projection", cam->GetProjectionMatrix() );
		s->SetUniformFloat( "time", (float)time );
		
		glDrawElements( rend->drawType, rend->numIndices, GL_UNSIGNED_INT, (void*)0 );
		
		glBindVertexArray( 0 );
		glBindBuffer( GL_ARRAY_BUFFER, 0 );
		glUseProgram(0);
	}
	
} // NAMESPACE END
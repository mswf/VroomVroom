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
		//s->ValidateProgram();
		
		glUseProgram(s->program);
		
		glm::mat3 mvMatrix = glm::mat3( cam->GetViewMatrix()* trans->GetTransform() );
		glm::mat3 normalMatrix = glm::transpose(glm::inverse(mvMatrix));
		glm::vec3 lightPosition( 1.0 );
		
		s->SetUniformMat4( "model", trans->GetTransform() );
		s->SetUniformMat4( "view", cam->GetViewMatrix() );
		s->SetUniformMat3( "normalMatrix", normalMatrix );
		s->SetUniformMat3( "mvMatrix", mvMatrix );
		s->SetUniformMat4( "projection", cam->GetProjectionMatrix() );
		s->SetUniformFloat( "time", (float)time );
		s->SetUniformFloat3( "lightPos", lightPosition);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture( GL_TEXTURE_2D, rend->material->diff_texture );
		s->SetUniformInt( "colorMap", 0 );
		
		glActiveTexture(GL_TEXTURE1);
		glBindTexture( GL_TEXTURE_2D, rend->material->normal_texture );
		s->SetUniformInt( "normalMap", 1 );
		
		glDrawElements( rend->drawType, rend->numIndices, GL_UNSIGNED_INT, (void*)0 );
		
		glBindVertexArray( 0 );
		glBindBuffer( GL_ARRAY_BUFFER, 0 );
		
		//TODO(Valentinas): Disable Vertex Attributes after drawing
		
		//glBindTexture(GL_ACTIVE_TEXTURE, 0);
		glUseProgram(0);
	}
	
} // NAMESPACE END
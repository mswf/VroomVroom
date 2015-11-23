#include "../DataStructure/shader.h"
#include "../Systems/renderer.h"
#include "../Components/cTransform.h"
#include "../Components/cCamera.h"
#include "console.h"
#include "texture.h"
#include <iostream>

namespace Renderer
{
	
	void Render( glm::uint32 time, Entity* camera, Entity* object )
	{
		CTransform* trans = Entity::GetComponent<CTransform>(object);
		CCamera* cam =		Entity::GetComponent<CCamera>(camera);
		ModelInstance* m = Entity::GetComponent<CMeshRenderer>(object)->model;
		Material* mtl = Entity::GetComponent<CMeshRenderer>(object)->material;
		Shader* s = mtl->shader;
		
		if (m == NULL) return;
		
		//glEnable(GL_PROGRAM_POINT_SIZE);
		glEnable(GL_CULL_FACE);
		glEnable(GL_BACK);
		//glPolygonMode(GL_FRONT, m->material->wireframe_enabled ? GL_FILL : GL_LINE );
		glEnable(GL_DEPTH_TEST);
		
		glBindVertexArray( m->vao );
		
		mtl->UseMaterial();
		
		glm::mat3 mvMatrix = glm::mat3( cam->GetViewMatrix()* trans->GetTransform() );
		glm::mat3 normalMatrix = glm::transpose(glm::inverse(mvMatrix));
		glm::vec3 lightPosition( 1.0 );
		
		s->SetUniformMat4( 		"model", 		trans->GetTransform() );
		s->SetUniformMat4( 		"view", 		cam->GetViewMatrix() );
		s->SetUniformMat3( 		"normalMatrix", normalMatrix );
		s->SetUniformMat3( 		"mvMatrix", 	mvMatrix );
		s->SetUniformMat4( 		"projection", 	cam->GetProjectionMatrix() );
		s->SetUniformFloat( 	"time", 		(float)time );
		s->SetUniformFloat3( 	"lightPos", 	lightPosition);
		
		BindTexture( GL_TEXTURE0, GL_TEXTURE_2D, mtl->diffuseTextureId);
		s->SetUniformInt( "colorMap", 0 );
		
		BindTexture( GL_TEXTURE1, GL_TEXTURE_2D, mtl->normalTextureId);
		s->SetUniformInt( "normalMap", 1 );
		
		glDrawElements( GL_TRIANGLES, m->numIndices, GL_UNSIGNED_INT, (void*)0 );
		
		glBindVertexArray( 0 );
		glBindBuffer( GL_ARRAY_BUFFER, 0 );
		
		//TODO(Valentinas): Disable Vertex Attributes after drawing
		
		UnbindTexture();
		glUseProgram(0);
	}
	
} // NAMESPACE END
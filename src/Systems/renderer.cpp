#include "../DataStructure/shader.h"
#include "../Systems/renderer.h"
#include "../Components/cTransform.h"
#include "../Components/cCamera.h"
#include "console.h"
#include <iostream>
#include "../DataStructure/texture.h"

namespace Renderer
{

void Render( glm::uint32 time, Entity* camera, Entity* object )
{
	const ModelInstance* m = Entity::GetComponent<CMeshRenderer>(object)->GetModelInstace();
	if (m == NULL)
	{
		return;
	}
	
	//CTransform* trans = Entity::GetComponent<CTransform>(object);
	CCamera* cam =		Entity::GetComponent<CCamera>(camera);
	const Material* mtl = Entity::GetComponent<CMeshRenderer>(object)->GetMaterial();
	unsigned int program = mtl->shader->program;

	// RENDERER MODE FOR RENDERING POINTS
	//glEnable(GL_PROGRAM_POINT_SIZE);
	
	// RENDERER MODE FOR BLEND_FUNC
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	// RENDERER MODE FOR CULL FACES
	//glEnable(GL_CULL_FACE);
	//glEnable(GL_BACK);
	
	// RENDERER MODE FOR WIREFRAME
	//glPolygonMode(GL_FRONT_AND_BACK, mtl->wireframe_enabled ? GL_LINE : GL_FILL );
	//GLint polygonMode[2];
	//glGetIntegerv( GL_POLYGON_MODE, polygonMode );
	
	// RENDERER MODE FOR DEPTH TESTING
	glEnable(GL_DEPTH_TEST);
	
	glBindVertexArray( m->vao );
	
	mtl->UseMaterial();

	glm::mat3 mvMatrix = glm::mat3( cam->GetViewMatrix() * object->worldTransform );
	glm::mat3 normalMatrix = glm::transpose(glm::inverse(mvMatrix));
	glm::vec3 lightPosition( 0.0, 5.0, 1.0 );

	//GLuint index1 = glGetSubroutineIndex( s->program, GL_FRAGMENT_SHADER, "diffuseOnly" );
	
	//GLuint index2 = glGetSubroutineIndex( s->program, GL_FRAGMENT_SHADER, "phongModel" );
	// shader, number of uniforms, array of subroutine function indexes
	//glUniformSubroutinesuiv( GL_FRAGMENT_SHADER, 1, &index1 );
	//glUniformSubroutinesuiv( GL_FRAGMENT_SHADER, 0, &index2 );
	
	//SetActiveSubroutine( program, *mtl->shader->shaders[1], "ShadeModelType", "phongModel");
	//SetActiveSubroutine( program, *mtl->shader->shaders[1], "myMode", "modeRed");
	//s->SetSubroutineUniform( s->program, GL_FRAGMENT_SHADER, 2, "colorModel");
	
	SetUniform( program,	"model", 		object->worldTransform );
	SetUniform( program,	"view", 		cam->GetViewMatrix() );
	SetUniform( program,	"normalMatrix", normalMatrix );
	SetUniform( program,	"mvMatrix", 	mvMatrix );
	SetUniform( program,	"projection", 	cam->GetProjectionMatrix() );
	SetUniform( program,	"time", 		(float)time );
	SetUniform( program,	"lightPos", 	lightPosition);

	BindTexture( GL_TEXTURE0, GL_TEXTURE_2D, mtl->diffuseTextureId);
	SetUniform( program, "colorMap", 0 );
	
	BindTexture( GL_TEXTURE1, GL_TEXTURE_2D, mtl->normalTextureId);
	SetUniform( program, "normalMap", 1 );

	glDrawElements( GL_TRIANGLES, m->numIndices, GL_UNSIGNED_INT, (void*)0 );
	
	glBindVertexArray( 0 );

	//TODO(Valentinas): Disable Vertex Attributes after drawing?

	UnbindTexture( GL_TEXTURE_2D );
	glUseProgram(0);
}

void RenderLines( glm::uint32 time, unsigned int vao, unsigned int count, ShaderProgram* program, Entity* camera )
{
	CCamera* cam = Entity::GetComponent<CCamera>(camera);
	
	glLineWidth(1.0f);
	glEnable(GL_LINE_SMOOTH);
	glPointSize(5.0f);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL );
	
	glBindVertexArray(vao);
	
	glUseProgram(program->program);
	
	SetUniform( program->program,	"model", 		glm::mat4(1) );
	SetUniform( program->program,	"view", 		cam->GetViewMatrix() );
	SetUniform( program->program,	"projection", 	cam->GetProjectionMatrix() );
	SetUniform( program->program,	"time", 		(float)time );
	
	glDrawArrays( GL_LINES, 0, count);
	glDrawArrays( GL_POINTS, 0, count);
	
	glUseProgram(0);
	
	glBindVertexArray( 0 );
}

void RenderCube( ModelInstance* cube, unsigned int cubeMap, ShaderProgram* program, Entity* camera )
{
	CCamera* cam = Entity::GetComponent<CCamera>(camera);
	
	glDepthMask (GL_FALSE);
	
	glUseProgram(program->program);
	
	SetUniform( program->program,	"view", 		cam->GetViewMatrix() );
	SetUniform( program->program,	"projection", 	cam->GetProjectionMatrix() );
	
	glActiveTexture (GL_TEXTURE0);
	glBindTexture (GL_TEXTURE_CUBE_MAP, cubeMap);
	SetUniform( program->program, "cube_texture", 0 );
	
	glBindVertexArray (cube->vao);
	glDrawArrays (GL_TRIANGLES, 0, cube->numIndices);
	
	glDepthMask (GL_TRUE);
}
	
} // NAMESPACE END
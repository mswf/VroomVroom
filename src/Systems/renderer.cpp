#include <glew.h>
#include "renderer.h"
#include "../console.h"

#include "../Components/entity.h"
#include "../Components/cTransform.h"
#include "../Components/cCamera.h"
#include "../Components/cMeshRenderer.h"

#include "../DataStructure/shader.h"
#include "../DataStructure/texture.h"

#include "../glm/vec2.hpp"
#include "../glm/vec3.hpp"
#include "../glm/mat4x4.hpp"
#include "../glm/gtc/type_ptr.hpp"
#include "../glm/gtc/matrix_transform.hpp"

namespace Renderer
{

	RenderSystem::RenderSystem()
	{
		
	}
	
	RenderSystem::~RenderSystem()
	{
		
	}
	
	bool RenderSystem::Initialize()
	{
		SetMeshRendererList( CMeshRenderer::GetMeshRendererList() );
		return true;
	}
	
	void RenderSystem::Update( void* data )
	{
		
	}
	
	void RenderSystem::SendMessage( void* message )
	{
		
	}
	
	void RenderSystem::RenderPass()
	{
		
	}
	
	/*
	void RenderSystem::ScreenGrab()
	{
		int size = 4 * w_width * w_height;
		GLubyte * pixels = new GLubyte[size];
		glReadBuffer( GL_BACK );
		glReadPixels(0, 0, w_width, w_height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
		
		//SaveImageTo( pixels, window->GetViewportSize().x, window->GetViewportSize().y, 24, 0xFF0000, 0x00FF00, 0x0000FF, FREE_IMAGE_FORMAT::FIF_PNG, "//");
		delete [] pixels;
	}
	*/

	void RenderSystem::Render()
	{
		std::vector< CMeshRenderer* >::const_iterator it = renderables->begin();
		std::vector< CMeshRenderer* >::const_iterator end = renderables->end();
		for ( ; it != end; ++it )
		{
		
			const ModelInstance* m = (*it)->GetModelInstace();
			if (m == NULL)
			{
				return;
			}
			
			const Material* mtl = (*it)->GetMaterial();
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

			glm::mat3 mvMatrix = glm::mat3( camera->GetViewMatrix() * (*it)->entity->transform->GetWorldTransform() );
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
			
			SetUniform( program,	"model", 		(*it)->entity->transform->GetWorldTransform() );
			SetUniform( program,	"view", 		camera->GetViewMatrix() );
			SetUniform( program,	"normalMatrix", normalMatrix );
			SetUniform( program,	"mvMatrix", 	mvMatrix );
			SetUniform( program,	"projection", 	camera->GetProjectionMatrix() );
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
	}

	void RenderSystem::RenderLines( unsigned int vao, unsigned long count )
	{
		glLineWidth(1.0f);
		glEnable(GL_LINE_SMOOTH);
		glPointSize(5.0f);
		
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL );
		
		glBindVertexArray(vao);
		
		glUseProgram(lineProgram->program);
		
		SetUniform( lineProgram->program,	"model", 		glm::mat4(1) );
		SetUniform( lineProgram->program,	"view", 		camera->GetViewMatrix() );
		SetUniform( lineProgram->program,	"projection", 	camera->GetProjectionMatrix() );
		SetUniform( lineProgram->program,	"time", 		(float)time );
		
		glDrawArrays( GL_LINES, 0, (int)count);
		glDrawArrays( GL_POINTS, 0, (int)count);
		
		glUseProgram(0);
		
		glBindVertexArray( 0 );
	}

	void RenderSystem::RenderCube( ModelInstance* cube, unsigned int cubeMap )
	{
		glDepthMask (GL_FALSE);
		
		glUseProgram(skyboxProgram->program);
		
		SetUniform( skyboxProgram->program,	"view", 		camera->GetViewMatrix() );
		SetUniform( skyboxProgram->program,	"projection", 	camera->GetProjectionMatrix() );
		
		glActiveTexture (GL_TEXTURE0);
		glBindTexture (GL_TEXTURE_CUBE_MAP, cubeMap);
		SetUniform( skyboxProgram->program, "cube_texture", 0 );
		
		glBindVertexArray (cube->vao);
		glDrawArrays (GL_TRIANGLES, 0, cube->numIndices);
		
		glDepthMask (GL_TRUE);
	}
	
} // NAMESPACE END
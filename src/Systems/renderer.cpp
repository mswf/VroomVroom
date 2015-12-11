#include <glew.h>
#include "renderer.h"
#include "../console.h"

#include "../Components/entity.h"
#include "../Components/cTransform.h"
#include "../Components/cCamera.h"
#include "../Components/cMeshRenderer.h"
#include "../Components/cDebugRenderer.h"

#include "../DataStructure/shader.h"
#include "../DataStructure/texture.h"
#include "../DataStructure/resource_manager.h"

#include "../glm/vec2.hpp"
#include "../glm/vec3.hpp"
#include "../glm/mat4x4.hpp"
#include "../glm/gtc/type_ptr.hpp"
#include "../glm/gtc/matrix_transform.hpp"

namespace Renderer
{

	RenderSystem::RenderSystem() :
		time(1),
		w_width(0),
		w_height(0),
		framebufferScaleX(1.0f),
		framebufferScaleY(1.0f),
		camera(NULL),
		debugProgram(NULL),
		skybox(NULL),
		skyboxMap(0),
		skyboxProgram(NULL)
	{
	}
	
	RenderSystem::~RenderSystem()
	{
		delete backgroundColour;
	}
	
	bool RenderSystem::Initialize()
	{
		SetMeshRendererList( CMeshRenderer::GetMeshRendererList() );
		SetDebugRendererList( CDebugRenderer::GetDebugRendererList() );
		ResourceManager& rm = ResourceManager::getInstance();
		skybox = rm.GetModel("__Skybox_model");
		skyboxProgram = rm.GetShaderProgram("__Skybox_program");
		debugProgram = rm.GetShaderProgram("__Debug_program");
		backgroundColour = new float[4];
		SetBackgroundColor(0.91f, 0.91f, 0.91f, 1.0f);
		return true;
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

	void RenderSystem::SetViewportRect()
	{
		glm::vec4 rect = camera->GetViewportRectangle();
		float w = w_width * framebufferScaleX;
		float h = w_height * framebufferScaleY;
		glViewport( w * rect.x, h * rect.y, w * rect.z, h * rect.w );
	}
	
	void RenderSystem::Render()
	{
		glClearColor( backgroundColour[0], backgroundColour[1], backgroundColour[2], backgroundColour[3] );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		
		if (camera == NULL) return;
		SetViewportRect();
		RenderEnvironment();
		RenderDebugLines();
		RenderScene();
	}

	void RenderSystem::RenderScene()
	{
		// RENDERER MODE FOR BLEND_FUNC
		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		glEnable(GL_DEPTH_TEST);
		
		camera->Call();
		
		std::vector< CMeshRenderer* >::const_iterator it = renderables->begin();
		std::vector< CMeshRenderer* >::const_iterator end = renderables->end();
		for ( ; it != end; ++it )
		{
			
			const ModelInstance* m = (*it)->GetModelInstace();
			if (m == NULL)
			{
				continue;
			}
			
			const Material* mtl = (*it)->GetMaterial();
			unsigned int program = mtl->shader->program;
			
			glBindVertexArray( m->vao );
			
			mtl->UseMaterial();
			mtl->SetUniforms();
			if ( mtl->two_sided )
			{
				glEnable(GL_CULL_FACE);
			}
			
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
			
			if ( mtl->wireframe_enabled )
			{
				glCullFace( GL_FRONT );
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE );
				glDrawElements( GL_TRIANGLES, m->numIndices, GL_UNSIGNED_INT, (void*)0 );
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL );
			}
		
			glCullFace( GL_BACK );
			glDrawElements( GL_TRIANGLES, m->numIndices, GL_UNSIGNED_INT, (void*)0 );
			
		}
		
		glDisable(GL_CULL_FACE);
		
		glBindVertexArray( 0 );
		
		UnbindTexture( GL_TEXTURE_2D );
		glUseProgram(0);
	}
	
	void RenderSystem::RenderDebugLines()
	{
		// Is there anything to render?
		if ( debugPrimitives->size() == 0 ) return;
		
		glUseProgram(debugProgram->program);
		
		std::vector< CDebugRenderer* >::const_iterator it = debugPrimitives->begin();
		std::vector< CDebugRenderer* >::const_iterator end = debugPrimitives->end();
		for ( ; it != end; ++it )
		{
			if ( !(*it)->ContainsPrimitives() )
			{
				continue;
			}
			
			// Assign render primitive
			GLenum primitive;
			if ( (*it)->GetDrawMode() == DrawMode::TRIANGLES )
			{
				primitive = GL_TRIANGLES;
			}
			else
			{
				primitive = GL_LINES;
				glEnable(GL_LINE_SMOOTH);
			}
			
			(*it)->UpdateBuffer();
						
			glBindVertexArray( (*it)->vao);
		
			SetUniform( debugProgram->program,	"model", 		(*it)->entity->transform->GetWorldTransform() );
			SetUniform( debugProgram->program,	"view", 		camera->GetViewMatrix() );
			SetUniform( debugProgram->program,	"projection", 	camera->GetProjectionMatrix() );
			SetUniform( debugProgram->program,	"time", 		(float)time );
		
			glDrawArrays( primitive, 0, (*it)->GetDrawCount() );
			if ( (*it)->IsDrawingPoints() )
			{
				glEnable(GL_PROGRAM_POINT_SIZE);
				SetUniform( debugProgram->program, "pointSize", (*it)->GetPointSize() );
				glDrawArrays(GL_POINTS, 0, (*it)->GetDrawCount());
			}
		}
		glUseProgram(0);
		glDisable(GL_LINE_SMOOTH);
		glBindVertexArray( 0 );
	}

	void RenderSystem::RenderEnvironment()
	{
		if (skyboxMap == 0) return;
		glDepthMask (GL_FALSE);
		
		glUseProgram(skyboxProgram->program);
		
		SetUniform( skyboxProgram->program,	"view", 		camera->GetViewMatrix() );
		SetUniform( skyboxProgram->program,	"projection", 	camera->GetProjectionMatrix() );
		
		glActiveTexture (GL_TEXTURE0);
		glBindTexture (GL_TEXTURE_CUBE_MAP, skyboxMap);
		SetUniform( skyboxProgram->program, "cube_texture", 0 );
		
		glBindVertexArray (skybox->vao);
		glDrawArrays (GL_TRIANGLES, 0, skybox->numIndices);
		
		glDepthMask (GL_TRUE);
	}
	
	void RenderSystem::SetWindowSize( const int& w, const int& h )
	{
		w_width = w;
		w_height = h;
		GLint dims[4] = {0};
		glGetIntegerv(GL_VIEWPORT, dims);
		GLint fbWidth = dims[2];
		GLint fbHeight = dims[3];
		SetFramebufferScale( (float)fbWidth/w_width, (float)fbHeight/w_height );
	}
	
	void RenderSystem::SetFramebufferScale( const float& scaleX, const float& scaleY )
	{
		framebufferScaleX = scaleX;
		framebufferScaleY = scaleY;
	}
	
	void RenderSystem::SetBackgroundColor( float r, float g, float b, float a )
	{
		backgroundColour[0] = r;
		backgroundColour[1] = g;
		backgroundColour[2] = b;
		backgroundColour[3] = a;
	}
	
} // NAMESPACE END
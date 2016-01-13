#include <glew.h>
#include "renderer.h"
#include "../console.h"

#include "../Components/entity.h"
#include "../Components/cTransform.h"
#include "../Components/cCamera.h"
#include "../Components/cMeshRenderer.h"
#include "../Components/cDebugRenderer.h"
#include "../Components/cLight.h"

#include "../DataStructure/shader.h"
#include "../DataStructure/texture.h"
#include "../DataStructure/resource_manager.h"

#include "../glm/vec2.hpp"
#include "../glm/vec3.hpp"
#include "../glm/mat4x4.hpp"
#include "../glm/gtc/type_ptr.hpp"
#include "../glm/gtc/matrix_transform.hpp"

#include "../Utilities/helperFunctions.h"


Renderer::Renderer() :
	skyboxMap(0),
	time(1),
	w_width(0),
	w_height(0),
	framebufferScaleX(1.0f),
	framebufferScaleY(1.0f), 
	backgroundColour(NULL),
	camera(NULL), 
	renderables(NULL),
	lights(NULL),
	debugPrimitives(NULL),
	debugProgram(NULL),
	skybox(NULL),
	skyboxProgram(NULL)
{
}
	
Renderer::~Renderer()
{
	delete[] backgroundColour;
}
	
bool Renderer::Initialize()
{
	SetMeshRendererList( CMeshRenderer::GetMeshRendererList() );
	SetDebugRendererList( CDebugRenderer::GetDebugRendererList() );
	SetLightList( CLight::GetLightList() );
	ResourceManager& rm = Assets;
	skybox = rm.GetModel("__Skybox_model");
	skyboxProgram = rm.GetShaderProgram("__Skybox_program");
	debugProgram = rm.GetShaderProgram("__Debug_program");
	backgroundColour = new float[4];
	SetBackgroundColor(0.91f, 0.91f, 0.91f, 1.0f);
	return true;
}
	
void Renderer::ScreenGrab()
{
	int components = 3;
	int width = w_width * framebufferScaleX;
	int height = w_height * framebufferScaleY;
		
	uint32 size = components * width * height;
	uint8* pixels = new uint8[size];
	glReadBuffer( GL_BACK );
	glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
		
	int row, col, z;
	uint8 temp_pixels;
		
	// TODO(Valentinas): use a bigger temp buffer and memcpy multiple pixels at once
	for (row = 0; row < (height/2); row++)
	{
		for (col = 0; col < width; col++)
		{
			for (z = 0; z < components; z++)
			{
				temp_pixels = pixels[ (row * width + col) * components + z ];
				pixels[(row * width + col) * components + z] = pixels[((height - row - 1) * width + col) * components + z];
				pixels[((height - row - 1) * width + col) * components + z] = temp_pixels;
			}
		}
	}
	string filename( "Screenshot_" + HelperFunctions::GetTimeString() + ".png" );
	HelperFunctions::WritePixels( filename.c_str(), ImageFileFormat::PNG, pixels, width, height, components );
	delete[] pixels;
	Terminal.LogRender("Screenshot taken: " + filename, true);
}
	
void Renderer::Render()
{
	ClearFlag();
	if (camera == NULL) return;
	RenderEnvironment();
	RenderScene();
	RenderDebugLines();
}

void Renderer::SetViewportRect()
{
	if (camera == NULL) return;
	glm::vec4 rect = camera->GetViewportRectangle();
	float w = w_width * framebufferScaleX;
	float h = w_height * framebufferScaleY;
	glViewport( w * rect.x, h * rect.y, w * rect.z, h * rect.w );
}
	
void Renderer::ClearFlag()
{
	glClearColor( backgroundColour[0], backgroundColour[1], backgroundColour[2], backgroundColour[3] );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}
	
void Renderer::RenderScene()
{
	// RENDERER MODE FOR BLEND_FUNC
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
	glEnable(GL_DEPTH_TEST);
		
	camera->Call();
		
	glm::vec3 lightPosition(0);
	glm::vec3 lightDirection(0.5,0.5,1.0);
	float lightIntensity = 1.0;
	glm::vec4 lightColour(1);
		
	if (lights->size() > 0)
	{
		lightDirection = lights->at(0)->GetDirection();
		lightIntensity = lights->at(0)->GetIntensity();
		lightColour = lights->at(0)->GetColour();
	}
		
	std::vector< CMeshRenderer* >::const_iterator it = renderables->begin();
	std::vector< CMeshRenderer* >::const_iterator end = renderables->end();
	for ( ; it != end; ++it )
	{
			
		const ModelInstance* model = (*it)->GetModelInstace();
		if (model == NULL || (*it)->entity == NULL)
		{
			continue;
		}
			
		const Material* material = (*it)->GetMaterial();
		unsigned int program = material->shader->program;
			
		glBindVertexArray( model->vao );
			
		material->UseMaterial();
		material->SetUniforms();
		if ( material->two_sided )
		{
			glEnable(GL_CULL_FACE);
		}
			
		glm::mat3 mvMatrix = glm::mat3( camera->GetViewMatrix() * (*it)->entity->transform->GetWorldTransform() );
		glm::mat3 normalMatrix = glm::transpose(glm::inverse(mvMatrix));
			
			
		SetUniform( program,	"model", 		(*it)->entity->transform->GetWorldTransform() );
		SetUniform( program,	"view", 		camera->GetViewMatrix() );
		SetUniform( program,	"normalMatrix", normalMatrix );
		SetUniform( program,	"mvMatrix", 	mvMatrix );
		SetUniform( program,	"projection", 	camera->GetProjectionMatrix() );
		SetUniform( program,	"time", 		(float)time );
		SetUniform( program,	"lightPos", 	lightPosition);
		SetUniform( program, 	"diffuseColor", material->GetDiffuseColor() );
			
		SetUniform( program, "lightIntensity", lightIntensity);
		SetUniform( program, "lightColour",		lightColour);
		SetUniform( program, "lightDirection", lightDirection);
			
			
		int offset = 0;
			
		BindTexture( GL_TEXTURE0 + offset, GL_TEXTURE_2D, material->diffuseTextureId);
		SetUniform(program, "colorMap", offset++);
			
		BindTexture( GL_TEXTURE0 + offset, GL_TEXTURE_2D, material->normalTextureId);
		SetUniform(program, "normalMap", offset++);
			
		BindTexture( GL_TEXTURE0 + offset, GL_TEXTURE_CUBE_MAP, skyboxMap );
		SetUniform(program, "cubeMap", offset++);
			
		if ( material->wireframe_enabled )
		{
			glCullFace( GL_FRONT );
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE );
			glDrawElements( GL_TRIANGLES, model->numIndices, GL_UNSIGNED_INT, 0 );
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL );
		}
		
		glCullFace( GL_BACK );
		glDrawElements( GL_TRIANGLES, model->numIndices, GL_UNSIGNED_INT, 0 );
	}
	
	glDisable(GL_CULL_FACE);
		
	glBindVertexArray( 0 );
		
	UnbindTexture( GL_TEXTURE_2D );
	glUseProgram(0);
}
	
void Renderer::RenderDebugLines()
{
	// Is there anything to render?
	if ( debugPrimitives->size() == 0 ) return;
		
	glUseProgram(debugProgram->program);
		
	glDisable(GL_DEPTH_TEST);
		
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
	glEnable(GL_DEPTH_TEST);
	glUseProgram(0);
	glDisable(GL_LINE_SMOOTH);
	glBindVertexArray( 0 );
}

void Renderer::RenderEnvironment()
{
	if (skyboxMap == 0) return;
	glDepthMask (GL_FALSE);
		
	glUseProgram(skyboxProgram->program);
	
	glm::mat4 modelMatrix = glm::inverse( camera->GetViewMatrix() ) * glm::mat4_cast(camera->entity->transform->GetRotation());
	// rotate 
	modelMatrix = glm::rotate(modelMatrix, glm::radians(90.0f), glm::vec3(1, 0, 0));

	SetUniform( skyboxProgram->program, "model", modelMatrix );
	SetUniform( skyboxProgram->program,	"view", 		camera->GetViewMatrix() );
	SetUniform( skyboxProgram->program,	"projection", 	camera->GetProjectionMatrix() );
		
	glActiveTexture (GL_TEXTURE0);
	glBindTexture (GL_TEXTURE_CUBE_MAP, skyboxMap);
	SetUniform( skyboxProgram->program, "cube_texture", 0 );
		
	glBindVertexArray (skybox->vao);
	glDrawArrays (GL_TRIANGLES, 0, skybox->numIndices);
		
	glDepthMask (GL_TRUE);
}
	
void Renderer::SetWindowSize( const int& w, const int& h )
{
	w_width = w;
	w_height = h;
	GLint dims[4] = {0};
	glGetIntegerv(GL_VIEWPORT, dims);
	GLint fbWidth = dims[2];
	GLint fbHeight = dims[3];
	// TODO: Valentinas, what was this trying to do?
	// SetFramebufferScale( (float)fbWidth/w_width, (float)fbHeight/w_height );
	SetViewportRect();
}
	
void Renderer::SetFramebufferScale( const float& scaleX, const float& scaleY )
{
	framebufferScaleX = scaleX;
	framebufferScaleY = scaleY;
}
	
void Renderer::SetBackgroundColor( float r, float g, float b, float a )
{
	backgroundColour[0] = r;
	backgroundColour[1] = g;
	backgroundColour[2] = b;
	backgroundColour[3] = a;
}
void Renderer::SetCamera( CCamera* c )
{
	if ( camera != NULL )
	{
		camera->SetRenderer(NULL);
	}
	camera = c;

	if (c != NULL )
	{
		c->SetRenderer(this);
	}
}
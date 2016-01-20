#ifndef renderer_h
#define renderer_h

#include <vector>
#include "ComponentSystem.h"
#include "../Utilities/typedef.h"

struct ModelInstance;
struct ShaderProgram;
class Entity;
class CMeshRenderer;
class CDebugRenderer;
class CLight;
class CCamera;


enum class ClearFlag
{
	NONE,
	SKYBOX,
	COLOR_DEPTH,
	COLOR,
	DEPTH
};
	
class Renderer : public ComponentSystem
{
    public:
        
        Renderer();
        ~Renderer();
		
        bool Initialize() override;

		void Render();
		
		void SetEnvironmentMap( unsigned int map );
		void SetWindowSize( const int& w, const int& h );
		void SetFramebufferScale( const float& scaleX, const float& scaleY );
		void SetBackgroundColor( float r, float g, float b, float a ) const;
		void SetSkybox( const char* name );
		void ScreenGrab() const;
		inline void SetMeshRendererList( std::vector< CMeshRenderer* >* list ) { renderables = list; }
		inline void SetDebugRendererList( std::vector< CDebugRenderer* >* list ) { debugPrimitives = list; }
		inline void SetLightList( std::vector< CLight* >* list ) { lights = list; }
		void SetCamera( CCamera* c );
		inline void SetTime( const uint32& t ) { time = t; }
	
		inline const int& GetWindowWidth() const { return w_width; }
		inline const int& GetWindowHeight() const { return w_height; }
		inline CCamera* GetCamera() const { return camera; }
		unsigned int skyboxMap;
		
	private:
	
		void InitializeShaders();
	
		void ClearFlag() const;
		void SetViewportRect() const;
		void RenderEnvironment() const;
		void RenderScene() const;
		void RenderDebugLines() const;
		
		uint32 time;
		int w_width;
		int w_height;
		float framebufferScaleX;
		float framebufferScaleY;
		float* backgroundColour;
		CCamera* camera;
		
		// Scene
		std::vector< CMeshRenderer*>* renderables;
		
		// Light
		std::vector< CLight* >* lights;
		
		// Line
		std::vector< CDebugRenderer*>* debugPrimitives;
		ShaderProgram* debugProgram;
		
		// Environment
		ModelInstance* skybox;
		ShaderProgram* skyboxProgram;
		
};
    
	
/*
	glViewport( 0, 0, (int)ImGui::GetIO().DisplaySize.x * ImGui::GetIO().DisplayFramebufferScale.x, (int)ImGui::GetIO().DisplaySize.y * ImGui::GetIO().DisplayFramebufferScale.y );
	 
	// RENDERER MODE FOR WIREFRAME
	//glPolygonMode(GL_FRONT_AND_BACK, mtl->wireframe_enabled ? GL_LINE : GL_FILL );
	//GLint polygonMode[2];
	//glGetIntegerv( GL_POLYGON_MODE, polygonMode );

	*/
	

#endif /* renderer_h */

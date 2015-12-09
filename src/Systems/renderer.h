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
class CCamera;

namespace Renderer
{
	class RenderSystem : public ComponentSystem
    {
        public:
        
            RenderSystem();
            ~RenderSystem();
		
            bool Initialize() override;

			void Render();
			void RenderCube( ModelInstance* cube, unsigned int cubeMap );
			void RenderLines( unsigned int vao, unsigned long count );
		
			void SetEnvironmentMap( unsigned int map );
			void SetWindowSize( const int& w, const int& h );
			void SetFramebufferScale( const float& scaleX, const float& scaleY );
			void SetMeshRendererList( std::vector< CMeshRenderer* >* list );
			void SetDebugRendererList( std::vector< CDebugRenderer* >* list );
			void SetCamera( CCamera* c );
			void SetTime( uint32 t );
			void SetPointDrawing( bool enabled );
		
			const int& GetWindowWidth( );
			const int& GetWindowHeight( );
			CCamera* GetCamera( );
		private:
		
			void SetViewportRect();
			void RenderEnvironment();
			void RenderScene();
			void RenderDebugLines();
		
			uint32 time;
			int w_width;
			int w_height;
			float framebufferScaleX;
			float framebufferScaleY;
			CCamera* camera;
		
			// Scene
			std::vector< CMeshRenderer*>* renderables;
		
			// Line
			std::vector< CDebugRenderer*>* debugPrimitives;
			ShaderProgram* debugProgram;
			bool drawPoints;
		
			// Environment
			ModelInstance* skybox;
			unsigned int cubeMap;
			ShaderProgram* skyboxProgram;
    };
    
	
	/*
	 glViewport( 0, 0, (int)ImGui::GetIO().DisplaySize.x * ImGui::GetIO().DisplayFramebufferScale.x, (int)ImGui::GetIO().DisplaySize.y * ImGui::GetIO().DisplayFramebufferScale.y );
	 */
	
}

#endif /* renderer_h */

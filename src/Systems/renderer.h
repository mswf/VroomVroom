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
			void SetMeshRendererList( std::vector< CMeshRenderer* >* list );
			void SetLineRendererList( std::vector< CDebugRenderer* >* list );
			void SetCamera( CCamera* c );
			void SetTime( uint32 t );
			void SetPointDrawing( bool enabled );
		
		private:
		
			void RenderEnvironment();
			void RenderScene();
			void RenderDebugLines();
		
			uint32 time;
			int w_width;
			int w_height;
			CCamera* camera;
		
			// Scene
			std::vector< CMeshRenderer* >* renderables;
		
			// Line
			std::vector< CDebugRenderer* >* lines;
			ShaderProgram* lineProgram;
			bool drawPoints;
		
			// Environment
			ModelInstance* skybox;
			unsigned int cubeMap;
			ShaderProgram* skyboxProgram;
    };
    
	
	
	
}

#endif /* renderer_h */

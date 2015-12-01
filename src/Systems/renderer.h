#ifndef renderer_h
#define renderer_h

#include <vector>
#include "ComponentSystem.h"
#include "../Utilities/typedef.h"

struct ModelInstance;
struct ShaderProgram;
class Entity;
class CMeshRenderer;
class CCamera;

namespace Renderer
{
	class RenderSystem : public ComponentSystem
    {
        public:
        
            RenderSystem();
            ~RenderSystem();
		
			void RenderPass();
		
            bool Initialize() override;
            void Update(void* data) override;
            void SendMessage(void* message) override;

			void Render();
			void RenderCube( ModelInstance* cube, unsigned int cubeMap, ShaderProgram* program );
			void RenderLines( unsigned int vao, unsigned int count, ShaderProgram* program );
		
			inline void SetMeshRendererList( std::vector< CMeshRenderer* >* list ) { renderables = list; }
			inline void SetCamera( CCamera* c ) { camera = c; }
			inline void SetTime( uint32 t ) { time = t; }
		
		private:
		
			uint32 time;
			CCamera* camera;
			std::vector< CMeshRenderer* >* renderables;
    };
    
	
	
	
}

#endif /* renderer_h */

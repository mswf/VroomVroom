#ifndef renderer_h
#define renderer_h

#include "ComponentSystem.h"
#include <glew.h>

#include "../glm/vec2.hpp"
#include "../glm/vec3.hpp"
#include "../glm/mat4x4.hpp"
#include "../glm/gtc/type_ptr.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../Components/entity.h"
#include "../Components/cMeshRenderer.h"

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
        
        private:
        
    };
    
	void Render( glm::uint32 time, Entity* camera, Entity* mesh );
}

#endif /* renderer_h */

#ifndef renderer_h
#define renderer_h

#include "ComponentSystem.h"

#include <glew.h>
#include <strstream>
#include <fstream>	// fin,


#include "../glm/vec2.hpp"
#include "../glm/vec3.hpp"
#include "../glm/mat4x4.hpp"
#include "../glm/gtc/type_ptr.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../Components/cMesh.h"
#include "../Components/entity.h"
#include "../Components/cMaterial.h"

namespace Renderer
{
	
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 texcoord;
	};

	
    class RenderSystem : public ComponentSystem
    {
        public:
        
            RenderSystem();
            ~RenderSystem();
        
            bool Initialize() override;
            void Update(void* data) override;
            void SendMessage(void* message) override;
        
        private:
        
    };
    
	void Render( glm::uint32 time, Entity* mesh, Entity* camera );
	void GenerateTriangle( Entity* e );
	void GenerateCube( Entity* e, bool centered = true );
	void GenerateBuffers( Entity* e, const Vertex* verts, GLuint verticeCount, GLuint faceCount, const GLubyte* indices );
	void ClearBuffers( CMesh* m );
	void LoadTexture();
	GLuint CreateTexture( unsigned char*, int, int );
	void CreateFBO();

}

#endif /* renderer_h */

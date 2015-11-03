#ifndef renderer_h
#define renderer_h

#include "ComponentSystem.h"

#include <glew.h>
#include <iostream>	// cout, cerr, endl
#include <fstream>	// fin,


#include "../glm/vec2.hpp"
#include "../glm/vec3.hpp"
#include "../glm/mat4x4.hpp"
#include "../glm/gtc/type_ptr.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../Components/cTransform.h"
#include "../Components/cCamera.h"
#include "../Components/cMesh.h"

namespace Renderer
{
    
    class Shader;
	
	struct RenderData
	{
		GLuint framebuffer;
		Shader* shader;
		GLuint arraybuffer;
		GLuint elementbuffer;
		GLuint vertexbuffer;
        CTransform transform;
		CCamera* camera;
	};
	
	struct Vertex
	{
		glm::vec3 position;
		glm::vec4 texcoord;
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
    
	void Render( glm::uint32 time, RenderData* data, CMesh* mesh );

	void GetRenderData( RenderData* , CMesh* mesh );
	void DeleteData( RenderData* );
	
	
	void GenerateCube( CMesh* mesh );
	void LoadModel( CMesh* mesh );
	void LoadTexture();
	GLuint CreateTexture( unsigned char*, int, int );
	void CreateFBO();

}

#endif /* renderer_h */

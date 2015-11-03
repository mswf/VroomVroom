#include "shader.h"
#include "renderer.h"
#include "component.h"
#include "cTransform.h"




namespace Renderer
{
	glm::ivec2 windowSize;
    GLuint program;
	GLuint framebuffer_object;
	GLuint framebuffer_depth;
	GLuint framebuffer_color;
	GLuint elementArrayBuffer;
	GLuint vertexArray_object;
	GLuint arrayBuffer;
	
    RenderSystem::RenderSystem()
    {
        
    }
    
    RenderSystem::~RenderSystem()
    {
        
    }
    
    bool RenderSystem::Initialize()
    {
        return true;
    }
    
    void RenderSystem::Update( void* data )
    {
        
    }
    
    void RenderSystem::SendMessage(void* message)
    {
        
    }
    
	void GetRenderData( RenderData* outRenderData )
	{
		LoadModel();
		CreateFBO();
		
		//LoadTexture();
		outRenderData->framebuffer = framebuffer_object;
		outRenderData->arraybuffer = arrayBuffer;
		outRenderData->elementbuffer = elementArrayBuffer;
		outRenderData->vertexbuffer = vertexArray_object;
		outRenderData->shader = new Shader(vertexArray_object);
		outRenderData->transform = new CTransform();
	}
	
	void GetCamera(Camera* cam, Projection type, float fov, float aspectRatio, float near, float far)
	{
		glm::mat4 projection;
		switch (type)
		{
			case Projection::ORTHOGONAL:
			{
				//projection = glm::ortho(T left, T right, T bottom, T top, near, far);
				break;
			}
			case Projection::PERSPECTIVE:
			{
				projection = glm::perspective(glm::radians(fov), aspectRatio, near, far);
				break;
			}
			default:
			{
				assert(false);
				break;
			}
		}
		cam->projection = projection;
		cam->view = glm::lookAt(cam->eye, cam->center, glm::vec3( 0.0, 1.0, 0.0 ));
	}
	
	void DeleteData( RenderData* data )
	{
		delete data->shader;
		delete data;
	}
	
	void DeleteCamera( Camera* cam )
	{
		delete cam;
	}
	
	void Render( glm::uint32 time, Camera* cam, RenderData* data )
	{
//        std::vector<Entity *> renderables;
//        Entity::entitySystem->getEntities<CTransform>(renderables);
        
		program = data->shader->program;
		
		//glBindFramebuffer( GL_FRAMEBUFFER, data->framebuffer );
		
		glClearColor( 0.2, 0.2, 0.2, 1.0 );
		glClear(GL_COLOR_BUFFER_BIT); //GL_DEPTH_BUFFER_BIT
		
		
		glEnable( GL_CULL_FACE );
		glCullFace( GL_BACK );
		
		glUseProgram(program);
		data->shader->SetUniformMat4( "model", data->transform.transform );
		data->shader->SetUniformMat4( "view", cam->view );
		data->shader->SetUniformMat4( "projection", cam->projection );
		data->shader->SetUniformFloat( "time", (float)time );

		Draw(data);
		
		glUseProgram(0);
	}

	void Draw(RenderData * data)
	{
		
		glBindBuffer( GL_ARRAY_BUFFER, data->arraybuffer );
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER,  data->elementbuffer );
		glBindVertexArray( data->vertexbuffer );
		
		GLuint position = GetAttribute("position");
		GLuint texcoord = GetAttribute("texcoord");
		
		glEnableVertexAttribArray(position);
		glEnableVertexAttribArray(texcoord);
		
		glVertexAttribPointer( position, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0 );
		glVertexAttribPointer( texcoord, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) sizeof(glm::vec3) );
		
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, 0);
		
		glBindVertexArray( 0 );
		glBindBuffer( GL_ARRAY_BUFFER, 0 );
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
		
	}

	GLuint GetAttribute( const char * attribute )
	{
		return glGetAttribLocation( program, attribute );
	}

	// Create a Frame buffer object
	void CreateFBO()
	{
		glGenFramebuffers( 1, &framebuffer_object );
		glBindFramebuffer( GL_FRAMEBUFFER, framebuffer_object );
		
		//glGenTextures( 1, &framebuffer_depth );
		//glBindTexture( GL_TEXTURE_2D, framebuffer_depth );
		//glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
		//glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
		//glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, windowSize.x, windowSize.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL );
		
		
		glGenTextures( 1, &framebuffer_color );
		glBindTexture( GL_TEXTURE_2D, framebuffer_color );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA32F, windowSize.x, windowSize.y, 0, GL_RGBA, GL_FLOAT, NULL );
		glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebuffer_color, 0 );
		
		
		glBindTexture( GL_TEXTURE_2D, 0);
		glBindFramebuffer( GL_FRAMEBUFFER, 0);
		
	}

	void ResizeFBO( int x, int y )
	{
		if ( windowSize != glm::ivec2(x,y) )
		{
			windowSize = glm::ivec2(x,y);
			
			glBindTexture( GL_TEXTURE_2D, framebuffer_depth );
			glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, x, y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL );
			
			glBindTexture( GL_TEXTURE_2D, framebuffer_color );
			glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA32F, x, y, 0, GL_RGBA, GL_FLOAT, NULL );
			
			glBindTexture( GL_TEXTURE_2D, 0 );
		}
	}
		
	// Import models and textures

	void LoadModel()
	{
		Vertex mesh[8];
		for (int i = 0; i < 8; i++)
		{
			float x = ( (i & 1) == 0 ? 0 : 1 );
			float y = ( (i & 2) == 0 ? 0 : 1 );
			float z = ( (i & 4) == 0 ? 0 : 1 );
			
			Vertex vert = { glm::vec3( x, y, z ) - 0.5f, glm::vec4( x, y, z, 1.0f ) };
			mesh[i] = vert;
		}
		
		GLubyte indices[36] =
		{
			0,2,3, 0,3,1, // Bottom
			
			1,3,7, 1,7,5, // Right
			
			5,7,6, 5,6,4, // Top
			
			4,6,2, 4,2,0, // Left
			
			2,6,7, 2,7,3,  // Back
			
			1,5,4, 1,4,0  // Front
			
		};
		
		//VAO
		glGenVertexArrays( 1, &vertexArray_object );
		glBindVertexArray( vertexArray_object );
		//VBO
		glGenBuffers( 1, &arrayBuffer );
		glBindBuffer( GL_ARRAY_BUFFER, arrayBuffer );
		glBufferData( GL_ARRAY_BUFFER, 8 * sizeof(Vertex), mesh, GL_STATIC_DRAW);
		//EBO
		glGenBuffers( 1, &elementArrayBuffer);
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, elementArrayBuffer );
		glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		
		glBindVertexArray( 0 );
		glBindBuffer( GL_ARRAY_BUFFER, 0 );
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
		
	}

	void LoadTexture(const char * file)
	{
		std::cout << "Loading texture" << std::endl;
		//CreateTexture(0, 0, 0);
	}

	// Create OpenGL texture
	GLuint CreateTexture(unsigned char* pixels, int width, int height)
	{
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
		
		// TODO(Valentinas): Delete texture only when finishing work
		//glDeleteTextures(1, &texture);
		
		return texture;
	}

} // NAMESPACE END
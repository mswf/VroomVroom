#include "renderer.h"

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
	
	glm::mat4 translation;
	glm::mat4 rotation;
	glm::mat4 scale;
	
	void GetRenderData( RenderData* outRenderData )
	{
		LoadModel();
		LoadShader();
		CreateFBO();
		
		//LoadTexture();
		outRenderData->framebuffer = framebuffer_object;
		outRenderData->arraybuffer = arrayBuffer;
		outRenderData->elementbuffer = elementArrayBuffer;
		outRenderData->vertexbuffer = vertexArray_object;
		outRenderData->program = program;
	}
	
	void GetCamera(Camera* cam, Projection type, float fov, float aspectRatio, float near, float far)
	{
		glm::mat4 projection;
		switch (type)
		{
			case ORTHOGONAL:
			{
				//projection = glm::ortho(T left, T right, T bottom, T top, near, far);
				break;
			}
			case PERSPECTIVE:
			{
				projection = glm::perspective(glm::radians(fov), aspectRatio, near, far);
				break;
			}
			default:
			{
				break;
			}
		}
		cam->projection = projection;
		cam->view = glm::lookAt(cam->eye, cam->center, glm::vec3( 0.0, 1.0, 0.0 ));
	}
	
	void DeleteData( RenderData* data )
	{
		delete data;
	}
	
	void DeleteCamera( Camera* cam )
	{
		delete cam;
	}
	
	void RenderObject(unsigned int time, RenderData * data, Camera* cam)
	{
		program = data->program;
		
		//glBindFramebuffer( GL_FRAMEBUFFER, data->framebuffer );
		
		glClearColor( 0.2, 0.2, 0.2, 1.0 );
		glClear(GL_COLOR_BUFFER_BIT); //GL_DEPTH_BUFFER_BIT
		
		
		//glEnable( GL_CULL_FACE );
		//glCullFace( GL_BACK );
		
		glm::mat4 model;
		glUseProgram(program);
		SetUniformMat4( "model", model );
		SetUniformMat4( "view", cam->view );
		SetUniformMat4( "projection", cam->projection );
		SetUniformFloat( "time", (float)time );

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

		
	void SetUniformInt( const char * uniform, int value )
	{
		glUniform1i( glGetUniformLocation ( program, uniform ), value );
	}

	void SetUniformFloat( const char * uniform, float value )
	{
		glUniform1f( glGetUniformLocation( program, uniform ), value );
	}

	void SetUniformFloat2(const char * uniform, glm::vec2 value)
	{
		glUniform2f( glGetUniformLocation( program, uniform ), value.x, value.y );
	}

	void SetUniformFloat3(const char * uniform, glm::vec3 value)
	{
		glUniform3f( glGetUniformLocation( program, uniform ), value.x, value.y, value.z);
	}

	void SetUniformMat4( const char * uniform, glm::mat4 value )
	{
		glUniformMatrix4fv( glGetUniformLocation( program, uniform ), 1, GL_FALSE, glm::value_ptr(value) );
		
	}

	// Load and create a shader program

	void LoadShader()
	{
		glBindVertexArray( vertexArray_object );
		const char* src_vertex = ReadFile("vs.txt");
		const char* src_fragment = ReadFile("fs.txt");
		program = glCreateProgram();
		GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource( vertex_shader, 1, &src_vertex, NULL);
		glShaderSource( fragment_shader, 1, &src_fragment, NULL);
		CompileShader(vertex_shader);
		CompileShader(fragment_shader);
		glAttachShader( program, vertex_shader );
		glAttachShader( program, fragment_shader );
		glBindFragDataLocation(program, 0, "outColor");
		LinkShader();
		ValidateShader();
	}

	void CompileShader(GLuint shader)
	{
		glCompileShader( shader );
		GLint status;
		glGetShaderiv( shader, GL_COMPILE_STATUS, &status );
		
		if ( status == GL_FALSE )
		{
			std::cout << "Shader compilation failed" << std::endl;
			GLchar message[255];
			glGetShaderInfoLog( shader, sizeof(message), 0, &message[0] );
			std::cout << message << std::endl;
			assert(false);
		}
	}

	void LinkShader()
	{
		glLinkProgram( program );
		GLint linkStatus;
		glGetProgramiv( program, GL_LINK_STATUS, &linkStatus );
		
		if ( linkStatus == GL_FALSE )
		{
			std::cout << "Shader linking failed" << std::endl;
			GLchar message[255];
			glGetProgramInfoLog( program, sizeof(message), 0, &message[0]);
			std::cout << message << std::endl;
			assert(false);
		}
		
	}

	void ValidateShader()
	{
		GLint validateStatus;
		glValidateProgram( program );
		glGetProgramiv( program, GL_VALIDATE_STATUS, &validateStatus);
		if (validateStatus == GL_FALSE)
		{
			std::cout << "Shader validation failed" << std::endl;
			GLchar message[255];
			glGetProgramInfoLog( program, sizeof(message), 0, &message[0]);
			std::cout << message << std::endl;
			//assert(false);
		}
	}

	void DeleteShaderProgram()
	{
		glDeleteProgram( program );
	}

	const char * ReadFile( const char * file )
	{
		std::ifstream fin( file );
		char * buffer;
		if( fin.is_open() )
		{
			fin.seekg (0, fin.end);
			int length = (int)fin.tellg();
			fin.seekg (0, fin.beg);
			buffer = new char[ length ];
			fin.read(buffer, length);
			//placing terminating character at the end
			buffer[ length - 1 ] = '\0';
			fin.close();
		}
		else //file could not be opened, either not found or do not have permission
		{
			std::cerr << "File could not be opened." << std::endl;
			assert(false);
		}
		return buffer;
	}
} // NAMESPACE END
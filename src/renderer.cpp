#include "renderer.hpp"

GLuint shaderProgram;

void RenderObject(unsigned int time)
{
	/*
	std::cout << "*---Start---*" << std::endl;
	LoadModel();
	LoadTexture();
	CreateShader();
	
	CreateVAO();
	CreateFBO();
	GetAttribute();
	GetUniform();
	SetAttribute();
	SetUniform();
	Draw();
	std::cout << "*---End---*" << std::endl;
	*/
}

void Draw()
{
	std::cout << "Drawing object" << std::endl;
}

void SetAttribute()
{
	std::cout << "Setting vertex attributes" << std::endl;
}

void GetAttribute()
{
	std::cout << "Getting vertex attributes" << std::endl;
}

void SetUniform()
{
	std::cout << "Setting shader uniforms" << std::endl;
}

void GetUniform()
{
	std::cout << "Getting shader uniforms" << std::endl;
}
// Create a Vertex array object
void CreateVAO()
{
	std::cout << "Creating vertex array object" << std::endl;
}

// Create a Frame buffer object

void CreateFBO()
{
	std::cout << "Creating frame buffer object" << std::endl;
}

// Import models and textures

void LoadModel()
{
	std::cout << "Loading model" << std::endl;
}

void LoadTexture()
{
	std::cout << "Loading texture" << std::endl;
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
	glDeleteTextures(1, &texture);
	
	return texture;
}

// Load and create a shader program

void LoadShaderSource()
{
	std::cout << "Loading shader source" << std::endl;
}

GLuint CreateShader(GLenum shaderType)
{
	std::cout << "Creating shader" << std::endl;
 	return glCreateShader(shaderType);
}

void CreateProgram()
{
	std::cout << "Creating shader program" << std::endl;
}

void CompileShader(GLuint shader)
{
	std::cout << "Compiling shader" << std::endl;
	glCompileShader( shader );
	GLint status;
	glGetShaderiv( shader, GL_COMPILE_STATUS, &status );
	
	if ( status == GL_FALSE )
	{
		std::cout << "Shader compilation failed" << std::endl;
		GLchar message[256];
		glGetShaderInfoLog( shader, sizeof(message), 0, &message[0] );
		std::cout << message << std::endl;
	}
}

void LinkShader()
{
	std::cout << "Linking shader" << std::endl;
//	glLinkProgram( m_programID );
//	GLint linkStatus;
//	glGetProgramiv( m_programID, GL_LINK_STATUS, &linkStatus );
//	
//	if ( linkStatus == GL_FALSE )
//	{
//		std::cout << "Shader linking failed" << std::endl;
//		GLchar message[256];
//		glGetProgramInfoLog( m_programID, sizeof(message), 0, &message[0]);
//		std::cout << message << std::endl;
//	}
	
}

void ValidateShader()
{
	std::cout << "Validating shader" << std::endl;
//	GLint validateStatus;
//	
//	glValidateProgram( m_programID );
//	glGetProgramiv( m_programID, GL_VALIDATE_STATUS, &validateStatus);
//	
//	//std::cout << "Validation status: " << validateStatus << std::endl;
//	
//	if (validateStatus == GL_FALSE)
//	{
//		std::cout << "Shader validation failed" << std::endl;
//		GLchar message[256];
//		glGetProgramInfoLog( m_programID, sizeof(message), 0, &message[0]);
//		std::cout << message << std::endl;
//	}
}

void GetShaderProgram()
{
	std::cout << "Getting shader program for use" << std::endl;
}

void DeleteShaderProgram()
{
	
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
	}
	return buffer;
}

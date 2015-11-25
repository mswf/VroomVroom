#include "shader.h"
#include "content.h"
#include "console.h"
#include "../Utilities/helperFunctions.h"

//TODO(VALENTINAS): Keep track of loaded shader programs, detach & link when reloading,
// GL_INVALID_FRAMEBUFFER_OPERATION Given when doing anything that would attempt to read from or write/render to a framebuffer that is not complete.
Shader::Shader()
	: program(0)
{
	std::string vs, fs;
	LoadDefault( vs, fs );

	GLuint vertex_shader_object = CreateShader( ShaderType::VERTEX, vs.c_str() );
	GLuint fragment_shader_object = CreateShader( ShaderType::FRAGMENT, fs.c_str() );
	
	program = CreateProgram(vertex_shader_object, fragment_shader_object);
	
	//glBindFragDataLocation(program, 0, "outColor");

	DeleteShaderObject( vertex_shader_object );
	DeleteShaderObject( fragment_shader_object );
}

Shader::~Shader()
{
	DeleteProgram( program );
}

void Shader::DeleteShaderObject( GLuint shader )
{
	glDeleteShader(shader);
}

void Shader::AttachShader( GLuint program, GLuint shader )
{
	glAttachShader( program, shader );
}

void Shader::DetachShader( GLuint program, GLuint shader )
{
	glDetachShader( program, shader );
}

void Shader::DeleteProgram( GLuint program )
{
	glDeleteProgram(program);
}

GLuint Shader::CreateShader( ShaderType shaderType, const char* source )
{
	GLenum shader = GetGLShaderEnum( shaderType );
	GLuint shaderObject = glCreateShader( shader );
	CheckGlError( "CreateShader" );
	
	// uint shader, sizei count, const char** string shader_source, const int lenght
	glShaderSource( shaderObject, 1, &source, NULL );
	CheckGlError( "ShaderSource" );
	
	glCompileShader( shaderObject );
	if ( !ShaderInfoLog( shaderObject, GL_COMPILE_STATUS ) )
	{
		return 0;
	}
	return shaderObject;
}

bool Shader::ShaderInfoLog( GLuint program, GLenum status )
{
	GLint log;
	glGetShaderiv( program, status, &log);
	if (log == GL_FALSE)
	{
		GLchar message[255];
		glGetShaderInfoLog( program, sizeof(message), 0, &message[0]);
		//printf("%s", message);
		Terminal.LogOpenGL( std::string( message ) );
		return false;
	}
	return true;
}

template<typename... Targs>
GLuint Shader::CreateProgram( Targs... shaderObjects )
{
	int i;
	const int size = sizeof...( shaderObjects );
	GLuint shaders[ size ] = { shaderObjects... };
	GLuint shaderProgram = glCreateProgram();
	for (i = 0; i < size; ++i)
	{
		AttachShader( shaderProgram, shaders[i] );
	}
	glLinkProgram( shaderProgram );
	ProgramInfoLog( shaderProgram, GL_LINK_STATUS );
	return shaderProgram;
}

bool Shader::ProgramInfoLog( GLuint program, GLenum status )
{
	GLint log;
	glGetProgramiv( program, status, &log);
	if (log == GL_FALSE)
	{
		GLchar message[255];
		glGetProgramInfoLog( program, sizeof(message), 0, &message[0]);
		//TODO(Valentinas): Find out why the Warning message not passed
		Terminal.LogOpenGL( std::string( message ) );
		return false;
	}
	return true;
}

void Shader::UseProgram()
{
	glUseProgram( program );
}

void Shader::LoadDefault( std::string &vs, std::string &fs )
{
	std::string defaultVertex = Content::GetPath() + std::string("/shaders/vertexShader.vert");
	std::string defaultFragment = Content::GetPath() + std::string("/shaders/fragmentShader.frag");
	bool vExists = HelperFunctions::FileExists( defaultVertex.c_str() );
	bool fExists = HelperFunctions::FileExists( defaultFragment.c_str() );

	if ( vExists && fExists )
	{
		vs = HelperFunctions::ReadFile(Content::GetPath() + std::string("/shaders/vertexShader.vert"));
		fs = HelperFunctions::ReadFile(Content::GetPath() + std::string("/shaders/fragmentShader.frag"));
	}
	else
	{
		vs = builtin_vertex;
		fs = builtin_fragment;
	}
}

void Shader::LogActiveAttributes( GLuint shaderProgram )
{
	GLint num_properties, size;
	int i;
	GLchar property_name[256];
	GLsizei length;
	GLenum type;
	
	glGetProgramiv( shaderProgram, GL_ACTIVE_ATTRIBUTES, &num_properties);
	
	for ( i = 0; i < num_properties; ++i )
	{
		glGetActiveUniform(shaderProgram, i, sizeof(property_name), &length, &size, &type, property_name);
		Terminal.LogOpenGL( std::string( "Attribute " + std::to_string(i) + ": " + property_name ) );
	}
}


void Shader::LogActiveUniforms( GLuint shaderProgram )
{
	GLint num_properties, size;
	int i;
	GLchar property_name[256];
	GLsizei length;
	GLenum type;

	glGetProgramiv( shaderProgram, GL_ACTIVE_UNIFORMS, &num_properties);
	
	for ( i = 0; i < num_properties; ++i )
	{
		glGetActiveUniform(shaderProgram, i, sizeof(property_name), &length, &size, &type, property_name);
		Terminal.LogOpenGL( std::string( "Uniform " + std::to_string(i) + ": " + property_name ) );
	}
}

bool Shader::ValidateProgram( GLuint shaderProgram )
{
	glValidateProgram( shaderProgram );
	return ProgramInfoLog( shaderProgram, GL_VALIDATE_STATUS );;
}

void Shader::CheckGlError( const char* caller )
{
	GLenum error_found = glGetError();
	if ( error_found != GL_NO_ERROR )
	{
		std::string err;
		switch ( error_found )
		{
			case GL_INVALID_ENUM:
			{
				err = " Invalid Enum. ";
				break;
			}
			case GL_INVALID_VALUE:
			{
				err = " Invalid value. ";
				break;
			}
			case GL_INVALID_OPERATION:
			{
				err = " Invalid operation. ";
				break;
			}
			case GL_STACK_OVERFLOW:
			{
				err = " Stack overflow. ";
				break;
			}
			case GL_STACK_UNDERFLOW:
			{
				err = " Stack underflow. ";
				break;
			}
			case GL_OUT_OF_MEMORY:
			{
				err = " Out of memory. ";
				break;
			}
			default:
				err = " UNKNOWN. ";
				break;
		}
		string glewErr( (const char*)glewGetErrorString(error_found) );
		Terminal.LogOpenGL( "Error from caller " + std::string(caller) + ". GL:" + err + "Glew: " + glewErr + "." );
	}
}

GLenum Shader::GetGLShaderEnum( ShaderType t )
{
	GLenum shader = 0;
	switch (t)
	{
		case ShaderType::VERTEX:
		{
			shader = GL_VERTEX_SHADER;
			break;
		}
		case ShaderType::GEOMETRY:
		{
			shader = GL_GEOMETRY_SHADER;
			break;
		}
		case ShaderType::FRAGMENT:
		{
			shader = GL_FRAGMENT_SHADER;
			break;
		}
		default:
		{
			shader = 0;
			break;
		}
	}
	return shader;
}

void Shader::SetUniformInt( const char* uniform, int value )
{
	glUniform1i( glGetUniformLocation ( program, uniform ), value );
}

void Shader::SetUniformFloat( const char* uniform, float value )
{
	glUniform1f( glGetUniformLocation( program, uniform ), value );
}

void Shader::SetUniformFloat2(const char* uniform, glm::vec2 value)
{
	glUniform2f( glGetUniformLocation( program, uniform ), value.x, value.y );
}

void Shader::SetUniformFloat3(const char* uniform, glm::vec3 value)
{
	glUniform3f( glGetUniformLocation( program, uniform ), value.x, value.y, value.z);
}

void Shader::SetUniformMat3( const char* uniform, glm::mat3 value )
{
	glUniformMatrix3fv( glGetUniformLocation( program, uniform ), 1, GL_FALSE, glm::value_ptr(value) );
}

void Shader::SetUniformMat4( const char* uniform, glm::mat4 value )
{
	glUniformMatrix4fv( glGetUniformLocation( program, uniform ), 1, GL_FALSE, glm::value_ptr(value) );
}
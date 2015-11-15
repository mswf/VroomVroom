#include "shader.h"
#include "content.h"
#include "console.h"
#include "../Utilities/helperFunctions.h"

//TODO(VALENTINAS): Keep track of loaded shader programs, detach & link when reloading,
//TODO(VALENTINAS): Detach shader from all programs when deleting,
Shader::Shader()
: program(0)
{
	std::string path = Content::GetPath() + "/shaders/";

	std::string vs = HelperFunctions::ReadFile(path + std::string("vertexShader.vert"));
	//std::string gs = HelperFunctions::ReadFile(path + std::string("geometryShader.geom"));
	std::string fs = HelperFunctions::ReadFile(path + std::string("fragmentShader.frag"));
	
	const char * src_vertex   = vs.c_str();
	//const char * src_geometry = gs.c_str();
	const char * src_fragment = fs.c_str();
	program = glCreateProgram();
	
	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	//GLuint geometry_shader = glCreateShader(GL_GEOMETRY_SHADER);
	
	glShaderSource( vertex_shader, 1, &src_vertex, NULL );
	glShaderSource( fragment_shader, 1, &src_fragment, NULL );
	//glShaderSource( geometry_shader, 1, &src_geometry, NULL );
	
	glCompileShader( vertex_shader );
	ShaderInfoLog(vertex_shader, GL_COMPILE_STATUS);
	//glCompileShader( geometry_shader );
	//ShaderInfoLog(geometry_shader, GL_COMPILE_STATUS);
	glCompileShader( fragment_shader );
	ShaderInfoLog(fragment_shader, GL_COMPILE_STATUS);
	
	glAttachShader( program, vertex_shader );
	//glAttachShader( program, geometry_shader );
	glAttachShader( program, fragment_shader );
	
	//glBindFragDataLocation(program, 0, "outColor");
	
	glLinkProgram( program );
	ProgramInfoLog(program, GL_LINK_STATUS);
	
	// ATTRIBUTE & UNIFORM INFORMATION
	
	//LogActiveProperties(GL_ACTIVE_ATTRIBUTES);
	//LogActiveProperties(GL_ACTIVE_UNIFORMS);
	
	// END ATTRIBUTE & UNIFORM INFORMATION
	
	glDeleteShader( vertex_shader );
	//glDeleteShader( geometry_shader );
	glDeleteShader( fragment_shader );
}

Shader::~Shader()
{
	glDeleteProgram( program );
}

void Shader::LogActiveProperties( GLenum activeProperties )
{
	GLint num_properties, size;
	int i;
	GLchar property_name[256];
	GLsizei length;
	GLenum type;
	
	glGetProgramiv(program, activeProperties, &num_properties);
	
	switch (activeProperties)
	{
		case GL_ACTIVE_ATTRIBUTES:
		{
			for ( i = 0; i < num_properties; ++i )
			{
				glGetActiveAttrib(program, i, sizeof(property_name), &length, &size, &type, property_name);
				Terminal.LogOpenGL( std::string( "Attribute " + std::to_string(i) + ": " + property_name ) );
			}
			break;
		}
		case GL_ACTIVE_UNIFORMS:
		{
			for ( i = 0; i < num_properties; ++i )
			{
				glGetActiveUniform(program, i, sizeof(property_name), &length, &size, &type, property_name);
				Terminal.LogOpenGL( std::string( "Uniform " + std::to_string(i) + ": " + property_name ) );
			}
			break;
		}
		default:
		{
			break;
		}
	}
}


void Shader::ProgramInfoLog( GLuint program, GLenum status )
{
	GLint log;
	glGetProgramiv( program, status, &log);
	if (log == GL_FALSE)
	{
		GLchar message[255];
		glGetProgramInfoLog( program, sizeof(message), 0, &message[0]);
		std::cout << message << std::endl;
		//TODO(Valentinas): Find out why the Warning message not passed
		//Terminal.Warning( std::string( message ) );
		//assert(false);
	}
}

void Shader::ShaderInfoLog( GLuint program, GLenum status )
{
	GLint log;
	glGetShaderiv( program, status, &log);
	if (log == GL_FALSE)
	{
		GLchar message[255];
		glGetShaderInfoLog( program, sizeof(message), 0, &message[0]);
		printf("%s", message);
		std::cout << message << std::endl;
		//TODO(Valentinas): Do not assert when HOTReloading
		//Terminal.Warning( std::string( message ) );
		//assert(false);
	}
}

void Shader::ValidateProgram()
{
	glValidateProgram( program );
	ProgramInfoLog(program, GL_VALIDATE_STATUS);
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
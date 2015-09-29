//
//  shader.cpp
//  VroomVroom
//
//  Created by Valentinas Rimeika on 24/09/15.
//  Copyright © 2015 Valentinas Rimeika. All rights reserved.
//

#include "shader.h"
#include "Utilities/helperFunctions.h"

#include "SDL2/SDL.h"

Renderer::Shader::Shader(GLuint vao)
: program(0)
{
	glBindVertexArray( vao );
	std::string path = SDL_GetBasePath();
	std::string vs = ReadFile((path + std::string("vs.txt")).c_str());
	std::string fs = ReadFile((path + std::string("fs.txt")).c_str());
	
	HelperFunctions::ConvertLineEndings(vs);
	HelperFunctions::ConvertLineEndings(fs);

	const char * src_vertex   = vs.c_str();
	const char * src_fragment = fs.c_str();
	program = glCreateProgram();
	
	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	
	glShaderSource( vertex_shader, 1, &src_vertex, NULL);
	glShaderSource( fragment_shader, 1, &src_fragment, NULL);
	
	glCompileShader( vertex_shader );
	LogError(vertex_shader, GL_COMPILE_STATUS);
	glCompileShader( fragment_shader );
	LogError(fragment_shader, GL_COMPILE_STATUS);
	
	glAttachShader( program, vertex_shader );
	glAttachShader( program, fragment_shader );
	
	glBindFragDataLocation(program, 0, "outColor");
	
	glLinkProgram( program );
	LogError(program, GL_LINK_STATUS);
	
	glValidateProgram( program );
	LogError(program, GL_VALIDATE_STATUS);

	
	glDeleteShader( vertex_shader );
	glDeleteShader( fragment_shader );
}

Renderer::Shader::~Shader()
{
	glDeleteProgram( program );
}

void Renderer::Shader::LogError( GLuint program, GLenum status )
{
	GLint log;
	glGetProgramiv( program, status, &log);
	if (log == GL_FALSE)
	{
		GLchar message[255];
		glGetProgramInfoLog( program, sizeof(message), 0, &message[0]);
		std::cout << message << std::endl;
		assert(false);
	}
}

void Renderer::Shader::SetUniformInt( const char* uniform, int value )
{
	glUniform1i( glGetUniformLocation ( program, uniform ), value );
}

void Renderer::Shader::SetUniformFloat( const char* uniform, float value )
{
	glUniform1f( glGetUniformLocation( program, uniform ), value );
}

void Renderer::Shader::SetUniformFloat2(const char* uniform, glm::vec2 value)
{
	glUniform2f( glGetUniformLocation( program, uniform ), value.x, value.y );
}

void Renderer::Shader::SetUniformFloat3(const char* uniform, glm::vec3 value)
{
	glUniform3f( glGetUniformLocation( program, uniform ), value.x, value.y, value.z);
}

void Renderer::Shader::SetUniformMat4( const char* uniform, glm::mat4 value )
{
	glUniformMatrix4fv( glGetUniformLocation( program, uniform ), 1, GL_FALSE, glm::value_ptr(value) );
	
}
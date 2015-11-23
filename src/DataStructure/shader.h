#ifndef shader_h
#define shader_h

#include <glew.h>
#include <iostream>
#include <string>
#include "../glm/vec2.hpp"
#include "../glm/vec3.hpp"
#include "../glm/mat4x4.hpp"
#include "../glm/gtc/type_ptr.hpp"


class Shader
{
	public:
		GLuint program;
	public:
		Shader();
		~Shader();
		void UseProgram();
		void SetUniformInt( const char* uniform, int value );
		void SetUniformFloat( const char* uniform, float value );
		void SetUniformFloat2( const char* uniform, glm::vec2 value );
		void SetUniformFloat3( const char* uniform, glm::vec3 value );
		void SetUniformMat3( const char* uniform, glm::mat3 value );
		void SetUniformMat4( const char* uniform, glm::mat4 value );
		void ValidateProgram();
		void LogActiveProperties( GLenum activeProperties );
	
	private:
		void LoadDefault( std::string& vs, std::string& fs );
		void ProgramInfoLog( GLuint program, GLenum status );
		void ShaderInfoLog( GLuint shader, GLenum status );
	
		const char* builtin_vertex =
		"#version 410\n"
		"layout(location = 0) in vec3 position;\n"
		"layout(location = 1) in vec2 texcoord;\n"
		"uniform mat4 model;\n"
		"uniform mat4 view;\n"
		"uniform mat4 projection;\n"
		"out vec2 uv;\n"
		"void main()\n"
		"{\n"
		"	uv = texcoord;\n"
		"	gl_Position = projection * view * model * vec4( position, 1.0 );\n"
		"}\n";
	
		const char* builtin_fragment =
		"#version 410\n"
		"uniform sampler2D colorMap;\n"
		"uniform float time;\n"
		"in vec2 uv;\n"
		"out vec4 Out_Color;\n"
		"void main()\n"
		"{\n"
		"	Out_Color = vec4(1.0, 1.0, 0.0, 1.0) * texture( colorMap, vec2(uv.s, uv.t * time) ); \n"
		"}\n";
};


#endif /* shader_h */

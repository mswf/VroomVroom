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
		void SetUniformInt( const char* uniform, int value );
		void SetUniformFloat( const char* uniform, float value );
		void SetUniformFloat2( const char* uniform, glm::vec2 value );
		void SetUniformFloat3( const char* uniform, glm::vec3 value );
		void SetUniformMat3( const char* uniform, glm::mat3 value );
		void SetUniformMat4( const char* uniform, glm::mat4 value );
		void ValidateProgram();
	void LogActiveProperties( GLenum activeProperties );
	private:
		void ProgramInfoLog( GLuint program, GLenum status );
		void ShaderInfoLog( GLuint shader, GLenum status );
};


#endif /* shader_h */

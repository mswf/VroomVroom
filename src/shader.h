//
//  Shader.hpp
//  VroomVroom
//
//  Created by Valentinas Rimeika on 24/09/15.
//  Copyright © 2015 Valentinas Rimeika. All rights reserved.
//

#ifndef shader_h
#define shader_h

#include <glew.h>
#include <iostream>
#include <fstream>
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace Renderer
{
	class Shader
	{
	public:
		GLuint program;
	public:
		Shader( GLuint vao );
		~Shader();
		void SetUniformInt( const char* uniform, int value );
		void SetUniformFloat( const char* uniform, float value );
		void SetUniformFloat2( const char* uniform, glm::vec2 value );
		void SetUniformFloat3( const char* uniform, glm::vec3 value );
		void SetUniformMat4( const char* uniform, glm::mat4 value );
	private:
		const char* ReadFile( const char* path );
		void LogError( GLuint program, GLenum pname);
	};
}


#endif /* shader_h */

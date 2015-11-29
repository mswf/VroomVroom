#ifndef shader_h
#define shader_h

#include <glew.h>
#include <string>
#include <map>
#include "../glm/vec2.hpp"
#include "../glm/vec3.hpp"
#include "../glm/mat4x4.hpp"
#include "../glm/gtc/type_ptr.hpp"
#include "data_types.h"

enum class GLSLShaderType
{
	VERTEX,
	GEOMETRY,
	FRAGMENT,
	TESS_CONTROL,
	TESS_EVALUATION
};

class Shader
{
	public:
	
		GLuint program;
	
		Shader();
		Shader( std::string vertex, std::string fragment );
	
		~Shader();
		void UseProgram();
		void UseProgramStages( GLuint pPipeline, GLSLShaderType shaderType, GLuint program );
		GLuint CreateShader( GLSLShaderType shaderType, const char* source );
	
		template<typename... Targs>
		GLuint CreateProgram( Targs... shaders );
		GLuint CreateSeparateProgram( GLenum shaderObject );
	
		void DeleteProgram( GLuint program );
		void DeleteShaderObject( GLuint shader );
		void DetachShader( GLuint program, GLuint shader );
		void AttachShader( GLuint program, GLuint shader );
		void LinkProgram( GLuint program );
		bool ValidateProgram( GLuint shaderProgram );
		bool IsShader( GLuint shader );
		bool IsProgram( GLuint program );
	
		void SetUniform( const char* name, int val );
		void SetUniform( const char* name, unsigned int val );
		void SetUniform( const char* name, float val );
		void SetUniform( const char* name, double val );
		void SetUniform( const char* name, const glm::vec2& vec );
		void SetUniform( const char* name, const glm::vec3& vec );
		void SetUniform( const char* name, const glm::vec4& vec );
		void SetUniform( const char* name, const glm::ivec2& vec );
		void SetUniform( const char* name, const glm::ivec3& vec );
		void SetUniform( const char* name, const glm::ivec4& vec );
		void SetUniform( const char* name, const glm::uvec2& vec );
		void SetUniform( const char* name, const glm::uvec3& vec );
		void SetUniform( const char* name, const glm::uvec4& vec );
		void SetUniform( const char* name, const glm::mat2& mat );
		void SetUniform( const char* name, const glm::mat3& mat );
		void SetUniform( const char* name, const glm::mat4& mat );
	
		void SetActiveSubroutine( GLuint program, ShaderObject shader, const char* uniform, const char* routine );
	
		void LogActiveAttributes( GLuint program );
		void LogActiveUniforms( GLuint program );
		void LogActiveSubroutines( GLuint shaderProgram, GLenum shaderType );
	
	
private:
	
		unsigned int GetSubroutineUniformLocation( const char* name, const std::map< std::string, unsigned int >& locations );
		void SetSubroutineUniformLocations( GLuint program, ShaderObject& shader );
	
		GLenum GetGLShaderEnum( GLSLShaderType type, bool separate = false );
		bool ProgramInfoLog( GLuint program, GLenum status );
		bool ShaderInfoLog( GLuint shader, GLenum status );
	
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

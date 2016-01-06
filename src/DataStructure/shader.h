#ifndef shader_h
#define shader_h

#include <glew.h>
#include <string>
#include <map>
#include "data_types.h"
#include "../glm/vec2.hpp"
#include "../glm/vec3.hpp"
#include "../glm/mat4x4.hpp"

enum class GLSLShaderType
{
	NONE,
	VERTEX,
	GEOMETRY,
	FRAGMENT,
	TESS_CONTROL,
	TESS_EVALUATION,
	UNKNOWN
};
	
void UseProgram( GLuint program );
void UseProgramStages( GLuint pPipeline, GLSLShaderType shaderType, GLuint program );
void CreateShader( GLuint& shader, GLenum shaderType, const char* source );

void CreateProgram( GLuint& program, const unsigned int* shaders, unsigned int count, bool separate = false );
void RemakeProgram( GLuint& program, const unsigned int* shaders, unsigned int count, bool separate = false );

void DeleteProgram( GLuint program );
void DeleteShaderObject( GLuint shader );
void DetachShader( GLuint program, GLuint shader );
void AttachShader( GLuint program, GLuint shader );
void LinkProgram( GLuint program );
bool ValidateProgram( GLuint program );
bool IsShader( GLuint shader );
bool IsProgram( GLuint program );
bool IsAttached( GLuint program, GLuint shader );
GLint GetAttachedShaderCount( GLuint program );
GLint GetAttributeLocation( GLuint program, const char* name );

void SetUniform( GLuint program, const char* name, int val );
void SetUniform( GLuint program, const char* name, unsigned int val );
void SetUniform( GLuint program, const char* name, float val );
void SetUniform( GLuint program, const char* name, double val );
void SetUniform( GLuint program, const char* name, const glm::vec2& vec );
void SetUniform( GLuint program, const char* name, const glm::vec3& vec );
void SetUniform( GLuint program, const char* name, const glm::vec4& vec );
void SetUniform( GLuint program, const char* name, const glm::ivec2& vec );
void SetUniform( GLuint program, const char* name, const glm::ivec3& vec );
void SetUniform( GLuint program, const char* name, const glm::ivec4& vec );
void SetUniform( GLuint program, const char* name, const glm::uvec2& vec );
void SetUniform( GLuint program, const char* name, const glm::uvec3& vec );
void SetUniform( GLuint program, const char* name, const glm::uvec4& vec );
void SetUniform( GLuint program, const char* name, const glm::mat2& mat );
void SetUniform( GLuint program, const char* name, const glm::mat3& mat );
void SetUniform( GLuint program, const char* name, const glm::mat4& mat );


void LogActiveAttributes( GLuint program );
void LogActiveUniforms( GLuint program );
void LogActiveUniformBlocks( GLuint program );
void LogActiveSubroutines( GLuint program, GLenum shaderType );

bool HasSubroutines( const GLuint& program, GLenum shaderType );
void SetActiveSubroutine( GLuint program, GLuint shaderType, Subroutines* sub, const char* uniform, const char* routine );
GLuint GetSubroutineUniformLocation( const char* name, const std::map< std::string, GLuint >& locations );
void SetSubroutineUniformLocations( GLuint program, GLuint shader, Subroutines& routines );

GLenum GetGLShaderEnum( GLSLShaderType type, bool separate = false );
bool ProgramInfoLog( GLuint program, GLenum status );
bool ShaderInfoLog( GLuint shader, GLenum status );

std::string GetGLSLUniformType( GLenum type );

#ifndef GLSL_SHADER_FILE
#define GLSL_SHADER_FILE(version, A) "#version " #version "\n" #A
#endif

static const char* builtin_vertex = GLSL_SHADER_FILE( 410,
	layout(location = 0) in vec3 position;
	layout(location = 2) in vec2 texcoord;
	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 projection;
	out vec2 uv;
	void main()
	{
		uv = texcoord;
		gl_Position = projection * view * model * vec4( position, 1.0 );
	}
);

static const char* builtin_fragment = GLSL_SHADER_FILE( 410,
	uniform sampler2D colorMap;
	uniform float time;
	in vec2 uv;
	out vec4 FragColor;
	void main()
	{
		FragColor = texture( colorMap, uv );
	}
);


#endif /* shader_h */

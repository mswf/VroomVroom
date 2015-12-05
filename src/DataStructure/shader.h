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
	TESS_EVALUATION
};
	
void UseProgram( GLuint program );
void UseProgramStages( GLuint pPipeline, GLSLShaderType shaderType, GLuint program );
void CreateShader( GLuint& shader, GLenum shaderType, const char* source );

void CreateProgram( GLuint& program, const unsigned int* shaders, unsigned int count, bool separate = false );

void DeleteProgram( GLuint program );
void DeleteShaderObject( GLuint shader );
void DetachShader( GLuint program, GLuint shader );
void AttachShader( GLuint program, GLuint shader );
void LinkProgram( GLuint program );
bool ValidateProgram( GLuint program );
bool IsShader( GLuint shader );
bool IsProgram( GLuint program );

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

bool HasSubroutines( const GLuint& program, GLenum shaderType );
void SetActiveSubroutine( GLuint program, ShaderObject shader, const char* uniform, const char* routine );

void LogActiveAttributes( GLuint program );
void LogActiveUniforms( GLuint program );
void LogActiveSubroutines( GLuint program, GLenum shaderType );

unsigned int GetSubroutineUniformLocation( const char* name, const std::map< std::string, unsigned int >& locations );
void SetSubroutineUniformLocations( GLuint program, ShaderObject& shader );

GLenum GetGLShaderEnum( GLSLShaderType type, bool separate = false );
bool ProgramInfoLog( GLuint program, GLenum status );
bool ShaderInfoLog( GLuint shader, GLenum status );

static const char* builtin_vertex =
"#version 410\n"
"layout(location = 0) in vec3 position;\n"
"layout(location = 2) in vec2 texcoord;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"out vec2 uv;\n"
"void main()\n"
"{\n"
"	uv = texcoord;\n"
"	gl_Position = projection * view * model * vec4( position, 1.0 );\n"
"}\n";

static const char* builtin_fragment =
"#version 410\n"
"uniform sampler2D colorMap;\n"
"uniform float time;\n"
"in vec2 uv;\n"
"out vec4 Out_Color;\n"
"void main()\n"
"{\n"
"	Out_Color = texture( colorMap, uv ); \n"
"}\n";


#endif /* shader_h */

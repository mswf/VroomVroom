#include "shader.h"
#include "../Utilities/opengl_common.h"
#include "../Utilities/helperFunctions.h"
#include "../glm/gtc/type_ptr.hpp"
#include "../content.h"
#include "../console.h"


/*

 UniformBlock
 Get uniform block location
 
 create material uniform buffer
	glGenBuffers( 1, &(aMesh.uniformBlockIndex) );
	glBindBuffer( GL_UNIFORM_BUFFER,aMesh.uniformBlockIndex );
	glBufferData( GL_UNIFORM_BUFFER, sizeof(aMat), (void *)(&aMat), GL_STATIC_DRAW );

	OTHER:
 	
 	// GL_INVALID_FRAMEBUFFER_OPERATION Given when doing anything that would attempt to read from or write/render to a framebuffer that is not complete.
 
 	glBindAttribLocation
	glBindFragDataLocation(program, 0, "outColor");

	
	//LogActiveSubroutines(program, GL_FRAGMENT_SHADER);
	
	SetSubroutineUniformLocations( program, fragment_shader_object );
	SetActiveSubroutine( program, fragment_shader_object, "shadeModel", "phongModel");
	
	GLuint programV = CreateSeparateProgram(vertex_shader_object);
	GLuint programF = CreateSeparateProgram(fragment_shader_object);
	
	GLuint ppo;
	glGenProgramPipelines(1, &ppo);
	glBindProgramPipeline(ppo);
	
	UseProgramStages( ppo, GLSLShaderType::VERTEX, programV );
	UseProgramStages( ppo, GLSLShaderType::FRAGMENT, programF );
}
*/

void DeleteShaderObject( GLuint shader )
{
	glDeleteShader(shader);
	//GL_INVALID_VALUE is generated if shader is not a value generated by OpenGL.
	CheckGlError( "glDeleteShader" );
}

void AttachShader( GLuint program, GLuint shader )
{
	if ( IsAttached( program, shader ) )
	{
		Terminal.LogOpenGL( "Shader is already attached to this program." );
	}
	glAttachShader( program, shader );
	// GL_INVALID_VALUE is generated if either program or shader is not a value generated by OpenGL.
	// GL_INVALID_OPERATION is generated if program is not a program object.
	// GL_INVALID_OPERATION is generated if shader is not a shader object.
	// GL_INVALID_OPERATION is generated if shader is already attached to program.
	CheckGlError( "glAttachShader" );
}

void DetachShader( GLuint program, GLuint shader )
{
	glDetachShader( program, shader );
	//GL_INVALID_VALUE is generated if either program or shader is a value that was not generated by OpenGL.
	//GL_INVALID_OPERATION is generated if program is not a program object.
	//GL_INVALID_OPERATION is generated if shader is not a shader object.
	//GL_INVALID_OPERATION is generated if shader is not attached to program.
	CheckGlError( "glDetachShader" );
}

void DeleteProgram( GLuint program )
{
	glDeleteProgram(program);
}

void LinkProgram( GLuint program )
{
	glLinkProgram( program );
	// GL_INVALID_VALUE is generated if program is not a value generated by OpenGL.
	// GL_INVALID_OPERATION is generated if program is not a program object.
	// GL_INVALID_OPERATION is generated if program is the currently active program object and transform feedback mode is active.
	CheckGlError( "glLinkProgram" );
}

void CreateShader( GLuint& shader, GLenum shaderType, const char* source )
{
	shader = glCreateShader( shaderType );
	
	// GL_INVALID_ENUM is generated if shaderType is not an accepted value.
	CheckGlError( "glCreateShader" );
	
	// uint shader, sizei count, const char** string shader_source, const int lenght
	glShaderSource( shader, 1, &source, NULL );
	
	// GL_INVALID_VALUE is generated if shader is not a value generated by OpenGL.
	// GL_INVALID_OPERATION is generated if shader is not a shader object.
	// GL_INVALID_VALUE is generated if count is less than 0.
	CheckGlError( "glShaderSource" );
	
	glCompileShader( shader );
	if ( !ShaderInfoLog( shader, GL_COMPILE_STATUS ) )
	{
		shader = 0;
	}
	CheckGlError( "glCompileShader" );
}

bool ShaderInfoLog( GLuint program, GLenum status )
{
	GLint log;
	glGetShaderiv( program, status, &log);
	if (log == GL_FALSE)
	{
		GLchar message[255];
		glGetShaderInfoLog( program, sizeof(message), 0, &message[0]);
		Terminal.LogOpenGL( "ShaderLog: " + std::string( message ), true );
		return false;
	}
	return true;
}

void CreateProgram( GLuint& program, const unsigned int* shaders, unsigned int count, bool separate )
{
	int i;
	program = glCreateProgram();
	if ( program == 0 )
	{
		Terminal.LogOpenGL( "glCreateProgram failed to create a program object." );
	}
	for (i = 0; i < count; ++i)
	{
		AttachShader( program, shaders[i] );
	}
	if (separate)
	{
		glProgramParameteri( program, GL_PROGRAM_SEPARABLE, GL_TRUE );
	}
	LinkProgram( program );
	ProgramInfoLog( program, GL_LINK_STATUS );
}

void RemakeProgram( GLuint& program, const unsigned int* shaders, unsigned int count, bool separate )
{
	int i;
	for (i = 0; i < count; ++i)
	{
		AttachShader( program, shaders[i] );
	}
	if (separate)
	{
		glProgramParameteri( program, GL_PROGRAM_SEPARABLE, GL_TRUE );
	}
	LinkProgram( program );
	ProgramInfoLog( program, GL_LINK_STATUS );
	CheckGlError( "RemakeProgram" );
}

bool ProgramInfoLog( GLuint program, GLenum status )
{
	GLint log;
	glGetProgramiv( program, status, &log);
	if (log == GL_FALSE)
	{
		GLchar message[255];
		glGetProgramInfoLog( program, sizeof(message), 0, &message[0]);
		Terminal.LogOpenGL( "ProgramLog: " + std::string( message ), true );
		return false;
	}
	return true;
}

void UseProgram( GLuint program )
{
	glUseProgram( program );
	// GL_INVALID_VALUE is generated if program is neither 0 nor a value generated by OpenGL.
	// GL_INVALID_OPERATION is generated if program is not a program object.
	// GL_INVALID_OPERATION is generated if program could not be made part of current state.
	// GL_INVALID_OPERATION is generated if transform feedback mode is active.
	CheckGlError( "glUseProgram" );
}

void UseProgramStages( GLuint pPipeline, GLSLShaderType shaderType, GLuint program )
{
	glUseProgramStages( pPipeline, GetGLShaderEnum(shaderType), program );
	CheckGlError( "UseProgramStages" );
}

bool IsShader( GLuint shader )
{
	return glIsShader( shader );
}

bool IsProgram( GLuint program )
{
	return glIsProgram( program );
}

bool IsAttached( GLuint program, GLuint shader )
{
	GLint maxCount = 1024;
	GLint actualCount = GetAttachedShaderCount( program );
	GLuint* shaders = new GLuint[maxCount];
	glGetAttachedShaders( program, maxCount, &actualCount, shaders );
	for (int i = 0; i < actualCount; ++i)
	{
		if (shaders[i] == shader)
		{
			//printf("Shader is attached, %i == %i \n", shaders[i], shader );
			return true;
		}
	}
	CheckGlError("IsAttached");

	delete shaders;
	return false;
}

GLint GetAttachedShaderCount( GLuint program )
{
	GLint num_shaders;
	glGetProgramiv( program, GL_ATTACHED_SHADERS, &num_shaders );
	// GL_INVALID_VALUE is generated if program is not a value generated by OpenGL.
	// GL_INVALID_OPERATION is generated if program does not refer to a program object.
	// GL_INVALID_OPERATION is generated if pname is GL_GEOMETRY_VERTICES_OUT, GL_GEOMETRY_INPUT_TYPE, or GL_GEOMETRY_OUTPUT_TYPE, and program does not contain a geometry shader.
	// GL_INVALID_ENUM is generated if pname is not an accepted value.
	// GL_INVALID_OPERATION is generated if pname is GL_COMPUTE_WORK_GROUP_SIZE and program does not contain a binary for the compute shader stage.
	CheckGlError( "GetAttachedShaderCount" );
	return num_shaders;
}

bool ValidateProgram( GLuint program )
{
	glValidateProgram( program );
	return ProgramInfoLog( program, GL_VALIDATE_STATUS );
}

void LogActiveAttributes( GLuint program )
{
	GLint num_properties, size;
	int i;
	GLchar property_name[256];
	GLsizei length;
	GLenum type;
	
	glGetProgramiv( program, GL_ACTIVE_ATTRIBUTES, &num_properties);
	
	for ( i = 0; i < num_properties; ++i )
	{
		glGetActiveUniform(program, i, sizeof(property_name), &length, &size, &type, property_name);
		Terminal.LogOpenGL( std::string( "Attribute " + std::to_string(i) + ": " + property_name ) );
	}
}

void LogActiveUniforms( GLuint program )
{
	GLint num_properties, size;
	int i;
	GLchar property_name[256];
	GLsizei length;
	GLenum type;

	glGetProgramiv( program, GL_ACTIVE_UNIFORMS, &num_properties);
	
	for ( i = 0; i < num_properties; ++i )
	{
		glGetActiveUniform(program, i, sizeof(property_name), &length, &size, &type, property_name);
		Terminal.LogOpenGL( std::string( "Uniform " + std::to_string(i) + ": " + property_name ) );
	}
}

void LogActiveUniformBlocks( GLuint program )
{
	
	GLuint blockIndex = glGetUniformBlockIndex( program, "Material" );
	CheckGlError("glGetUniformBlockIndex");
	
	//GL_ACTIVE_UNIFORM_BLOCKS
	GLint uniforms = 0;
	glGetProgramiv( program, GL_ACTIVE_UNIFORM_BLOCKS, &uniforms );
	CheckGlError("Active Uniforms");
	
	for ( int i = 0; i < uniforms; ++i )
	{
		//GL_UNIFORM_BLOCK_NAME_LENGTH
		GLsizei activeUniformLength = 0;
		GLchar uniformName[256];
		glGetActiveUniformBlockiv( program, i, GL_UNIFORM_BLOCK_NAME_LENGTH, &activeUniformLength);
		CheckGlError("activeUniformBinding");
		
		glGetActiveUniformBlockName( program, i, 256, &activeUniformLength, uniformName );
		CheckGlError("glGetActiveUniformBlockName");
		
		//GL_UNIFORM_BLOCK_DATA_SIZE
		GLint blockSize = 0;
		glGetActiveUniformBlockiv( program, blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);
		CheckGlError("Block Data size");
		
		//GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS
		GLint activeUniforms = 0;
		glGetActiveUniformBlockiv( program, blockIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &activeUniforms);
		CheckGlError("activeUniforms");
		
		//GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES
		GLint activeUniformIndices = 0;
		glGetActiveUniformBlockiv( program, blockIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &activeUniformIndices);
		CheckGlError("activeUniformIndices");
		
		//GL_UNIFORM_BLOCK_BINDING
		GLint activeUniformBinding = 0;
		glGetActiveUniformBlockiv( program, blockIndex, GL_UNIFORM_BLOCK_BINDING, &activeUniformBinding);
		CheckGlError("activeUniformBinding");
		
		GLint uniformblockReferenced = 0;
		glGetActiveUniformBlockiv( program, blockIndex, GL_UNIFORM_BLOCK_REFERENCED_BY_VERTEX_SHADER, &uniformblockReferenced);
		CheckGlError("uniformblockReferenced");
		// GL_UNIFORM_BLOCK_REFERENCED_BY_VERTEX_SHADER
 		// GL_UNIFORM_BLOCK_REFERENCED_BY_TESS_CONTROL_SHADER
 		// GL_UNIFORM_BLOCK_REFERENCED_BY_TESS_EVALUATION_SHADER
 		// GL_UNIFORM_BLOCK_REFERENCED_BY_GEOMETRY_SHADER
		// GL_UNIFORM_BLOCK_REFERENCED_BY_FRAGMENT_SHADER
		
		Terminal.LogOpenGL( std::string( "Uniform block " + std::to_string(i) + ": " + uniformName + " of size " +  std::to_string(blockSize) ) );
	}
	
	GLint bindingPoint = 0;
	glUniformBlockBinding( program, blockIndex, bindingPoint );
	CheckGlError("bindingPoint");
	
	GLuint materialBuffer;
	glGenBuffers(1, &materialBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, materialBuffer);
 
	float myFloats[24] =
	{
		0.5, 0.5, 0.5, 1.0,
		0.0, 0.6, 0.3, 1.0,
		0.0, 0.0, 0.0, 1.0,
		0.0, 0.0, 0.0, 1.0,
		0.0, 0.0, 0.0, 1.0,
		0.0, 0.0, 0.0, 1.0
	};
	
	glBufferData(GL_UNIFORM_BUFFER, sizeof(myFloats), myFloats, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, materialBuffer);
}

void LogActiveSubroutines( GLuint program, GLenum shaderType )
{
	int i, j, length, numCompatibleSubroutines, maxSubroutines, maxSubroutineUniforms, activeSubroutines, numActiveSubroutineUniforms;
	char name[256];
 
	glGetIntegerv( GL_MAX_SUBROUTINES, &maxSubroutines );
	glGetIntegerv( GL_MAX_SUBROUTINE_UNIFORM_LOCATIONS, &maxSubroutineUniforms );
	std::string subroutineInfo( "Max Subroutines: " + std::to_string( maxSubroutines ) + "  Max Subroutine Uniforms: " + std::to_string( maxSubroutineUniforms ) );
	Terminal.LogOpenGL( subroutineInfo );
 
	glGetProgramStageiv( program, shaderType, GL_ACTIVE_SUBROUTINES, &activeSubroutines );
	glGetProgramStageiv( program, shaderType, GL_ACTIVE_SUBROUTINE_UNIFORMS, &numActiveSubroutineUniforms );
	Terminal.LogOpenGL( std::string( "Active Subroutines: " + std::to_string( activeSubroutines ) ) );
	Terminal.LogOpenGL( std::string( "Active Subroutine Uniforms: " + std::to_string( numActiveSubroutineUniforms ) ) );
	
	for ( i = 0; i < numActiveSubroutineUniforms; ++i )
	{
		glGetActiveSubroutineUniformName( program, shaderType, i, 256, &length, name );
		
		Terminal.LogOpenGL( std::string( "Subroutine Uniform: " + std::to_string(i) + " name: " + name ) );
		glGetActiveSubroutineUniformiv( program, shaderType, i, GL_NUM_COMPATIBLE_SUBROUTINES, &numCompatibleSubroutines );
		
		int *s = (int*)malloc( sizeof(int) * numCompatibleSubroutines );
		glGetActiveSubroutineUniformiv( program, shaderType, i, GL_COMPATIBLE_SUBROUTINES, s);
		Terminal.LogOpenGL( "Compatible Subroutines:" );
		for ( j=0; j < numCompatibleSubroutines; ++j )
		{
			glGetActiveSubroutineName( program, shaderType, s[j], 256, &length, name);
			Terminal.LogOpenGL( "\t" + std::to_string( s[j] ) + " - " + name );
		}
		Terminal.LogOpenGL( "---------" );
		free(s);
	}

}

GLenum GetGLShaderEnum( GLSLShaderType type, bool separate )
{
	GLenum shader = 0;
	switch (type)
	{
		case GLSLShaderType::VERTEX:
		{
			shader =  separate ? GL_VERTEX_SHADER_BIT : GL_VERTEX_SHADER;
			break;
		}
		case GLSLShaderType::GEOMETRY:
		{
			shader = separate ? GL_GEOMETRY_SHADER_BIT : GL_GEOMETRY_SHADER;
			break;
		}
		case GLSLShaderType::FRAGMENT:
		{
			shader = separate ? GL_FRAGMENT_SHADER_BIT : GL_FRAGMENT_SHADER;
			break;
		}
		case GLSLShaderType::TESS_CONTROL:
		{
			shader = separate ? GL_TESS_CONTROL_SHADER_BIT : GL_TESS_CONTROL_SHADER;
			break;
		}
		case GLSLShaderType::TESS_EVALUATION:
		{
			shader = separate ? GL_TESS_EVALUATION_SHADER_BIT : GL_TESS_EVALUATION_SHADER;
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

// UNIFORM SETTING

// SINGLE VALUE ( float, int, uint, double )
void SetUniform( GLuint program, const char* name, float val )
{
	glUniform1f( glGetUniformLocation( program, name ), val );
}
void SetUniform( GLuint program, const char* name, int val )
{
	glUniform1i( glGetUniformLocation ( program, name ), val );
}
void SetUniform( GLuint program, const char* name, unsigned int val )
{
	glUniform1ui( glGetUniformLocation ( program, name ), val );
}
void SetUniform( GLuint program, const char* name, double val )
{
	glUniform1d( glGetUniformLocation( program, name ), val );
}

// TUPLE VALUES ( float, int, uint )
void SetUniform( GLuint program, const char* name, const glm::vec2& vec )
{
	glUniform2f( glGetUniformLocation( program, name ), vec.x, vec.y );
}
void SetUniform( GLuint program, const char* name, const glm::ivec2& vec )
{
	glUniform2i( glGetUniformLocation( program, name ), vec.x, vec.y );
}
void SetUniform( GLuint program, const char* name, const glm::uvec2& vec )
{
	glUniform2ui( glGetUniformLocation( program, name ), vec.x, vec.y );
}

// TRIPLE VALUES ( float, int, uint )
void SetUniform( GLuint program, const char* name, const glm::vec3& vec )
{
	glUniform3f( glGetUniformLocation( program, name ), vec.x, vec.y, vec.z );
}
void SetUniform( GLuint program, const char* name, const glm::ivec3& vec )
{
	glUniform3i( glGetUniformLocation( program, name ), vec.x, vec.y, vec.z );
}
void SetUniform( GLuint program, const char* name, const glm::uvec3& vec )
{
	glUniform3ui( glGetUniformLocation( program, name ), vec.x, vec.y, vec.z );
}

// QUATER VALUES ( float, int, uint )
void SetUniform( GLuint program, const char* name, const glm::vec4& vec )
{
	glUniform4f( glGetUniformLocation( program, name ), vec.x, vec.y, vec.z, vec.w );
}
void SetUniform( GLuint program, const char* name, const glm::ivec4& vec )
{
	glUniform4i( glGetUniformLocation( program, name ), vec.x, vec.y, vec.z, vec.w );
}
void SetUniform( GLuint program, const char* name, const glm::uvec4& vec )
{
	glUniform4ui( glGetUniformLocation( program, name ), vec.x, vec.y, vec.z, vec.w );
}

// MATRICES ( 2x2f, 3x3f, 4x4f )
void SetUniform( GLuint program, const char* name, const glm::mat2& mat )
{
	glUniformMatrix2fv( glGetUniformLocation( program, name ), 1, GL_FALSE, glm::value_ptr(mat) );
}
void SetUniform( GLuint program, const char* name, const glm::mat3& mat )
{
	glUniformMatrix3fv( glGetUniformLocation( program, name ), 1, GL_FALSE, glm::value_ptr(mat) );
}
void SetUniform( GLuint program, const char* name, const glm::mat4& mat )
{
	glUniformMatrix4fv( glGetUniformLocation( program, name ), 1, GL_FALSE, glm::value_ptr(mat) );
}

// SUBROUTINES

bool HasSubroutines( const GLuint& program, GLenum shaderType )
{
	int subroutines = 0;
	glGetProgramStageiv( program, shaderType, GL_ACTIVE_SUBROUTINE_UNIFORM_LOCATIONS, &subroutines );
	if ( subroutines > 0 )
	{
		return true;
	}
	return false;
}

void SetSubroutineUniformLocations( GLuint program, GLuint shader, Subroutines& routines )
{
	//TODO(Valentinas): Make a proper initialization for shader.subroutines
	int numActiveSubroutineUniforms;
	glGetProgramStageiv( program, shader, GL_ACTIVE_SUBROUTINE_UNIFORM_LOCATIONS, &numActiveSubroutineUniforms );
	if (numActiveSubroutineUniforms > 0)
	{
		routines.numSubroutines = numActiveSubroutineUniforms;
		//TODO(Valentinas): USE NEW INSTEAD OF MALLOC?
		routines.subroutines = (GLuint*)malloc( sizeof(GLuint) * numActiveSubroutineUniforms );
		for ( int i = 0; i < numActiveSubroutineUniforms; ++i )
		{
			int length;
			char name[256];
			// Get subroutine uniform name
			glGetActiveSubroutineUniformName(program, shader, i, 256, &length, name );
			//printf( "%s uniform.\n", name );
			routines.locations.insert( std::pair<std::string, unsigned int>( name, i ) );
			routines.subroutines[i] = 0;
		}
	}
}

unsigned int GetSubroutineUniformLocation( const char* name, const std::map< std::string, unsigned int >& locations )
{
	//TODO(Valentinas): ADD CHECK TO FIND WHETHER LOCATIONS EXISTS
	std::string value(name);
	return locations.at(value);
	//Terminal.LogOpenGL("Subroutine uniform not found.");
}

void SetActiveSubroutine( GLuint program, ShaderObject shader, const char* uniform, const char* routine )
{
	GLuint index = GetSubroutineUniformLocation( uniform, shader.locations );
	shader.subroutines[index] = glGetSubroutineIndex( program, shader.shaderType, routine );
	glUniformSubroutinesuiv( shader.shaderType, shader.numSubroutines, shader.subroutines );
	
	// GL_INVALID_OPERATION is generated if no program object is current.
	// GL_INVALID_VALUE is generated if count is not equal to the value of GL_ACTIVE_SUBROUTINE_UNIFORM_LOCATIONS for the shader stage shadertype of the current program.
	// GL_INVALID_VALUE is generated if any value in indices is greater than or equal to the value of GL_ACTIVE_SUBROUTINES for the shader stage shadertype of the current program.
	// GL_INVALID_ENUM is generated if shadertype is not one of the accepted values.
	CheckGlError("Subroutine");
	
	/*
	 for (int i =0; i<2; ++i)
	 {
		int in = shader.subroutines[i];
		printf("Subroutine: %i -> %i \n", i, in);
	 }
	 */
}

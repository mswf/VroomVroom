#include "material.h"
#include "texture.h"
#include "shader.h"
#include "resource_manager.h"

Material::Material() :
	shader(NULL),
	diffuseTextureId(0),
	specularTextureId(0),
	normalTextureId(0),
	cubemapTextureId(0),
	heightTextureId(0),
	name("No name"),
	wireframe_enabled(false),
	two_sided(true),
	textureCount(0),
	shininess(0.0f),
	shininess_strenght(0.0f),
	opacity(0.0f),
	bump_scaling(0.0f)
{
	SetArray(diffuse,		glm::vec4(0.3f, 0.3f, 0.3f, 1.0f));
	SetArray(ambient,		glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
	SetArray(specular,		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	SetArray(emissive,		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	SetArray(transparent,	glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	SetArray(reflective,	glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

	shader = Assets.GetShaderProgram("Default");
}

void Material::SetArray(float* array, glm::vec4 values)
{
	array[0] = values.r;
	array[1] = values.g;
	array[2] = values.b;
	array[3] = values.a;
}

void Material::SetDiffuseTexture( const char* name )
{
	diffuseTextureId = Assets.GetImageId( name );
}

void Material::SetNormalTexture( const char* name )
{
	normalTextureId = Assets.GetImageId( name );
}

void Material::SetSpecularTexture( const char* name )
{
	specularTextureId = Assets.GetImageId( name );
}

void Material::SetCubeMapTexture( const char* name )
{
	cubemapTextureId = Assets.GetImageId( name );
}

void Material::SetHeightMapTexture( const char* name )
{
	heightTextureId = Assets.GetImageId( name );
}

void Material::UseMaterial() const
{
	UseProgram( shader->program );
}

// TODO(Valentinas): Needs more work on this stage
void Material::SetUniforms() const
{
	//LogActiveUniformBlocks( shader->program );
	//LogActiveUniforms( shader->program );
	/*
	std::vector<ShaderObject*>::const_iterator it = shader->shaders.begin();
	std::vector<ShaderObject*>::const_iterator end = shader->shaders.end();
	for ( ; it != end; ++it )
	{
		std::vector< Uniform >::const_iterator u = (*it)->uniforms.begin();
		std::vector< Uniform >::const_iterator ue = (*it)->uniforms.end();
		for ( ; u != ue; ++u )
		{
			printf( "Setting uniform %s \n", (*u).name.c_str() );
			SetShaderUniform( (*u) );
		}
	}
	*/
	//SetActiveSubroutine( shader->program, shader->shaders[0]->shaderType, &shader->shaders[0]->subroutine, "shadeModel", "phongModel");
}

void Material::SetAmbientColor( glm::vec4 colour )
{
	SetArray(ambient, colour);
}

void Material::SetDiffuseColor( glm::vec4 colour )
{
	SetArray(diffuse, colour);
}

void Material::SetSpecularColor( glm::vec4 colour )
{
	SetArray(specular, colour);
}

void Material::SetEmissiveColor( glm::vec4 colour )
{
	SetArray(emissive, colour);
}

void Material::SetTransparentColor( glm::vec4 colour )
{
	SetArray(transparent, colour);
}

void Material::SetReflectiveColor( glm::vec4 colour )
{
	SetArray(reflective, colour);
}

const glm::vec4 Material::GetAmbientColor() const
{
	return glm::vec4( ambient[0], ambient[1], ambient[2], ambient[3] );
}

const glm::vec4 Material::GetDiffuseColor() const
{
	return glm::vec4( diffuse[0], diffuse[1], diffuse[2], diffuse[3] );
}

const glm::vec4 Material::GetSpecularColor() const
{
	return glm::vec4( specular[0], specular[1], specular[2], specular[3] );
}

const glm::vec4 Material::GetEmissiveColor() const
{
	return glm::vec4( emissive[0], emissive[1], emissive[2], emissive[3] );
}

const glm::vec4 Material::GetTransparentColor() const
{
	return glm::vec4( transparent[0], transparent[1], transparent[2], transparent[3] );
}

const glm::vec4 Material::GetReflectiveColor() const
{
	return glm::vec4( reflective[0], reflective[1], reflective[2], reflective[3] );
}

void Material::SetShaderUniform( Uniform u ) const
{
	const char* name = u.name.c_str();
	unsigned int p = shader->program;
	switch (u.type)
	{
		case GL_FLOAT:
		{
			float value = *( (float*)u.data );
			SetUniform( p, name, value );
		}
		case GL_FLOAT_VEC2:
		{
//			float x = ( (float*)u.data )[0];
//			float y = ( (float*)u.data )[1];
//			glm::vec2( x, y );
			glm::vec2 value = *( (glm::vec2*)u.data );
			SetUniform( p, name, value );
		}
		case GL_FLOAT_VEC3:
		{
//			float x = ( (float*)u.data )[0];
//			float y = ( (float*)u.data )[1];
//			float z = ( (float*)u.data )[2];
//			glm::vec3( x, y, z );
			glm::vec3 value = *( (glm::vec3*)u.data );
			SetUniform( p, name, value );
		}
		case GL_FLOAT_VEC4:
		{
//			float x = ( (float*)u.data )[0];
//			float y = ( (float*)u.data )[1];
//			float z = ( (float*)u.data )[2];
//			float w = ( (float*)u.data )[3];
//			glm::vec4( x, y, z, w );
			glm::vec4 value = *( (glm::vec4*)u.data );
			SetUniform( p, name, value );
		}
		case GL_DOUBLE:
		{
			double value = *( (double*)u.data );
			SetUniform( p, name, value );
		}
		case GL_INT:
		{
			int value = *( (int*)u.data );
			SetUniform( p, name, value );
		}
		case GL_INT_VEC2:
		{
//			int x = ( (int*)u.data )[0];
//			int y = ( (int*)u.data )[1];
//			glm::ivec2( x, y );
			glm::ivec2 value = *( (glm::ivec2*)u.data );
			SetUniform( p, name, value );
		}
		case GL_INT_VEC3:
		{
//			int x = ( (int*)u.data )[0];
//			int y = ( (int*)u.data )[1];
//			int z = ( (int*)u.data )[2];
//			glm::ivec3( x, y, z );
			glm::ivec3 value = *( (glm::ivec3*)u.data );
			SetUniform( p, name, value );
		}
		case GL_INT_VEC4:
		{
//			int x = ( (int*)u.data )[0];
//			int y = ( (int*)u.data )[1];
//			int z = ( (int*)u.data )[2];
//			int w = ( (int*)u.data )[3];
//			glm::ivec4( x, y, z, w );
			glm::ivec4 value = *( (glm::ivec4*)u.data );
			SetUniform( p, name, value );
		}
		case GL_UNSIGNED_INT:
		{
			unsigned int value = *( (unsigned int*)u.data );
			SetUniform( p, name, value );
		}
		case GL_UNSIGNED_INT_VEC2:
		{
//			unsigned int x = ( (unsigned int*)u.data )[0];
//			unsigned int y = ( (unsigned int*)u.data )[1];
//			glm::uvec2( x, y );
			glm::uvec2 value = *( (glm::uvec2*)u.data );
			SetUniform( p, name, value );
		}
		case GL_UNSIGNED_INT_VEC3:
		{
//			unsigned int x = ( (unsigned int*)u.data )[0];
//			unsigned int y = ( (unsigned int*)u.data )[1];
//			unsigned int z = ( (unsigned int*)u.data )[2];
//			glm::uvec3( x, y, z );
			glm::uvec3 value = *( (glm::uvec3*)u.data );
			SetUniform( p, name, value );
		}
		case GL_UNSIGNED_INT_VEC4:
		{
//			unsigned int x = ( (unsigned int*)u.data )[0];
//			unsigned int y = ( (unsigned int*)u.data )[1];
//			unsigned int z = ( (unsigned int*)u.data )[2];
//			unsigned int w = ( (unsigned int*)u.data )[3];
//			glm::uvec4( x, y, z, w );
			glm::uvec4 value = *( (glm::uvec4*)u.data );
			SetUniform( p, name, value );
		}
		//case GL_FLOAT_MAT2:			{ return "mat2";}
		//case GL_FLOAT_MAT3:			{ return "mat3";}
		case GL_FLOAT_MAT4:
		{
			glm::mat4 matrix = *( (glm::mat4*)u.data );
			SetUniform( p, name, matrix );
		}
		//case GL_SAMPLER_1D: 		{ return "sampler1D"; }
		//case GL_SAMPLER_2D: 		{ return "sampler2D"; }
		//case GL_SAMPLER_3D: 		{ return "sampler3D"; }
		//case GL_SAMPLER_CUBE: 		{ return "samplerCube"; }
		//case GL_SAMPLER_1D_SHADOW: 	{ return "sampler1DShadow"; }
		//case GL_SAMPLER_2D_SHADOW: 	{ return "sampler2DShadow"; }
		//default: 					{ return "Not supported"; }
	}
}

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
	wireframe_enabled(0),
	two_sided(0),
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

	shader = ResourceManager::getInstance().GetShaderProgram("Default");
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
	diffuseTextureId = ResourceManager::getInstance().GetImageId( name );
}

void Material::SetNormalTexture( const char* name )
{
	normalTextureId = ResourceManager::getInstance().GetImageId( name );
}

void Material::SetSpecularTexture( const char* name )
{
	specularTextureId = ResourceManager::getInstance().GetImageId( name );
}

void Material::SetCubeMapTexture( const char* name )
{
	cubemapTextureId = ResourceManager::getInstance().GetImageId( name );
}

void Material::SetHeightMapTexture( const char* name )
{
	heightTextureId = ResourceManager::getInstance().GetImageId( name );
}

void Material::SetShader( ShaderProgram* s )
{
	shader = s;
}

void Material::UseMaterial() const
{
	UseProgram( shader->program );
}

void Material::SetUniforms()
{

}

void Material::SetShininess( float sh )
{
	shininess = sh;
}

void Material::SetShininessStrength( float str )
{
	shininess_strenght = str;
}

void Material::SetOpacity( float op )
{
	opacity = op;
}

void Material::SetBumpScaling( float bump )
{
	bump_scaling = bump;
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

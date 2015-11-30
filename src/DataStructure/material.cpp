#include "material.h"
#include "texture.h"
#include "shader.h"
#include "resource_manager.h"

Material::Material() :
	shader(NULL),
	diffuseTextureId(0),
	specularTextureId(0),
	normalTextureId(0),
	name("No name"),
	wireframe_enabled(0),
	two_sided(0),
	textureCount(0),
	shininess(0.0f),
	shininess_strenght(0.0f),
	opacity(0.0f),
	bump_scaling(0.0f)
	//diffuse { 0.3f, 0.3f, 0.3f, 1.0f },
	//ambient { 0.2f, 0.2f, 0.2f, 1.0f },
	//specular { 0.0f, 0.0f, 0.0f, 1.0f },
	//emissive { 0.0f, 0.0f, 0.0f, 1.0f },
	//transparent { 0.0f, 0.0f, 0.0f, 1.0f },
	//reflective { 0.0f, 0.0f, 0.0f, 1.0f }
{
	shader = ResourceManager::getInstance().GetShaderProgram("Default");
}

void Material::SetDiffuseTexture( const char* name )
{
	diffuseTextureId = ResourceManager::getInstance().GetImageId( name );
}

void Material::SetNormalTexture( const char* name )
{
	normalTextureId = ResourceManager::getInstance().GetImageId( name );
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
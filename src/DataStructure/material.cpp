#include "material.h"
#include "texture.h"
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
	bump_scaling(0.0f),
	diffuse { 0.3f, 0.3f, 0.3f, 1.0f },
	ambient { 0.2f, 0.2f, 0.2f, 1.0f },
	specular { 0.0f, 0.0f, 0.0f, 1.0f },
	emissive { 0.0f, 0.0f, 0.0f, 1.0f },
	transparent { 0.0f, 0.0f, 0.0f, 1.0f },
	reflective { 0.0f, 0.0f, 0.0f, 1.0f }
{
	shader = new Shader();
}

void Material::SetDiffuseTexture( const char* name )
{
	diffuseTextureId = ResourceManager::getInstance().GetImageId( name );
}

void Material::SetShader( Shader* s )
{
	shader = s;
}

void Material::UseMaterial()
{
	shader->UseProgram();
}

void Material::SetUniforms()
{
	
}

// create material uniform buffer
//glGenBuffers(1,&(aMesh.uniformBlockIndex));
//glBindBuffer(GL_UNIFORM_BUFFER,aMesh.uniformBlockIndex);
//glBufferData(GL_UNIFORM_BUFFER, sizeof(aMat), (void *)(&aMat), GL_STATIC_DRAW);
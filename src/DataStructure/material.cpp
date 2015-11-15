#include "material.h"

Material::Material( Shader* shader ) :
	shader(shader)
{
	
}

Material::~Material()
{
	
}

void Material::SetShader( Shader* sh )
{
	shader = sh;
}

void Material::SetDiffuse( unsigned int tex )
{
	diff_texture = tex;
}

void Material::SetNormal( unsigned int tex )
{
	normal_texture = tex;
}
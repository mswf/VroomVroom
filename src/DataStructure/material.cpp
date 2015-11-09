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
#include "cMaterial.h"

const int CMaterial::familyId = (int)ComponentTypes::MATERIAL;

CMaterial::CMaterial( Shader* shader ) :
	shader(shader)
{
	
}

CMaterial::~CMaterial()
{
	
}
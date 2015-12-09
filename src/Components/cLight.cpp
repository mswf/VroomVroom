#include "cLight.h"

const int CLight::familyId = (int)ComponentTypes::LIGHT;
std::vector< CLight* > CLight::list;

CLight::CLight()
{
	list.push_back( this );
}

CLight::~CLight()
{
	
}

void CLight::Call()
{
	
}

#include "cLight.h"
#include "entity.h"

const int CLight::familyId = static_cast<int>(ComponentTypes::LIGHT);
std::vector< CLight* > CLight::list;

CLight::CLight():
	intensity(1),
	colour(glm::vec4(1))
{
	list.push_back( this );
}

CLight::~CLight()
{
	
}

void CLight::Call()
{
	
}

void CLight::SetIntensity( float i )
{
	intensity = i;
}

void CLight::SetColour( glm::vec4 c )
{
	colour = c;
}

glm::vec4 CLight::GetColour()
{
	return colour;
}

float CLight::GetIntensity()
{
	return intensity;
}

glm::vec3 CLight::GetDirection()
{
	return glm::eulerAngles(entity->transform->GetRotation());
}

glm::vec3 CLight::GetPosition()
{
	return entity->transform->GetPosition();
}

std::vector< CLight* >* CLight::GetLightList()
{
	return &list;
}
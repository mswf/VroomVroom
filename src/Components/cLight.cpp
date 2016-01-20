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

glm::vec4 CLight::GetColour() const
{
	return colour;
}

float CLight::GetIntensity() const
{
	return intensity;
}

glm::vec3 CLight::GetDirection() const
{
	return glm::eulerAngles(entity->transform->GetRotation());
}

glm::vec3 CLight::GetPosition() const
{
	return entity->transform->GetPosition();
}

std::vector< CLight* >* CLight::GetLightList()
{
	return &list;
}
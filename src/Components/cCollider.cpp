#include "cCollider.h"
#include "entity.h"
#include "SDL2/SDL_assert.h"

const int CCollider::familyId = (int)ComponentTypes::COLLIDER;
std::vector< CCollider* > CCollider::list;

CCollider::CCollider() :
	collisionType(CollisionType::NONE),
	radius(0)
{
	list.push_back( this );
}

CCollider::~CCollider()
{

}

void CCollider::Call()
{

}

//make private, call from the Call() function?
bool CCollider::SphereToSphere(const CCollider* other) const
{
	if (collisionType != CollisionType::SPHERE || other->collisionType != CollisionType::SPHERE)
	{
		SDL_assert(false);
	}

	//should be global position
	glm::vec3 position1 = entity->transform->GetPosition();
	glm::vec3 position2 = other->entity->transform->GetPosition();
	float dist = glm::distance(position1, position2);
	if (dist > radius + other->radius)
	{
		return false;
	}
	return true;
}

void CCollider::SetRadius(float radius)
{
	this->radius = radius;
}

float CCollider::GetRadius() const
{
	return radius;
}
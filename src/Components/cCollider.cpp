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
	float distSqrd = glm::dot(position1, position1) + glm::dot(position2, position2);
	float radiusTotal = radius + other->radius;
	if (distSqrd > radiusTotal * radiusTotal)
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

void CCollider::SetCollisionBox(float width, float height, float length)
{
	this->width = width;
	this->height = height;
	this->length = length;
}

bool CCollider::BoxToBox(const CCollider* other) const
{
	//        _________
	//       /|       /|
	//      / |      / |
	//     /__|_____/_ |
	//    |  /      |  /
	//    | /       | /
	//    |/________|/


	glm::vec3 position1 = entity->transform->GetPosition();
	glm::vec3 position2 = other->entity->transform->GetPosition();

	// multiply .5f because position is center, if should be corner, go fix by removing multiplication
	if (glm::abs(position1.x - position2.x) < (width + other->width) * .5f)
	{
		if (glm::abs(position1.y - position2.y) < (height + other->height) * .5f)
		{
			if (glm::abs(position1.z - position2.z) < (length + other->length) * .5f)
			{
				return true;
			}
		}
	}

	return false;
}

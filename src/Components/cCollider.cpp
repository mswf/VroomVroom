#include "cCollider.h"
#include "entity.h"
#include "SDL2/SDL_assert.h"

const int CCollider::familyId = (int)ComponentTypes::COLLIDER;
std::vector< CCollider* > CCollider::list;

CCollider::CCollider()
{
	list.push_back( this );
}

CCollider::~CCollider()
{

}

void CCollider::Call()
{
}
/*


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

glm::vec3 CCollider::GetCollisionBox() const
{
	return glm::vec3(width, height, length);
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

	if (this->collisionType != CollisionType::BOX || other->collisionType != CollisionType::BOX)
	{
		SDL_assert(false);
		return false;
	}


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
}*/
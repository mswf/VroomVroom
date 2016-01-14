#include "sphereCollider.h"
#include "boxCollider.h"
#include "../glm/vec3.hpp"
#include "../Components/entity.h"

void SphereCollider::SetRadius(float radius)
{
	this->radius = radius;
}

float SphereCollider::GetRadius() const
{
	return radius;
}

bool SphereCollider::Collide(const BoxCollider& other) const
{
	glm::vec3 boxMin, boxMax, sphereCenter;
	glm::vec3 boxCenter = other.entity->transform->GetPosition();
	sphereCenter = entity->transform->GetPosition();
	float radiusSquared = radius * radius;

	boxMin = boxCenter - other.GetCollisionBox() * 0.5f;
	boxMax = boxCenter + other.GetCollisionBox() * 0.5f;

	float minDistance = 0;
	int i;
	for (i = 0; i < 3; ++i)
	{
		if (sphereCenter[i] < boxMin[i])
		{
			minDistance += (sphereCenter[i] - boxMin[i]) * (sphereCenter[i] - boxMin[i]);
		}
		else if (sphereCenter[i] > boxMax[i])
		{
			minDistance += (sphereCenter[i] - boxMax[i]) * (sphereCenter[i] - boxMax[i]);
		}
	}
	return minDistance <= radiusSquared;
}

bool SphereCollider::Collide(const SphereCollider& other) const
{
	//should be global position
	glm::vec3 position1 = entity->transform->GetPosition();
	glm::vec3 position2 = other.entity->transform->GetPosition();
	float distSqrd = glm::dot(position1, position1) + glm::dot(position2, position2);
	float radiusTotal = radius + other.radius;
	if (distSqrd > radiusTotal * radiusTotal)
	{
		return false;
	}
	return true;
}

bool SphereCollider::Collide(const CCollider& other) const
{
	return other.Collide(*this);
}

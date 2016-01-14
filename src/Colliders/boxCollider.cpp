#include "boxCollider.h"
#include "sphereCollider.h"
#include "../Components/entity.h"

void BoxCollider::SetCollisionBox(float width, float height, float length)
{
	SetCollisionBox(glm::vec3(width, height, length));
}

void BoxCollider::SetCollisionBox(const glm::vec3& size)
{
	this->bounds = size;
}

const glm::vec3& BoxCollider::GetCollisionBox() const
{
	return bounds;
}

bool BoxCollider::Collide(const BoxCollider& other) const
{
	printf("box with box\n");
	return 0;
}

bool BoxCollider::Collide(const SphereCollider& other) const
{
	glm::vec3 boxMin, boxMax, sphereCenter;
	glm::vec3 boxCenter = entity->transform->GetPosition();
	sphereCenter = other.entity->transform->GetPosition();
	float radiusSquared = other.GetRadius() * other.GetRadius();

	boxMin = boxCenter - bounds * 0.5f;
	boxMax = boxCenter + bounds * 0.5f;

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

bool BoxCollider::Collide(const CCollider& other) const
{
	return other.Collide(*this);
}

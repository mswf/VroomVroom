#ifndef cCollider_h
#define cCollider_h

#include "component.h"
#include <vector>
#include "../glm/vec3.hpp"

enum class CollisionType
{
	SPHERE,
	BOX,
	NONE
};

class CCollider : public Component
{
	public:

		static const int familyId;

		CCollider();
		~CCollider();

		void Call();
		bool SphereToSphere(const CCollider* other) const;
		void SetRadius(float radius);
		float GetRadius() const;
		void SetCollisionBox(float width, float height, float length);
		glm::vec3 GetCollisionBox() const;
		bool SphereToBox(const CCollider* other) const;
		bool BoxToBox(const CCollider* other) const;
		void SetCollisionType(CollisionType type);
	private:
		static std::vector< CCollider* > list;
		CollisionType collisionType;
		float radius;
		float width, height, length;
};


#endif /* cCollider_h */

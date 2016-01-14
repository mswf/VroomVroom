#ifndef BoxCollider_h
#define BoxCollider_h

#include "../Components/cCollider.h"
#include "../glm/vec3.hpp"

class SphereCollider;
class BoxCollider : public CCollider
{
	public:
		void SetCollisionBox(float width, float height, float length);
		void SetCollisionBox(const glm::vec3& size);
		const glm::vec3& GetCollisionBox() const;

		bool Collide(const BoxCollider& other) const override;
		bool Collide(const SphereCollider& other) const override;
		bool Collide(const CCollider& other) const override;
	private:
		glm::vec3 bounds;
};


#endif

#ifndef SphereCollider_h
#define SphereCollider_h

#include "../Components/cCollider.h"


class BoxCollider;
class SphereCollider : public CCollider
{
	public:
		void SetRadius(float radius);
		float GetRadius() const;

		bool Collide(const BoxCollider& other) const override;
		bool Collide(const SphereCollider& other) const override;
		bool Collide(const CCollider& other) const override;
		//bool Collide(const CCollider& other) const = 0;

	private:
		float radius;
};



#endif /* SphereCollider_h */

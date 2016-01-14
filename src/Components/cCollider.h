#ifndef cCollider_h
#define cCollider_h

#include "component.h"
#include <vector>

class SphereCollider;
class BoxCollider;

class CCollider : public Component
{
	public:

		static const int familyId;

		CCollider();
		~CCollider();
		void Call();

		virtual bool Collide(const BoxCollider& other) const = 0;
		virtual bool Collide(const SphereCollider& other) const = 0;
		virtual bool Collide(const CCollider& other) const = 0;

	private:
		static std::vector< CCollider* > list;
};


#endif /* cCollider_h */

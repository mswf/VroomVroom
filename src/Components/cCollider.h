#ifndef cCollider_h
#define cCollider_h

#include "component.h"
#include <vector>
#include "../glm/vec3.hpp"

class CCollider : public Component
{
public:
	
	static const int familyId;
	
	CCollider();
	~CCollider();
	
	void Call();
	
private:
	
	static std::vector< CCollider* > list;
};


#endif /* cCollider_h */

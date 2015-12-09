#ifndef cLight_h
#define cLight_h

#include "component.h"
#include <vector>
#include "../glm/vec3.hpp"

class CLight : public Component
{
	public:
	
		static const int familyId;
		
		CLight();
		~CLight();
		
		void Call();

	private:

		static std::vector< CLight* > list;
};

#endif /* cLight_h */

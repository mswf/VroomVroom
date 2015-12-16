#ifndef cLight_h
#define cLight_h

#include "component.h"
#include <vector>
#include "../glm/vec3.hpp"
#include "../glm/vec4.hpp"

class CLight : public Component
{
	public:
	
		static const int familyId;
		
		CLight();
		~CLight();
		
		void Call();
		glm::vec3 GetDirection();
		glm::vec3 GetPosition();
		void SetIntensity( float intensity );
		void SetColour( glm::vec4 colour );
		glm::vec4 GetColour();
		float GetIntensity();
		static std::vector< CLight* >* GetLightList();

	private:

		float intensity;
		glm::vec4 colour;
		static std::vector< CLight* > list;
};

#endif /* cLight_h */

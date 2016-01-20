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
		
		void Call() override;
		glm::vec3 GetDirection() const;
		glm::vec3 GetPosition() const;
		void SetIntensity( float intensity );
		void SetColour( glm::vec4 colour );
		glm::vec4 GetColour() const;
		float GetIntensity() const;
		static std::vector< CLight* >* GetLightList();

	private:

		float intensity;
		glm::vec4 colour;
		static std::vector< CLight* > list;
};

#endif /* cLight_h */

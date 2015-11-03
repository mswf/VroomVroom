#ifndef cTransform_h
#define cTransform_h

#include "component.h"
#include <vector>
#include "../glm/mat4x4.hpp"
#include "../glm/vec3.hpp"
#include "../glm/gtc/quaternion.hpp"
#include "../glm/gtx/transform.hpp"


class CTransform : public Component
{
    public:
        static const int familyId;
	
        CTransform( CTransform* parent = nullptr );
        ~CTransform();
	
		glm::mat4 transform;
	
		std::vector<CTransform* > GetChildren();
		CTransform* GetParent();
		void SetParent( CTransform* newParent );
	
		void Rotate( glm::vec3 rotate );
		void Translate( glm::vec3 translation );
		void Scale( glm::vec3 scale );
		// POSITION FUNCTIONALITY
	
		glm::vec3 GetPosition() const;
		void SetPosition( glm::vec3 position );
	
		glm::vec3 GetLocalPosition() const;
		void SetLocalPosition( glm::vec3 position );

		// ROTATION FUNCTIONALITY
		glm::vec3 GetRotation() const;
		void SetRotation( glm::vec3 rotation );
	
		glm::vec3 GetLocalRotation() const;
		void SetLocalRotation( glm::vec3 rotation );
	
		float GetPitch() const;
		float GetYaw() const;
		float GetRoll() const;
	
		void SetPitch( float angle );
		void SetYaw( float angle );
		void SetRoll( float angle );
	
		void Pitch( float angle ); // Pitch is the X axis
		void Yaw( float angle ); // Yaw is the Y axis
		void Roll( float angle ); // Roll is the Z axis
	
		// SCALE FUNCTIONALITY
	
		glm::vec3 GetScale() const;
		void SetScale( glm::vec3 scale );
	
		glm::vec3 GetLocalScale() const;
		void SetLocalScale( glm::vec3 scale );
	
	
    private:
		void SetRotationMatrix( glm::mat4 rot );
	
		CTransform* root;
		CTransform* parent;
		std::vector<CTransform* > children;
	

};


#endif /* cTransform_h */

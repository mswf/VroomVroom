#ifndef cTransform_h
#define cTransform_h

#include "component.h"
#include "../glm/mat4x4.hpp"
#include "../glm/vec3.hpp"

class CTransform : public Component
{
    public:
        static const int familyId;
	
        CTransform();
        ~CTransform();
	
		void Call();
	
		const glm::mat4& GetTransform() const;
		const glm::mat4& GetWorldTransform() const;
	
		void SetTransform( const glm::mat4& transform );
		void SetWorldTransform( const glm::mat4& transform );
	
		void Translate( const glm::vec3& translation );
		void Rotate( const glm::vec3& rotate, const float& angle );
		void Scale( const glm::vec3& scale );
	
		// POSITION
		glm::vec3 GetPosition() const;
		void SetPosition( const glm::vec3& position );

		const float GetPositionX() const;
		const float GetPositionY() const;
		const float GetPositionZ() const;
	
		void SetPositionX( const float& x );
		void SetPositionY( const float& y );
		void SetPositionZ( const float& z );
	
		void TranslateX( const float& x );
		void TranslateY( const float& y );
		void TranslateZ( const float& z );
	
		// ROTATION
		glm::vec3 GetRotation() const;
		void SetRotation( const glm::vec3& rotation );
	
		const float GetPitch() const;
		const float GetYaw() const;
		const float GetRoll() const;
	
		void SetPitch( const float& angle );
		void SetYaw( const float& angle );
		void SetRoll( const float& angle );
	
		void Pitch( const float& angle ); // Pitch is the X axis
		void Yaw( const float& angle ); // Yaw is the Y axis
		void Roll( const float& angle ); // Roll is the Z axis
	
		// SCALE
		glm::vec3 GetScale() const;
		void SetScale( const glm::vec3& scale );
	
		const float GetScaleX() const;
		const float GetScaleY() const;
		const float GetScaleZ() const;
	
		void SetScaleX( const float& x );
		void SetScaleY( const float& y );
		void SetScaleZ( const float& z );
	
		void ScaleX( const float& x );
		void ScaleY( const float& y );
		void ScaleZ( const float& z );
	
	
    private:
	
		void Update();
		void SetRotationMatrix( const glm::mat4& rot );
		glm::mat4 transform;
		glm::mat4 worldTransform;

};


#endif /* cTransform_h */

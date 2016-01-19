#ifndef cTransform_h
#define cTransform_h

#include "component.h"
#include <vector>
#include "../glm/gtc/quaternion.hpp"
#include "../glm/mat4x4.hpp"
//#include "../glm/vec3.hpp"

class CTransform : public Component
{
    public:
        static const int familyId;
	
        CTransform();
        ~CTransform();

	void Call() override;

		const glm::mat4& GetTransform();
		void SetTransform( const glm::mat4& transform );
	
		void LookAt( const glm::vec3& target, const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f) );
	
		void AddChild( CTransform* c );
		void RemoveChild( CTransform* c );
	
		const CTransform* GetParent() const;
		std::vector< CTransform* >& GetChildren();
	
		const glm::vec3 VectorRight() const;
		const glm::vec3 VectorUp() const;
		const glm::vec3 VectorForward() const;
	
		// POSITION
	
		void Translate( const glm::vec3& translation );
		void Translate( const float& x, const float& y, const float& z );
	
		void TranslateX( const float& x );
		void TranslateY( const float& y );
		void TranslateZ( const float& z );
	
		glm::vec3 GetPosition();
		glm::vec3 GetLocalPosition();
	
		const float GetPositionX() const;
		const float GetPositionY() const;
		const float GetPositionZ() const;
	
		void SetPosition( const glm::vec3& position );
		void SetPosition( const float& x, const float& y, const float& z );

		void SetPositionX( const float& x );
		void SetPositionY( const float& y );
		void SetPositionZ( const float& z );
	
		// ROTATION
		void Rotate( const float& angle, const glm::vec3& rotate );
		const glm::vec3 GetEulerAngles() const;
		const glm::quat GetRotation() const;
	
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
		void Scale( const glm::vec3& scale );
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
	
		glm::mat4 transform;
	
		glm::quat rotation;
		glm::vec3 eulerRotation;
		glm::vec3 position;
		glm::vec3 scale;
	
		bool update;
	
		CTransform* parent;
		std::vector< CTransform* > children;

};


#endif /* cTransform_h */

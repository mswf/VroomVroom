#ifndef cCamera_h
#define cCamera_h

#include "component.h"
#include "../glm/vec3.hpp"
#include "../glm/mat4x4.hpp"
#include "../glm/gtc/matrix_transform.hpp"

enum class Projection
{
	ORTHOGRAPHIC,
	PERSPECTIVE
};

class CCamera : public Component
{
	public:
	
		static const int familyId;
	
		CCamera( Projection proj, float aspectRatio, float near = 0.1f, float far = 1000.0f, float fov = 90.0f );
		~CCamera();
		void Call();
		void SetProjectionType( Projection type );
		void SetAspectRatio( float ratio );
		void SetFOV( float value );
		void SetNearPlaneDistance( float zNear );
		void SetFarPlaneDistance( float zFar );
		const glm::mat4& GetViewMatrix() const;
		const glm::mat4& GetProjectionMatrix() const;
	
	private:
	
		void UpdateView( const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up );
	
		Projection type;
		glm::mat4 projectionMatrix;
		glm::mat4 viewMatrix;
		glm::vec3 upVector;
		float aspectRatio;
		float fieldOfView;
		float zNear;
		float zFar;
	
};

#endif /* cCamera_h */

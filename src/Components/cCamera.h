#ifndef cCamera_h
#define cCamera_h

#include "component.h"
#include "../glm/vec3.hpp"
#include "../glm/mat4x4.hpp"
#include "../glm/gtc/matrix_transform.hpp"

enum class Projection
{
	ORTHOGONAL,
	PERSPECTIVE
};

class CCamera : public Component
{
	public:
	
		static const int familyId;
	
		CCamera( Projection proj, float fov, float aspectRatio, float near, float far );
		~CCamera();
		void Call();
		void SetProjectionType( Projection type );
		void SetUpVector( glm::vec3 up );
		void SetEyeVector( glm::vec3 eye );
		void SetTargetVector( glm::vec3 center );
		const glm::mat4& GetViewMatrix() const;
		const glm::mat4& GetProjectionMatrix() const;
	
	private:
	
		void UpdateView();
	
		Projection type;
		glm::mat4 projectionMatrix;
		glm::mat4 viewMatrix;
		glm::vec3 eyeVector;
		glm::vec3 centerVector;
		glm::vec3 upVector;
		float fieldOfView;
		float aspectRatio;
		float zNear;
		float zFar;
	
};

#endif /* cCamera_h */

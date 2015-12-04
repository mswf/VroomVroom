#include "cCamera.h"
#include "entity.h"

const int CCamera::familyId = (int)ComponentTypes::CAMERA;

CCamera::CCamera( Projection proj, float fov, float aspectRatio, float near, float far )
:	type( proj ),
	fieldOfView( fov ),
	aspectRatio( aspectRatio ),
	zNear( near ),
	zFar( far ),
	projectionMatrix( glm::mat4(1) ),
	viewMatrix( glm::mat4(1) ),
	eyeVector( glm::vec3(1) ),
	centerVector( glm::vec3(0) ),
	upVector( glm::vec3(0.0f, 1.0f, 0.0f) )
{
	SetProjectionType( proj );
	//UpdateView( glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3( 0.0f, 1.0f, 0.0f ) );
}

void CCamera::SetProjectionType( Projection type )
{
	switch (type)
	{
		case Projection::ORTHOGRAPHIC:
		{
			projectionMatrix = glm::ortho( -aspectRatio, aspectRatio, -aspectRatio, aspectRatio, zNear, zFar );
			centerVector = glm::vec3(-1);
			break;
		}
		case Projection::PERSPECTIVE:
		{
			projectionMatrix = glm::perspective( glm::radians(fieldOfView), aspectRatio, zNear, zFar );
			break;
		}
		default:
		{
			assert(false);
			break;
		}
	}

}

void CCamera::Call()
{
	glm::vec3 eye = entity->transform->GetPosition();
	// Needs a debug camera for testing purposes
	//glm::vec3 direction = eye + entity->transform->GetRotation(); //centerVector;
	glm::vec3 direction = entity->transform->GetRotation();
	UpdateView( eye, direction, upVector );
}

void CCamera::SetUpVector( glm::vec3 up )
{
	upVector = up;
	Call();
}

void CCamera::SetEyeVector( glm::vec3 eye )
{
	eyeVector = eye;
	Call();
}

// Center vector should be the rotation vector
void CCamera::SetTargetVector( glm::vec3 center )
{
	centerVector = center;
	Call();
}

void CCamera::UpdateView( const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up )
{
	// To simulate a circular aperture, we move around the camera in a circle in the plane perpendicular to the direction we are looking at. We can easily get two vectors describing the plane using cross products.
	//	glm::vec3 right = glm::normalize(glm::cross(object - eye, up));
	//	glm::vec3 p_up = glm::normalize(glm::cross(object - eye, right));
	// int n = 10; // number of light rays
	// glm::vec3 bokeh = right * cosf(i * 2 * M_PI / n) + p_up * sinf(i * 2 * M_PI / n);
	// float aperture = 0.05;
	viewMatrix = glm::lookAt( eye, center, up ); // Eye + Apeture + Bokeh
}

const glm::mat4& CCamera::GetViewMatrix() const
{
	return viewMatrix;
}

const glm::mat4& CCamera::GetProjectionMatrix() const
{
	return projectionMatrix;
}
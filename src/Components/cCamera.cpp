#include "cCamera.h"
#include "entity.h"

const int CCamera::familyId = (int)ComponentTypes::CAMERA;

CCamera::CCamera( Projection proj, float aspectRatio, float near, float far, float fov )
:	type( proj ),
	fieldOfView( fov ),
	aspectRatio( aspectRatio ),
	zNear( near ),
	zFar( far ),
	projectionMatrix( glm::mat4(1) ),
	viewMatrix( glm::mat4(1) )
{
	SetProjectionType( proj );
}

void CCamera::SetProjectionType( Projection type )
{
	switch (type)
	{
		case Projection::ORTHOGRAPHIC:
		{
			projectionMatrix = glm::ortho( -aspectRatio, aspectRatio, -aspectRatio, aspectRatio, zNear, zFar );
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
	/*
		// To simulate a circular aperture, we move around the camera in a circle in the plane perpendicular to the direction we are looking at. We can easily get two vectors describing the plane using cross products.
		glm::vec3 right = glm::normalize(glm::cross(object - eye, up));
		glm::vec3 p_up = glm::normalize(glm::cross(object - eye, right));
		int n = 10; // number of light rays
		glm::vec3 bokeh = right * cosf(i * 2 * M_PI / n) + p_up * sinf(i * 2 * M_PI / n);
		float aperture = 0.05;
	*/
	
	// Needs a debug camera for testing purposes
	glm::vec3 eye = entity->transform->GetPosition();
	glm::vec3 target = entity->transform->GetRotation() + eye;
	glm::vec3 direction = glm::normalize(target);
	
	// Eye + Apeture + Bokeh
	entity->transform->SetWorldTransform( glm::inverse( glm::lookAt( eye , direction, glm::vec3(0.0, 1.0, 0.0) ) ) );
	viewMatrix = entity->GetTransform();
	//return glm::inverse( entity->GetTransform() );
}

void CCamera::SetAspectRatio( float ratio )
{
	aspectRatio = ratio;
}

void CCamera::SetFOV( float value )
{
	fieldOfView = value;
}

void CCamera::SetNearPlaneDistance( float value )
{
	zNear = value;
}

void CCamera::SetFarPlaneDistance( float value )
{
	zFar = value;
}

const glm::mat4& CCamera::GetViewMatrix() const
{
	return viewMatrix;
}

const glm::mat4& CCamera::GetProjectionMatrix() const
{
	return projectionMatrix;
}
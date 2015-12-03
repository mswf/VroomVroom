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
	UpdateView();
}

void CCamera::SetProjectionType( Projection type )
{
	switch (type)
	{
		case Projection::ORTHOGONAL:
		{
			//projection = glm::ortho(T left, T right, T bottom, T top, near, far);
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
}

void CCamera::SetUpVector( glm::vec3 up )
{
	upVector = up;
	UpdateView();
}

void CCamera::SetEyeVector( glm::vec3 eye )
{
	eyeVector = eye;
	UpdateView();
}

void CCamera::SetTargetVector( glm::vec3 center )
{
	centerVector = center;
	UpdateView();
}

void CCamera::UpdateView()
{
	// To simulate a circular aperture, we move around the camera in a circle in the plane perpendicular to the direction we are looking at. We can easily get two vectors describing the plane using cross products.
	//	glm::vec3 right = glm::normalize(glm::cross(object - eye, up));
	//	glm::vec3 p_up = glm::normalize(glm::cross(object - eye, right));
	// int n = 10; // number of light rays
	// glm::vec3 bokeh = right * cosf(i * 2 * M_PI / n) + p_up * sinf(i * 2 * M_PI / n);
	// float aperture = 0.05;
	viewMatrix = glm::lookAt( eyeVector, centerVector, upVector ); // Eye + Apeture + Bokeh
}

const glm::mat4& CCamera::GetViewMatrix() const
{
	return viewMatrix;
}

const glm::mat4& CCamera::GetProjectionMatrix() const
{
	return projectionMatrix;
}
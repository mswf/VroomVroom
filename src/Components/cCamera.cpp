#include "cCamera.h"
#include "entity.h"

const int CCamera::familyId = (int)ComponentTypes::CAMERA;

CCamera::CCamera( Projection proj, float aspectRatio, float near, float far, float fov )
:	type( proj ),
	fieldOfView( fov ),
	aspectRatio( aspectRatio ),
	zNear( near ),
	zFar( far ),
	viewportRectangle( glm::vec4( 0, 0, 1, 1 ) ),
	projectionMatrix( glm::mat4(1) ),
	viewMatrix( glm::mat4(1) )
{
	SetProjectionType( proj );
}

CCamera::~CCamera()
{
	
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
	// Needs a debug camera for testing purposes
	glm::vec3 eye = entity->transform->GetPosition();
	glm::vec3 target = entity->transform->GetRotation();
	
	entity->transform->SetWorldTransform( glm::inverse( glm::lookAt( eye , target, entity->transform->VectorUp() ) ) );
	viewMatrix = entity->GetTransform();
	//return glm::inverse( entity->GetTransform() );
}

void CCamera::SetAspectRatio( float ratio )
{
	aspectRatio = ratio;
	SetProjectionType(type);
}

void CCamera::SetFOV( float value )
{
	fieldOfView = value;
	SetProjectionType(type);
}

void CCamera::SetNearPlaneDistance( float value )
{
	zNear = value;
	SetProjectionType(type);
}

void CCamera::SetFarPlaneDistance( float value )
{
	zFar = value;
	SetProjectionType(type);
}

void CCamera::SetViewportRectangle( const glm::vec4& rect )
{
	viewportRectangle = rect;
}

const float& CCamera::GetAspectRatio() const
{
	return aspectRatio;
}

const float& CCamera::GetFOV() const
{
	return fieldOfView;
}

const float& CCamera::GetNearPlaneDistance() const
{
	return zNear;
}

const float& CCamera::GetFarPlaneDistance() const
{
	return zFar;
}

const Projection& CCamera::GetProjectionType() const
{
	return type;
}

const glm::vec4& CCamera::GetViewportRectangle() const
{
	return viewportRectangle;
}

const glm::mat4& CCamera::GetViewMatrix() const
{
	return viewMatrix;
}

const glm::mat4& CCamera::GetProjectionMatrix() const
{
	return projectionMatrix;
}
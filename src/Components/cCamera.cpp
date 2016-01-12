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
	glm::vec3 target = entity->transform->GetEulerAngles();
	
	entity->transform->SetWorldTransform( glm::inverse( glm::lookAt( eye , target, glm::vec3(0.0, 1.0, 0.0) ) ) );
	viewMatrix = entity->GetTransform();
	//return glm::inverse( entity->GetTransform() );
}

glm::vec3 CCamera::ScreenToWorldPosition( const glm::ivec2& position )
{
	printf("Hello, shooting ray at : %i x %i \n", position.x, position.y );
	//Debugging values
	float width = 1280.0f;
	float height = 720.0f;
	
	//Step 1
	float x = ( 2.0 * position.x ) / width - 1.0;
	float y = 1.0 - ( 2.0 * position.y ) / height;
	float z = 1.0;
	glm::vec3 ray_nds( x, y, z );
	
	//Step 2
	glm::vec4 ray_clip( ray_nds.x, ray_nds.y, -1.0, 1.0 );
	
	//Step 3
	glm::vec4 ray_eye = glm::inverse(projectionMatrix) * ray_clip;
	ray_eye = glm::vec4( ray_eye.x, ray_eye.y, -1.0, 1.0 );
	
	//Step 4
	glm::vec3 ray_world = glm::normalize( glm::vec3( glm::inverse(viewMatrix) * ray_eye ) );
	
	return ray_world;
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
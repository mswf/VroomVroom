#include "cCamera.h"
#include "entity.h"

const int CCamera::familyId = static_cast<int>(ComponentTypes::CAMERA);

CCamera::CCamera( Projection proj, float aspectRatio, float near, float far, float fov )
:	type( proj ),
	projectionMatrix( glm::mat4(1) ),
	viewMatrix( glm::mat4(1) ),
	viewportRectangle( glm::vec4( 0, 0, 1, 1 ) ),
	aspectRatio( aspectRatio ),
	fieldOfView( fov ),
	zNear( near ),
	zFar( far )
{
	SetProjectionType( proj );
}

CCamera::~CCamera()
{
	entity = nullptr;
	if (renderer != nullptr)
	{
		renderer->SetCamera( nullptr );
	}
	renderer = nullptr;
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
	glm::vec4 position = entity->transform->GetWorldTransform()[3];
	glm::vec3 eye = glm::vec3( position.x, position.y, position.z );
	glm::vec3 target = eye + entity->transform->VectorForward();// + entity->transform->GetEulerAngles();
	
	viewMatrix = glm::lookAt( eye , target, entity->transform->VectorUp() );
}

glm::vec3 CCamera::ScreenToWorldPosition( const glm::ivec2& position )
{
	float width = renderer->GetWindowWidth();
	float height = renderer->GetWindowHeight();
	
	printf( " %i %i \n", position.x, position.y );
	
	//Step 1
	float x = ( 2.0 * position.x ) / width - 1.0;
	float y = 1.0 - ( 2.0 * position.y ) / height;
	float z = -1.0;
	
	//Step 2
	glm::vec4 ray_eye = glm::inverse(projectionMatrix) * glm::vec4(x, y, z, 1.0);
	ray_eye = glm::vec4( ray_eye.x, ray_eye.y, -1.0, 1.0 );
	
	//Step 3
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

void CCamera::SetRenderer( Renderer* r )
{
	renderer = r;
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
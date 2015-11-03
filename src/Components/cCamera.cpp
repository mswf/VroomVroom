//
//  camera.cpp
//  VroomVroom
//
//  Created by Valentinas Rimeika on 02/11/15.
//  Copyright © 2015 Valentinas Rimeika. All rights reserved.
//

#include "cCamera.h"

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

void CCamera::SetUpVector( glm::vec3 up )
{
	upVector = up;
	UpdateView();
}

void CCamera::UpdateView()
{
	viewMatrix = glm::lookAt( eyeVector, centerVector, upVector );
}

glm::mat4 CCamera::GetViewMatrix()
{
	return viewMatrix;
}

glm::mat4 CCamera::GetProjectionMatrix()
{
	return projectionMatrix;
}
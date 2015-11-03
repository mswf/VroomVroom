//
//  camera.hpp
//  VroomVroom
//
//  Created by Valentinas Rimeika on 02/11/15.
//  Copyright © 2015 Valentinas Rimeika. All rights reserved.
//

#ifndef cCamera_h
#define cCamera_h

#include "component.h"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"

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
		void SetProjectionType( Projection type );
		void SetUpVector( glm::vec3 up );
		glm::mat4 GetViewMatrix();
		glm::mat4 GetProjectionMatrix();
	
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

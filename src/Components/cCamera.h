#ifndef cCamera_h
#define cCamera_h

#include "component.h"
#include "../glm/vec3.hpp"
#include "../glm/vec4.hpp"
#include "../glm/mat4x4.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../Systems/renderer.h"

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
		glm::vec3 ScreenToWorldPosition( const glm::ivec2& position );
		void SetProjectionType( Projection type );
		void SetAspectRatio( float ratio );
		void SetFOV( float value );
		void SetNearPlaneDistance( float zNear );
		void SetFarPlaneDistance( float zFar );
		void SetViewportRectangle( const glm::vec4& rect );
		void SetRenderer( Renderer* renderer );
		const Projection& GetProjectionType() const;
		const float& GetAspectRatio() const;
		const float& GetFOV() const;
		const float& GetNearPlaneDistance() const;
		const float& GetFarPlaneDistance() const;
		const glm::vec4& GetViewportRectangle() const;
		const glm::mat4& GetViewMatrix() const;
		const glm::mat4& GetProjectionMatrix() const;
	
	private:
	
		void UpdateView( const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up );
	
		Renderer* renderer;
		Projection type;
		glm::mat4 projectionMatrix;
		glm::mat4 viewMatrix;
		glm::vec3 upVector;
		glm::vec4 viewportRectangle;
		float aspectRatio;
		float fieldOfView;
		float zNear;
		float zFar;
	
};

/*
 // To simulate a circular aperture, we move around the camera in a circle in the plane perpendicular to the direction we are looking at. We can easily get two vectors describing the plane using cross products.
 	glm::vec3 right = glm::normalize(glm::cross(object - eye, up));
 	glm::vec3 p_up = glm::normalize(glm::cross(object - eye, right));
 	int n = 10; // number of light rays
 	glm::vec3 bokeh = right * cosf(i * 2 * M_PI / n) + p_up * sinf(i * 2 * M_PI / n);
 	float aperture = 0.05;
 
 	// Eye + Apeture + Bokeh
*/

#endif /* cCamera_h */

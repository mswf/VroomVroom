#ifndef renderer_h
#define renderer_h

#include <glew.h>
#include <iostream>	// cout, cerr, endl
#include <fstream>	// fin,

#include "Shader.h"

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace Renderer
{

	enum class Projection
	{
		ORTHOGONAL,
		PERSPECTIVE
	};
	
	struct Vertex
	{
		glm::vec3 position;
		glm::vec4 texcoord;
	};
	
	struct RenderData
	{
		GLuint framebuffer;
		Shader * shader;
		GLuint arraybuffer;
		GLuint elementbuffer;
		GLuint vertexbuffer;
	};
	
	struct Camera
	{
		glm::mat4 projection;
		glm::mat4 view;
		glm::vec3 eye;
		glm::vec3 center;
	};
	
	void RenderObject( unsigned int, RenderData*, Camera* );
	void Draw( RenderData* );
	
	void GetRenderData( RenderData* );
	void DeleteData( RenderData* );
	
	
	void LoadModel();
	void LoadTexture();
	GLuint CreateTexture( unsigned char*, int, int );
	
	void GetCamera( Camera*, Projection, float, float, float, float);
	void DeleteCamera( Camera* );
	void CreateOrthogonalProjection();
	void CreatePerspectiveProjection();
	void CreateFBO();

	GLuint GetAttribute( const char* );

}

#endif /* renderer_h */

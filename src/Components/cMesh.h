#ifndef cMesh_h
#define cMesh_h

#include "../Components/component.h"
#include <vector>
#include <glew.h>
#include "../glm/vec3.hpp"
#include "../glm/vec4.hpp"

class CMesh : public Component
{
	public:
	
		static const int familyId;
	
		CMesh();
		~CMesh();
	
		GLenum drawType;
		GLuint vertexArrayObject;
		GLuint vertexBufferObject;
		GLuint elementArratBuffer;
		GLuint numFaces;
		GLuint vertexLoc;
		GLuint normalLoc;
		GLuint texCoordLoc;
	
	private:
	
	
};

#endif /* mesh_h */

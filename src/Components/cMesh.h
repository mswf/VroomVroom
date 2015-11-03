#ifndef cMesh_h
#define cMesh_h

#include <vector>
#include "../Utilities/typedef.h"
#include "../glm/vec3.hpp"
#include "../glm/vec4.hpp"
#include "mesh.h"

class CMesh
{
	public:
	
		CMesh();
		~CMesh();
	
		uint32 vertexArrayObject;
		uint32 vertexBufferObject;
		uint32 indexBufferObject;
		uint32 numFaces;
		uint32 vertexLoc;
		uint32 normalLoc;
		uint32 texCoordLoc;
	
	private:
	
	
};

#endif /* mesh_h */

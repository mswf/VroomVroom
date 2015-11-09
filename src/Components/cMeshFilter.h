#ifndef cMesh_h
#define cMesh_h

#include "../Components/component.h"
#include <vector>
#include <glew.h>
#include "../DataStructure/data_types.h"

class CMeshFilter : public Component
{
	public:
	
		static const int familyId;
	
		CMeshFilter();
		~CMeshFilter();
	
		void Buffer( const Mesh* mesh );
		void UnBuffer();
	
		// GL_POINTS, GL_LINES,
		// GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FANS,
		// GL_QUADS, GL_QUAD_STRIP,
		// GL_POLYGONS
		GLenum drawType;
		// Vertex Array Object
		GLuint vao;
		// Vertex Buffer Object
		GLuint vbo;
		// Element Array Buffer
		GLuint eab;
		unsigned int numIndices;
		GLuint vertexAttributeLocation;
		GLuint normalAttributeLocation;
		GLuint texCoordAttributeLocation;
	
	private:
	
	
};

#endif /* mesh_h */

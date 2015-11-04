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
	
		struct Vertex
		{
			glm::vec3 position;
			glm::vec3 texcoord;
		};
	
		static const int familyId;
	
		CMesh();
		~CMesh();
	
		void GenerateBuffers( const Vertex* vertices, GLuint verticeCount, const GLubyte* indices, GLuint indiceCount );
	
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
		GLuint numIndices;
		GLuint vertexAttributeLocation;
		GLuint normalAttributeLocation;
		GLuint texCoordAttributeLocation;
	
	private:
	
	
};

#endif /* mesh_h */

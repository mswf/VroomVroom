#ifndef cMeshRenderer_h
#define cMeshRenderer_h

#include "component.h"
#include "../DataStructure/material.h"
#include "../DataStructure/data_types.h"

class CMeshRenderer : public Component
{
	public:
	
		static const int familyId;

		GLenum drawType;
	
		// Vertex Array Object
		GLuint vao;
		GLuint vbo;
		// Vertex Buffer Object
		GLuint normalBuffer;
		GLuint vertexBuffer;
		GLuint uvBuffer;
		// Element Array Buffer
		GLuint eab;
	
		unsigned int numIndices;
	
		Material* material;
	
		CMeshRenderer();
		~CMeshRenderer();
	
		void Buffer( const Mesh* mesh );
		void UnBuffer();
		void SetMaterial( Material* material );
		
	private:
	
};

#endif /* cMeshRenderer_h */

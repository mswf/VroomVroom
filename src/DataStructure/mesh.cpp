#include "mesh.h"
#include "../console.h"
#include "glew.h"
#include "material.h"

void BufferMesh( const Mesh* m, ModelInstance* instance, Material* mtl )
{
	
	GLuint p = mtl->shader->program;
	instance->numIndices = m->numIndices;
	
	//VAO always goes first before loading data to VBO
	glGenVertexArrays( 1, &instance->vao );
	glBindVertexArray( instance->vao );
	
	//EBO, storing indices
	//Terminal.LogRender( "Buffering indices" );
	glGenBuffers( 1, &instance->indiceBuffer);
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, instance->indiceBuffer );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, m->indices.size() * sizeof(unsigned int), &m->indices.front(), GL_STATIC_DRAW);
	
	//VBO, allocate data and upload data from CPU to GPU
	
	/*
	GLint position_attrib_index = 	glGetAttribLocation(p, "position");
	GLint normal_attrib_index = 	glGetAttribLocation(p, "normal");
	GLint texcoord_attrib_index = 	glGetAttribLocation(p, "texcoord");
	GLint tangent_attrib_index = 	glGetAttribLocation(p, "tangent");
	GLint bitangent_attrib_index = 	glGetAttribLocation(p, "bitangent");
	*/
	//Terminal.LogRender( "Buffering vertices" );
	
	/*
	
	glGenBuffers( 1, &instance->vertexBuffer );
	glBindBuffer( GL_ARRAY_BUFFER, instance->vertexBuffer );
	glBufferData( GL_ARRAY_BUFFER, m->vertices.size() * sizeof(glm::vec3), &m->vertices.front(), GL_STATIC_DRAW);
	glEnableVertexAttribArray( position_attrib_index );
	glVertexAttribPointer( position_attrib_index, 3, GL_FLOAT, GL_FALSE, 0, 0 );
	
	*/

	glGenBuffers( 1, &instance->vbo );
	glBindBuffer( GL_ARRAY_BUFFER, instance->vbo );
	unsigned long v, n, u, t, bt, buf, v3, v2;
	
	v3 = sizeof(glm::vec3);
	v2 = sizeof(glm::vec2);
	v = m->vertices.size() * v3;
	n = m->normals.size() * v3;
	u = m->uvs.size() * v2;
	t = m->tangents.size() * v3;
	bt = m->bitangents.size()* v3;
	buf = v + n + u + t + bt;
	
	glBufferData( GL_ARRAY_BUFFER, buf, NULL, GL_STATIC_DRAW);
	
	// Buffer vertices
	//glBufferData( GL_ARRAY_BUFFER, m->vertices.size() * sizeof(glm::vec3), &m->vertices.front(), GL_STATIC_DRAW);
	glBufferSubData( GL_ARRAY_BUFFER, 0, m->vertices.size() * sizeof(glm::vec3), &m->vertices.front() );
	glEnableVertexAttribArray( 0 ); //position_attrib_index
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );
	
	if (m->hasNormals)
	{
		//Terminal.LogRender( "Buffering normals" );
		//glGenBuffers( 1, &instance->normalBuffer );
		//glBindBuffer( GL_ARRAY_BUFFER, instance->normalBuffer );
		//glBufferData( GL_ARRAY_BUFFER, m->normals.size() * sizeof(glm::vec3), &m->normals.front(), GL_STATIC_DRAW);
		glBufferSubData( GL_ARRAY_BUFFER, v, m->normals.size() * sizeof(glm::vec3), &m->normals.front() );
		glEnableVertexAttribArray( 1 ); // normal_attrib_index
		//glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, 0 );
		glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, (void*)v );
	}
	
	if (m->hasUVs)
	{
		//Terminal.LogRender( "Buffering uvs" );
		//glGenBuffers( 1, &instance->uvBuffer );
		//glBindBuffer( GL_ARRAY_BUFFER, instance->uvBuffer );
		//glBufferData( GL_ARRAY_BUFFER, m->uvs.size() * sizeof(glm::vec2), &m->uvs.front(), GL_STATIC_DRAW);
		glBufferSubData( GL_ARRAY_BUFFER, v+n, m->uvs.size() * sizeof(glm::vec2), &m->uvs.front() );
		glEnableVertexAttribArray( 2 ); // texcoord_attrib_index
		glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 0, (void*)(v+n) );
	}
	
	if (m->hasTangentsAndBitangets)
	{
		//glGenBuffers( 1, &instance->tangentBuffer );
		//glBindBuffer( GL_ARRAY_BUFFER, instance->tangentBuffer );
		//glBufferData( GL_ARRAY_BUFFER, m->tangents.size() * sizeof(glm::vec3), &m->tangents.front(), GL_STATIC_DRAW);
		glBufferSubData( GL_ARRAY_BUFFER, v+n+u, m->tangents.size() * sizeof(glm::vec3), &m->tangents.front() );
		glEnableVertexAttribArray( 3 ); // tangent_attrib_index
		glVertexAttribPointer( 3, 3, GL_FLOAT, GL_FALSE, 0, (void*)(v+n+u) );
		
		//glGenBuffers( 1, &instance->bitangentBuffer );
		//glBindBuffer( GL_ARRAY_BUFFER, instance->bitangentBuffer );
		//glBufferData( GL_ARRAY_BUFFER, m->bitangents.size() * sizeof(glm::vec3), &m->bitangents.front(), GL_STATIC_DRAW);
		glBufferSubData( GL_ARRAY_BUFFER, v+n+u+t, m->bitangents.size() * sizeof(glm::vec3), &m->bitangents.front() );
		glEnableVertexAttribArray( 4 ); // bitangent_attrib_index
		glVertexAttribPointer( 4, 3, GL_FLOAT, GL_FALSE, 0, (void*)(v+n+u+t) );
	}
	
	/*
	 glGenBuffers( 1, &vbo );
	 glBindBuffer( GL_ARRAY_BUFFER, vbo );
	 unsigned long buf = m->vertices.size() * sizeof(glm::vec3) + m->normals.size() * sizeof(glm::vec3);
	 glBufferData( GL_ARRAY_BUFFER, buf, NULL, GL_STATIC_DRAW);
	 
	 glEnableVertexAttribArray( 0 );
	 glBufferSubData( GL_ARRAY_BUFFER, 0, m->vertices.size() * sizeof(glm::vec3), &m->vertices.front());
	 glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );
	 
	 glEnableVertexAttribArray( 1 );
	 glBufferSubData( GL_ARRAY_BUFFER, m->vertices.size() * sizeof(glm::vec3), m->normals.size() * sizeof(glm::vec3), &m->normals.front());
	 glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, (void*)sizeof(glm::vec3) );
	 */
	
	glBindVertexArray( 0 );
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
	
}

void UnBuffer( ModelInstance* instance )
{
	glDeleteVertexArrays( 1, &instance->vao );
	glDeleteBuffers( GL_ARRAY_BUFFER, &instance->vbo );
	//glDeleteBuffers( GL_ARRAY_BUFFER, &instance->vertexBuffer );
	//glDeleteBuffers( GL_ARRAY_BUFFER, &instance->normalBuffer );
	//glDeleteBuffers( GL_ARRAY_BUFFER, &instance->uvBuffer );
	glDeleteBuffers( GL_ELEMENT_ARRAY_BUFFER, &instance->indiceBuffer );
	
}
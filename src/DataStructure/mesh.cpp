#include "mesh.h"
#include "glew.h"

void BufferMesh( const Mesh* m, ModelInstance* instance )
{
	instance->numIndices = m->numIndices;
	unsigned long v, n, u, t, bt, buf, v3, v2;
	v3 = sizeof(glm::vec3);			// Vector3 size
	v2 = sizeof(glm::vec2);			// Vector2 size
	v = m->vertices.size() * v3; 	// vertice offset
	n = m->normals.size() * v3;		// normal offset
	u = m->uvs.size() * v2; 		// uv offset
	t = m->tangents.size() * v3; 	// tangent offset
	bt = m->bitangents.size()* v3; 	// btangent offset
	buf = v + n + u + t + bt; 		// Buffer size

	//VAO always goes first before loading data to VBO
	glGenVertexArrays( 1, &instance->vao );
	glBindVertexArray( instance->vao );
	
	//EBO, storing indices
	glGenBuffers( 1, &instance->indiceBuffer);
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, instance->indiceBuffer );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, m->indices.size() * sizeof(unsigned int), &m->indices.front(), GL_STATIC_DRAW);

	//VBO, storing vertices, normals, uvs, tangents, bitangents
	glGenBuffers( 1, &instance->vbo );
	glBindBuffer( GL_ARRAY_BUFFER, instance->vbo );
	glBufferData( GL_ARRAY_BUFFER, buf, NULL, GL_STATIC_DRAW);
	
	// Buffer vertices
	glBufferSubData( GL_ARRAY_BUFFER, 0, m->vertices.size() * sizeof(glm::vec3), &m->vertices.front() );
	glEnableVertexAttribArray( 0 );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );
	
	if (m->hasNormals)
	{
		// Buffer normals
		glBufferSubData( GL_ARRAY_BUFFER, v, m->normals.size() * sizeof(glm::vec3), &m->normals.front() );
		glEnableVertexAttribArray( 1 );
		glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, (void*)v );
	}
	
	if (m->hasUVs)
	{
		// Buffer uvs
		glBufferSubData( GL_ARRAY_BUFFER, v+n, m->uvs.size() * sizeof(glm::vec2), &m->uvs.front() );
		glEnableVertexAttribArray( 2 );
		glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 0, (void*)(v+n) );
	}
	
	if (m->hasTangentsAndBitangets)
	{
		// Buffer tangents
		glBufferSubData( GL_ARRAY_BUFFER, v+n+u, m->tangents.size() * sizeof(glm::vec3), &m->tangents.front() );
		glEnableVertexAttribArray( 3 );
		glVertexAttribPointer( 3, 3, GL_FLOAT, GL_FALSE, 0, (void*)(v+n+u) );
		
		// Buffer bitangents
		glBufferSubData( GL_ARRAY_BUFFER, v+n+u+t, m->bitangents.size() * sizeof(glm::vec3), &m->bitangents.front() );
		glEnableVertexAttribArray( 4 );
		glVertexAttribPointer( 4, 3, GL_FLOAT, GL_FALSE, 0, (void*)(v+n+u+t) );
	}
	
	glBindVertexArray( 0 );
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
	
}

void UnBuffer( ModelInstance* instance )
{
	glDeleteVertexArrays( 1, &instance->vao );
	glDeleteBuffers( GL_ARRAY_BUFFER, &instance->vbo );
	glDeleteBuffers( GL_ELEMENT_ARRAY_BUFFER, &instance->indiceBuffer );
	
}
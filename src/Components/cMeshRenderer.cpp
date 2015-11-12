#include "cMeshRenderer.h"
#include "entity.h"
#include "glew.h"
#include "console.h"
const int CMeshRenderer::familyId = (int)ComponentTypes::MESH_RENDERER;

CMeshRenderer::CMeshRenderer() :
	material( NULL ),
	drawType(GL_TRIANGLES), // GL_POINTS, GL_LINES, GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FANS, GL_QUADS, GL_QUAD_STRIP, GL_POLYGONS
	vao( 0 ),
	eab( 0 ),
	vbo( 0 ),
	numIndices( 0 )
{
}

CMeshRenderer::~CMeshRenderer()
{
	//TODO(Valentinas): DELETE MATERIAL HERE?
	//delete material;
}

void CMeshRenderer::SetMaterial( Material* mat )
{
	material = mat;
}

void CMeshRenderer::Buffer( const Mesh* m )
{
	GLuint p;
	if (material == NULL)
	{
		Terminal.LogRender( "Material NOT ASSIGNED" );
		return;
	}
	p = material->shader->program;
	numIndices = m->numIndices;
	
	//VAO always goes first before loading data to VBO
	glGenVertexArrays( 1, &vao );
	glBindVertexArray( vao );
	
	//EBO, storing indices
	Terminal.LogRender( "Buffering indices" );
	glGenBuffers( 1, &eab);
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, eab );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, m->indices.size() * sizeof(unsigned int), &m->indices.front(), GL_STATIC_DRAW);
	
	//VBO, allocate data and upload data from CPU to GPU

	GLint position_attrib_index = glGetAttribLocation(p, "position");
	GLint normal_attrib_index = glGetAttribLocation(p, "normal");
	GLint texcoord_attrib_index = glGetAttribLocation(p, "texcoord");
	
	Terminal.LogRender( "Buffering vertices" );
	
	glGenBuffers( 1, &vertexBuffer );
	glBindBuffer( GL_ARRAY_BUFFER, vertexBuffer );
	glBufferData( GL_ARRAY_BUFFER, m->vertices.size() * sizeof(glm::vec3), &m->vertices.front(), GL_STATIC_DRAW);
	glEnableVertexAttribArray( position_attrib_index );
	glVertexAttribPointer( position_attrib_index, 3, GL_FLOAT, GL_FALSE, 0, 0 );
	
	if (m->hasNormals)
	{
		Terminal.LogRender( "Buffering normals" );
		glGenBuffers( 1, &normalBuffer );
		glBindBuffer( GL_ARRAY_BUFFER, normalBuffer );
		glBufferData( GL_ARRAY_BUFFER, m->normals.size() * sizeof(glm::vec3), &m->normals.front(), GL_STATIC_DRAW);
		glEnableVertexAttribArray( normal_attrib_index );
		glVertexAttribPointer( normal_attrib_index, 3, GL_FLOAT, GL_FALSE, 0, 0 );
	}
	
	if (m->hasUVs)
	{
		Terminal.LogRender( "Buffering uvs" );
		glGenBuffers( 1, &uvBuffer );
		glBindBuffer( GL_ARRAY_BUFFER, uvBuffer );
		glBufferData( GL_ARRAY_BUFFER, m->uvs.size() * sizeof(glm::vec2), &m->uvs.front(), GL_STATIC_DRAW);
		glVertexAttribPointer( texcoord_attrib_index, 2, GL_FLOAT, GL_FALSE, 0, 0 );
		glEnableVertexAttribArray( texcoord_attrib_index );
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

void CMeshRenderer::UnBuffer()
{
	glDeleteVertexArrays( 1, &vao );
	glDeleteBuffers( GL_ARRAY_BUFFER, &vertexBuffer );
	glDeleteBuffers( GL_ARRAY_BUFFER, &normalBuffer );
	glDeleteBuffers( GL_ARRAY_BUFFER, &uvBuffer );
	glDeleteBuffers( GL_ELEMENT_ARRAY_BUFFER, &eab );
	
}
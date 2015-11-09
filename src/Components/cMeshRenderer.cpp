#include "cMeshRenderer.h"
#include "entity.h"

const int CMeshRenderer::familyId = (int)ComponentTypes::MESH_RENDERER;

CMeshRenderer::CMeshRenderer() :
	material( NULL ),
	drawType(GL_TRIANGLES), // GL_POINTS, GL_LINES, GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FANS, GL_QUADS, GL_QUAD_STRIP, GL_POLYGONS
	vao( 0 ),
	eab( 0 ),
	vbo( 0 ),
	hasNormals( false ),
	hasUVs( false ),
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
	
	//VAO always goes first before loading data to VBO
	glGenVertexArrays( 1, &vao );
	glBindVertexArray( vao );
	
	//VBO, allocate data and upload data from CPU to GPU
	glGenBuffers( 1, &vbo );
	glBindBuffer( GL_ARRAY_BUFFER, vbo );
	glBufferData( GL_ARRAY_BUFFER, m->vertices.size() * sizeof(glm::vec3), &m->vertices.front(), GL_STATIC_DRAW);
	glEnableVertexAttribArray( 0 );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0 );
	
	//EBO, storing indices
	glGenBuffers( 1, &eab);
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, eab );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, m->indices.size(), &m->indices.front(), GL_STATIC_DRAW);
	
	
	numIndices = (unsigned int)m->indices.size();
	
	if (hasNormals)
	{
		glGenBuffers( 1, &vbo );
		glBindBuffer( GL_ARRAY_BUFFER, vbo );
		glBufferData( GL_ARRAY_BUFFER, m->normals.size() * sizeof(glm::vec3), &m->normals.front(), GL_STATIC_DRAW);
		glEnableVertexAttribArray( 1 );
		glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0 );
	}
	
	if (hasUVs)
	{
		glGenBuffers( 1, &vbo );
		glBindBuffer( GL_ARRAY_BUFFER, vbo );
		glBufferData( GL_ARRAY_BUFFER, m->uvs.size() * sizeof(glm::vec2), &m->uvs.front(), GL_STATIC_DRAW);
		glEnableVertexAttribArray( 2 );
		glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0 );
	}
	
	glBindVertexArray( 0 );
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
	
}

void CMeshRenderer::UnBuffer()
{
	glDeleteVertexArrays( 1, &vao );
	glDeleteBuffers( GL_ARRAY_BUFFER, &vbo );
	glDeleteBuffers( GL_ELEMENT_ARRAY_BUFFER, &eab );
	
}
#include "cMeshFilter.h"

const int CMeshFilter::familyId = (int)ComponentTypes::MESH;

CMeshFilter::CMeshFilter() :
	drawType(GL_TRIANGLES),
	vao(0),	eab(0), vbo(0),	numIndices(0),
	vertexAttributeLocation(0), normalAttributeLocation(0), texCoordAttributeLocation(0)
{
	
}

CMeshFilter::~CMeshFilter()
{

}

void CMeshFilter::Buffer( const Mesh* m )
{
	
	//VAO always goes first before loading data to VBO
	glGenVertexArrays( 1, &vao );
	glBindVertexArray( vao );
	
	//VBO, allocate data and upload data from CPU to GPU
	glGenBuffers( 1, &vbo );
	glBindBuffer( GL_ARRAY_BUFFER, vbo );
	glBufferData( GL_ARRAY_BUFFER, m->vertices.size() * sizeof(glm::vec3), &m->vertices.front(), GL_STATIC_DRAW);
	
	//EBO, storing indices
	glGenBuffers( 1, &eab);
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, eab );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, m->indices.size(), &m->indices.front(), GL_STATIC_DRAW);
	
	//numIndices = m->indices.size();
	
	glBindVertexArray( 0 );
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

}
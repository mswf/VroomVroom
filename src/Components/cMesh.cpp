#include "cMesh.h"

const int CMesh::familyId = (int)ComponentTypes::MESH;

CMesh::CMesh() :
	drawType(GL_TRIANGLES),
	vao(0),	eab(0), vbo(0),	numIndices(0),
	vertexAttributeLocation(0), normalAttributeLocation(0), texCoordAttributeLocation(0)
{
	
}

CMesh::~CMesh()
{

}

void CMesh::GenerateBuffers( const Vertex* vertices, GLuint verticeCount, const GLubyte* indices, GLuint indiceCount )
{
	
	//VAO always goes first before loading data to VBO
	glGenVertexArrays( 1, &vao );
	glBindVertexArray( vao );
	
	//VBO, allocate data and upload data from CPU to GPU
	glGenBuffers( 1, &vbo );
	glBindBuffer( GL_ARRAY_BUFFER, vbo );
	glBufferData( GL_ARRAY_BUFFER, verticeCount * sizeof(Vertex), vertices, GL_STATIC_DRAW);
	
	//EBO, storing indices
	glGenBuffers( 1, &eab);
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, eab );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, indiceCount, indices, GL_STATIC_DRAW);
	
	//GLuint position = glGetAttribLocation( mat->shader->program, "position" );
	//GLuint texcoord = glGetAttribLocation( mat->shader->program, "texcoord" );
	
	//glEnableVertexAttribArray( position );
	//glEnableVertexAttribArray( texcoord );
	
	//glVertexAttribPointer( position, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0 );
	//glVertexAttribPointer( texcoord, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*) sizeof(glm::vec3) );
	
	glBindVertexArray( 0 );
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
}

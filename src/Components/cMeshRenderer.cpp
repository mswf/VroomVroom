#include "cMeshRenderer.h"
#include "entity.h"

const int CMeshRenderer::familyId = (int)ComponentTypes::MESH_RENDERER;

CMeshRenderer::CMeshRenderer() :
	mesh(NULL)
{
	Update();
}

CMeshRenderer::~CMeshRenderer()
{
	
}

void CMeshRenderer::Update()
{
	mesh = Entity::GetComponent<CMeshFilter>(entity);
}

void CMeshRenderer::SetVertexAttributes( const char* attribute )
{
	
	 GLuint position = glGetAttribLocation( material->shader->program, "position" );
	 //GLuint texcoord = glGetAttribLocation( material->shader->program, "texcoord" );
	 //GLuint normal = glGetAttribLocation( material->shader->program, "normal" );
	 
	 glEnableVertexAttribArray( position );
	 //glEnableVertexAttribArray( texcoord );
	 //glEnableVertexAttribArray( normal );
	 
	 glVertexAttribPointer( position, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0 );
	 //glVertexAttribPointer( texcoord, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0 );
	 //glVertexAttribPointer( normal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*) (sizeof(glm::vec3) + sizeof(glm::vec2)) );
	
	// READ THIS MESSAGE BEFORE ADDING THIS STUFF:
	// Use layouts (location = 0) in vec4 position
	// Add flags whether mesh contains normals or uvs
	// Depending on flags, buffer & set Vertex Attributes.
}
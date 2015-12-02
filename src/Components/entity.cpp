#include "entity.h"
#include <iostream>

// Generated some random string so that no one compares it in constructor
const char* Entity::root_name = "root_:)_XgPFL>u{9+?9,GllN6IF;+L:<CkXvEn9Y$6dX[}47/BxB7nn40/$@j%p|b>i:Rt,)yFs{)qy{qN=JX.vq2vm+.a^A-{s0(mTg$0_s,fZMUgHx<4sHC-#@!?$XtiW}dnz7-4^-M^tdbd9N.DYR#v?,(?q!S!O]WmQnug}f-vq12OH:t_28!=BARhqRD5#n_|&EiHgJkDtjvMkF7LR;@QRKj*E{5G9hdV6E>=jc6BfzrZ=gbO|oU2apN^.B%";

std::multimap< int, Entity* > Entity::componentStorage;
Entity* Entity::root = new Entity( root_name );

Entity::Entity( std::string name, Entity* parent ) :
	parent(parent),
	name(name)
{
	if( parent == NULL && name != root_name )
	{
		Entity::root->AddChild(this);
	}
	if( parent != NULL )
	{
		parent->AddChild(this);
	}
	transform = new CTransform();
	AddComponent(this, transform);
}

Entity::~Entity()
{
	// TODO(Valentinas): Test if the children are cleaned properly
	std::vector< Entity* >::const_iterator iter = children.begin();
	std::vector< Entity* >::const_iterator end = children.end();
	for ( ; iter != end; ++iter)
	{
		delete (*iter);
	}
}

void Entity::AddChild( Entity* c )
{
	if ( c->parent != NULL )
	{
		c->parent->RemoveChild( c );
	}
	children.push_back(c);
	c->parent = this;
}

void Entity::RemoveChild( Entity *c )
{
	std::vector< Entity* >::const_iterator iter = children.begin();
	std::vector< Entity* >::const_iterator end = children.end();
	for ( ; iter != end; ++iter)
	{
		if ( c == (*iter) )
		{
			
			children.erase(iter);
			break;
		}
	}
}

void Entity::Update()
{
	// localTransform = parentWorldTransform.inverse() * worldTransform;
	// M_loc = M_parent_inv * M
	glm::mat4 localTransform = GetTransform();
	glm::mat4 worldTransform = (parent != NULL ) ? (parent->transform->GetWorldTransform() * localTransform) : localTransform;
	transform->SetWorldTransform(worldTransform);
	
	std::vector< Entity* >::const_iterator iter = children.begin();
	std::vector< Entity* >::const_iterator end = children.end();
	for( ; iter != end; ++iter )
 	{
		(*iter)->Update();
	}
}

const glm::mat4& Entity::GetTransform()
{
	return transform->GetTransform();
}


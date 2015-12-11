#include "entity.h"
#include <iostream>

// Generated some random string so that no one compares it in constructor
const char* Entity::root_name = "root_:)_XgPFL>u{9+?9,GllN6IF;+L:<CkXvEn9Y$6dX[}47/BxB7nn40/$@j%p|b>i:Rt,)yFs{)qy{qN=JX.vq2vm+.a^A-{s0(mTg$0_s,fZMUgHx<4sHC-#@!?$XtiW}dnz7-4^-M^tdbd9N.DYR#v?,(?q!S!O]WmQnug}f-vq12OH:t_28!=BARhqRD5#n_|&EiHgJkDtjvMkF7LR;@QRKj*E{5G9hdV6E>=jc6BfzrZ=gbO|oU2apN^.B%";

std::multimap< int, Entity* > Entity::componentStorage;
Entity* Entity::root = new Entity( root_name );

// TODO: FIX THIS: Possible crash if you pass yourself as the parent in the constructor
Entity::Entity( std::string name, Entity* parent ) :
	name(name)
{
	transform = new CTransform();
	transform->entity = this;
	
	if( transform->GetParent() == NULL && name != root_name )
	{
		Entity::root->AddChild(this);
	}
	if( parent != NULL )
	{
		parent->AddChild(this);
	}
}

Entity::~Entity()
{
	// TODO(Valentinas): Test if the children are cleaned properly
	std::map< int, Component* >::const_iterator it = entityComponents.begin();
	std::map< int, Component* >::const_iterator end = entityComponents.end();
	for( ; it != end; ++it )
	{
		delete (*it).second;
		entityComponents.erase(it);
	}
	delete transform;
}

void Entity::AddChild( Entity* c )
{
	transform->AddChild( c->transform );
}

void Entity::RemoveChild( Entity *c )
{
	transform->RemoveChild( c->transform );
}

void Entity::Update()
{
	std::map< int, Component* >::const_iterator iter_comp = entityComponents.begin();
	std::map< int, Component* >::const_iterator end_comp = entityComponents.end();
	for( ; iter_comp != end_comp; ++iter_comp )
	{
		(*iter_comp).second->Call();
	}
}

const glm::mat4& Entity::GetTransform() const
{
	return transform->GetTransform();
}

const Entity* Entity::GetParent() const
{
	return transform->GetParent()->entity;
}

const std::vector<Entity*> Entity::GetChildren() const
{
	std::vector<Entity* > children;
	std::vector<CTransform* >::const_iterator it = transform->GetChildren().begin();
	std::vector<CTransform* >::const_iterator end = transform->GetChildren().end();
	for( ; it != end ; ++it )
	{
		children.push_back( (*it)->entity );
	}
	return children;
}




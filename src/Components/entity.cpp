#include "entity.h"
#include <iostream>

std::multimap< int, Entity* > Entity::componentStorage;

Entity::Entity( std::string name, Entity* parent ) :
	parent(parent),
	name(name)
{
	AddComponent(this, new CTransform());
}

Entity::~Entity()
{
	std::vector< Entity* >::const_iterator iter = GetChildrenIteratorStart();
	std::vector< Entity* >::const_iterator end = GetChildrenIteratorEnd();
	for ( ; iter != end; ++iter)
	{
		delete *iter;
	}
}

void Entity::AddChild( Entity* c )
{
	children.push_back(c);
	c->parent = this;
}

void Entity::Update()
{
	glm::mat4 localTransform = GetTransform();
	worldTransform = (parent != NULL ) ? (parent->worldTransform * localTransform) : localTransform;
	
	std::vector< Entity* >::const_iterator iter = children.begin();
	std::vector< Entity* >::const_iterator end = children.end();
	for( ; iter != end; ++iter )
 	{
		(*iter)->Update();
	}
}

std::vector< Entity* >::const_iterator Entity::GetChildrenIteratorStart() const
{
	return children.begin();
}

std::vector< Entity* >::const_iterator Entity::GetChildrenIteratorEnd() const
{
	return children.end();
}

const glm::mat4& Entity::GetTransform()
{
	return GetComponent<CTransform>(this)->GetTransform();
}


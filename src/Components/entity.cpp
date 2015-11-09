#include "entity.h"
#include "cTransform.h"

std::multimap< int, Entity* > Entity::componentStorage;

Entity::Entity( std::string name, Entity* parent ) :
	parent(parent),
	name(name)
{
	AddComponent(this, new CTransform());
}

Entity::~Entity()
{
	std::vector< Entity* >::const_iterator iter = children.begin();
	std::vector< Entity* >::const_iterator end = children.end();
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
	// WorldTransform = (parent != NULL ) ? (parent->WorldTransform * this.Transform) : this.Transform;
	std::vector< Entity* >::const_iterator iter = children.begin();
	std::vector< Entity* >::const_iterator end = children.begin();
	for( ; iter != end; ++iter )
 	{
		(*iter)->Update();
	}
}

void Entity::GetChildrenIteratorStart() const
{
	//return children.begin();
}

void Entity::GetChildrenIteratorEnd() const
{
	//return children.end();
}

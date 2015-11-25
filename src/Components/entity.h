#ifndef entity_h
#define entity_h

#include "../Components/component.h"
#include "cTransform.h"
#include <string>
#include <vector>
#include <map>

class Entity
{
	public:
	
		Entity( std::string name = "Entity_Object", Entity* parent = NULL );
		~Entity();
	
		std::map< int, Component* > entityComponents;
	
		void AddChild( Entity* c );
		void Update();
		const glm::mat4& GetTransform();
		std::vector< Entity* >::const_iterator GetChildrenIteratorStart() const;
		std::vector< Entity* >::const_iterator GetChildrenIteratorEnd() const;
	
		// TODO(Valentinas): Add remove function for components
	
		template<typename T>
		static void AddComponent( Entity* e, T* comp )
		{
			Entity::componentStorage.insert( std::pair< int, Entity* >( T::familyId, e ) );
			e->entityComponents.insert( std::pair< int, Component* >( T::familyId, comp ) );
			comp->entity = e;
		}
	
		template<typename T>
		static T* GetComponent( Entity* e )
		{
			return (T*)e->entityComponents[T::familyId];
		}

	
		template<typename T>
		static void GetEntities( std::vector< Entity* > &result )
		{
			auto iterPair = Entity::componentStorage.equal_range( T::familyId );
			for ( auto iter = iterPair.first; iter != iterPair.second; ++iter )
			{
				result.push_back( iter->second );
			}
		}
	
		glm::mat4 worldTransform;
	protected:
//		glm::mat4 worldTransform;
		Entity* parent;
		std::vector< Entity* > children;
		std::string name;
	
	private:
		static std::multimap< int, Entity* > componentStorage;
};

#endif /* entity_h */

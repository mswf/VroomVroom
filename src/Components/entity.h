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
	
		static Entity* root;
	
		Entity( std::string name = "Entity_Object", Entity* parent = NULL );
		~Entity();
	
		void AddChild( Entity* c );
		void RemoveChild( Entity* c );
		void Update();
		const glm::mat4& GetTransform();
		const Entity* GetParent();
		const std::vector<Entity*> GetChildren();
	
		template<typename T>
		static void AddComponent( Entity* e, T* comp )
		{
			Entity::componentStorage.insert( std::pair< int, Entity* >( T::familyId, e ) );
			e->entityComponents.insert( std::pair< int, Component* >( T::familyId, comp ) );
			comp->IncrementAddedToEntity();
			comp->entity = e;
		}
	
		template<typename T>
		static void RemoveComponent( Entity* e, T* comp )
		{
			unsigned int id = T::familyId;
			std::map< int, Component* >::const_iterator it = e->entityComponents.find( id );
			if ( it != e->entityComponents.end() )
			{
				std::multimap<int, Entity*>::iterator ig;
				for ( ig = componentStorage.equal_range(id).first; ig != componentStorage.equal_range(id).second; ++ig )
				{
					if ( (*ig).second->name == e->name )
					{
						componentStorage.erase(ig);
						//std::cout << (*ig).first << ' ' << (*ig).second->name << std::endl;
					}
				}
				e->entityComponents.erase(it);
			}
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
	
		std::string name;
		std::map< int, Component* > entityComponents;
		CTransform* transform;
	
	private:
	
		static std::multimap< int, Entity* > componentStorage;
		static const char* root_name;
};

#endif /* entity_h */

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
		static bool shouldClean;
	
		Entity( std::string name = "Entity_Object_" + std::to_string(counter) );
		~Entity();
	
		void AddChild( Entity* c ) const;
		void RemoveChild( Entity* c ) const;
		void ClearComponents();
		void DestroyChildren() const;
		void Update();
		const glm::mat4& GetTransform() const;
		const Entity* GetParent() const;
		std::vector<Entity*> GetChildren() const;
		// TODO(Valentinas): Adding the same component to the same entity will add it to the global list twice,
	 	//					 removing will remove both of them, so can be fixed later
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
			return static_cast<T*>(e->entityComponents[T::familyId]);
		}
	
		template<typename T>
		static std::vector< T* > GetComponentInChildren( Entity* e )
		{
			std::vector< T* > list;
			std::vector< CTransform* >::const_iterator it = e->transform->GetChildren().begin();
			std::vector< CTransform* >::const_iterator end = e->transform->GetChildren().end();
			for( ; it != end ; ++it )
			{
				Entity* entity = (*it)->entity;
				list.push_back( Entity::GetComponent<T>( entity ) );
				std::vector< T* > childList = Entity::GetComponentInChildren<T>( entity );
				list.insert( std::end(list), std::begin(childList), std::end(childList) );
			}
			return list;
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
	
		static void Destroy( Entity* e )
		{
			e->destroy = true;
			shouldClean = true;
			std::vector<CTransform* >::const_iterator it_c = e->transform->GetChildren().begin();
			std::vector<CTransform* >::const_iterator end_c = e->transform->GetChildren().end();
			for( ; it_c != end_c ; ++it_c )
			{
				(*it_c)->entity->destroy = true;
			}
		}

		bool IsSetToDestroy() const { return destroy; }
	
		std::string name;
		std::map< int, Component* > entityComponents;
		CTransform* transform;
	
	private:
	
		static std::multimap< int, Entity* > componentStorage;
		static const char* root_name;
		static unsigned int counter;
		bool destroy;
};

#endif /* entity_h */

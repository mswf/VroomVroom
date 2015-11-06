//
//  entity.h
//  VroomVroom
//
//  Created by Valentinas Rimeika on 03/11/15.
//  Copyright © 2015 Valentinas Rimeika. All rights reserved.
//

#ifndef entity_h
#define entity_h

#include "../Components/component.h"
#include <string>
#include <vector>
#include <map>

class Entity
{
	public:
	
		Entity( std::string name = "entity_object"  );
		~Entity();
	
		std::map< int, Component* > entityComponents;
	
		// TODO: Add remove function for components
	
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

	
	private:
		static std::multimap< int, Entity* > componentStorage;
		std::string name;
};

#endif /* entity_h */

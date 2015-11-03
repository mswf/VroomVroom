//
//  entity.cpp
//  VroomVroom
//
//  Created by Valentinas Rimeika on 03/11/15.
//  Copyright © 2015 Valentinas Rimeika. All rights reserved.
//

#include "entity.h"

std::multimap< int, Entity* > Entity::componentStorage;

Entity::Entity( std::string name ) :
	name(name)
{
	
}
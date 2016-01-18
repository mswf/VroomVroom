//
//  mCollider.hpp
//  VroomVroom
//
//  Created by Robin Zaagsma on 18/01/16.
//  Copyright © 2016 Valentinas Rimeika. All rights reserved.
//

#ifndef mCollider_hpp
#define mCollider_hpp

#include "moduleMacros.h"

class mCollider
{
	public:
		static void Bind(lua_State*);
	private:
		lFuncDef(__boxEngineInit);
		lFuncDef(__sphereEngineInit);
		lFuncDef(collide);
	
		lFuncDef(setCollisionBox);
		lFuncDef(getCollisionBox);
	
		lFuncDef(setRadius);
		lFuncDef(getRadius);
	
};

#endif /* mCollider_hpp */

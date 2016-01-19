//
//  mRaycast.hpp
//  VroomVroom
//
//  Created by Robin Zaagsma on 19/01/16.
//  Copyright © 2016 Valentinas Rimeika. All rights reserved.
//

#ifndef mRaycast_hpp
#define mRaycast_hpp

#include "moduleMacros.h"

class mRaycast
{
	public:
		static void Bind(lua_State*);
	private:
		lFuncDef(castRay);
};

#endif /* mRaycast_hpp */

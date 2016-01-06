//
//  mLight.hpp
//  VroomVroom
//
//  Created by Robin Zaagsma on 05/01/16.
//  Copyright © 2016 Valentinas Rimeika. All rights reserved.
//

#ifndef mLight_hpp
#define mLight_hpp

#include "moduleMacros.h"

class mLight {
	public:
		static void Bind(lua_State*);
	private:
	lFuncDef(__engineInit);
	
	lFuncDef(setIntensity);
	lFuncDef(setColor);
	lFuncDef(setType);
};

#endif /* mLight_hpp */

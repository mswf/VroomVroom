//
//  mSystem.hpp
//  VroomVroom
//
//  Created by Robin Zaagsma on 02/12/15.
//  Copyright © 2015 Valentinas Rimeika. All rights reserved.
//

#ifndef mSystem_hpp
#define mSystem_hpp

#include <stdio.h>
#include "moduleMacros.h"

class mSystem
{
	public:
		static void Bind(lua_State*);
	private:
		lFuncDef(clearConsole);
};

#endif /* mSystem_hpp */

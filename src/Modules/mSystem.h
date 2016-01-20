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

class Renderer;

class mSystem
{
	public:
		static void Bind(lua_State*);
	
		static void SetRenderer(Renderer*);
	
	private:
		static Renderer* renderer;
	
		lFuncDef(clearConsole);
	
		lFuncDef(setSkybox);
};

#endif /* mSystem_hpp */

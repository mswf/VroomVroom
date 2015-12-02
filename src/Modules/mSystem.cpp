//
//  mSystem.cpp
//  VroomVroom
//
//  Created by Robin Zaagsma on 02/12/15.
//  Copyright © 2015 Valentinas Rimeika. All rights reserved.
//

#include "mSystem.h"
#include "../content.h"

void mSystem::Bind(lua_State* L)
{
	lua_getglobal(L, "Engine");
	
	lua_newtable(L);
	
	lua_pushstring(L, Content::GetPath().c_str());
	lua_setfield(L, -2, "contentPath");
	
	lua_setfield(L, -2, "system");
}
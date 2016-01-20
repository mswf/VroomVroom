//
//  mSystem.cpp
//  VroomVroom
//
//  Created by Robin Zaagsma on 02/12/15.
//  Copyright © 2015 Valentinas Rimeika. All rights reserved.
//

#include "mSystem.h"
#include "../content.h"
#include "../console.h"
#include "../Systems/Renderer.h"

Renderer* mSystem::renderer = nullptr;

void mSystem::Bind(lua_State* L)
{
	lua_getglobal(L, "Engine");
	
	lua_newtable(L);
	
	lua_pushstring(L, Content::GetPath().c_str());
	lua_setfield(L, -2, "contentPath");
	
	lua_setfield(L, -2, "system");
	
	lua_newtable(L);
	
	lua_pushcfunction(L, lw_setSkybox__);
	lua_setfield(L, -2, "setSkybox");
	
	lua_setfield(L, -2, "renderer");

	
	//TODO(robin): this is probably better of in mLog. It's not in any table anyway
	lua_pushcfunction(L, lw_clearConsole__);
	lua_setfield(L, -2, "clearConsole");
}

void mSystem::SetRenderer(Renderer* r)
{
	renderer = r;
}

lFuncImp(mSystem, clearConsole)
{
	Terminal.Clear();
	return 0;
}

lFuncImp(mSystem, setSkybox)
{
	lua_settop(L, 1);
	lgString(name, 1, "undefined");
	
	renderer->SetSkybox(name.c_str());
	return 0;
}
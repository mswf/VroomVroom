//
//  mWindow.cpp
//  VroomVroom
//
//  Created by Steff Kempink on 16/12/15.
//  Copyright © 2015 Valentinas Rimeika. All rights reserved.
//

#include "mWindow.h"
#include "engine.h"

//#include "../Utilities/typedef.h"

void mWindow::Bind(lua_State* L)
{
	lua_getglobal(L, "Engine");

	lua_newtable(L);
	lStart(Window)
		lBind(getSize)
		lBind(getWidth)
		lBind(getHeight)
		lBind(setSize)
	lEnd(Window)
	luaL_openlib(L, 0, Window_funcs, 0);
	lua_setfield(L, -2, "window");
}

lFuncImp(mWindow, getSize)
{
	lua_settop(L, 1);

	glm::vec2 dimensions = Engine::GetWindowSize();

	lua_pushnumber(L, dimensions.x);
	lua_pushnumber(L, dimensions.y);

	return 2;
}

lFuncImp(mWindow, getWidth)
{
	lua_settop(L, 1);

	glm::vec2 dimensions = Engine::GetWindowSize();

	lua_pushnumber(L, dimensions.x);

	return 1;
}

lFuncImp(mWindow, getHeight)
{
	lua_settop(L, 1);

	glm::vec2 dimensions = Engine::GetWindowSize();

	lua_pushnumber(L, dimensions.y);

	return 1;
}

lFuncImp(mWindow, setSize)
{
	lua_settop(L, 3);

	lgInt(width, 1, 1280);
	lgInt(height, 2, 720);

	Engine::SetWindowSize(glm::vec2(width, height));

	return 0;
}
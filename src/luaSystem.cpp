//
//  LuaSystem.cpp
//  VroomVroom
//
//  Created by Robin Zaagsma on 09/10/15.
//  Copyright © 2015 Valentinas Rimeika. All rights reserved.
//

#include "luaSystem.h"
#include "content.h"
#include <iostream>
#include <lua.hpp>
#include "standardIncludes.h"

sLuaSystem::sLuaSystem():
	hasMainBeenCalled(false)
{
	lState = luaL_newstate();
	luaL_openlibs(lState);
	SetPackagePath();
	BindEngine();

	string path;
	Content::CreateFilePath("main.lua", &path);

	if (luaL_loadfile(lState, path.c_str()))
	{
		//TODO replace couts with proper Console::Error or even Console::LuaError?
		std::cout << "Could not open main.lua - The program will not run correctly" << std::endl;
		return;
	}

	//try to parse main.lua
	if (lua_pcall(lState, 0, 0, 0) != 0)
	{
		//TODO replace couts with proper Console::Error or even Console::LuaError?
		std::cout << "lua error: " << lua_tostring(lState, -1) << std::endl;
		return;
	}
}

sLuaSystem::~sLuaSystem()
{
	//TODO release resources
}

void sLuaSystem::Main()
{
	if (!hasMainBeenCalled)
	{
		lua_getglobal(lState, "Game");
		lua_getfield(lState, -1, "main");
		if (lua_pcall(lState, 0, 0, 0) != 0)
		{
			//TODO replace couts with proper Console::Error or even Console::LuaError?
			std::cout << "lua error: " << lua_tostring(lState, -1) << std::endl;
		}

		hasMainBeenCalled = true;
	}
}

void sLuaSystem::Update(int dt)
{
	lua_getglobal(lState, "Game");
	lua_getfield(lState, -1, "update");
	lua_pushnumber(lState, dt);
	if (lua_pcall(lState, 1, 0, 0) != 0)
	{
		//TODO replace couts with proper Console::Error or even Console::LuaError?
		std::cout << "lua error: " << lua_tostring(lState, -1) << std::endl;
	}
}

void sLuaSystem::Attempt(string command)
{
    if(luaL_dostring(lState, command.c_str()) != 0){
        Terminal.LuaError(string(lua_tostring(lState, -1)));
    }
}

//PRIVATE
/*
 *  Tells the Lua state to look in the content directory whenever 'require' is used in Lua scripts.
 */
void sLuaSystem::SetPackagePath()
{
	lua_getglobal(lState, "package");
	lua_getfield(lState, -1, "path");
	const char* cur_path = lua_tostring(lState, -1);
	string new_path = string(cur_path);
	new_path += ";";
	new_path += Content::GetPath();
	new_path += "/?.lua";

	lua_pop(lState, 1);
	lua_pushstring(lState, new_path.c_str());
	lua_setfield(lState, -2, "path");
	lua_pop(lState, 1);
}

void sLuaSystem::BindEngine()
{
	static const luaL_reg engineFuncs[] =
	{
		{"Log", __l_log__},
		{0, 0}
	};

	luaL_openlib(lState, "Engine", engineFuncs, 0);
	lua_pop(lState, 1);
}

//Lua binding functions :)

int __l_log__(lua_State* L)
{
	string msg = "";
	string bg = "transparent";
	string clr = "#eee";

	//set our lua stack to hold exactly 3 values. If the lua caller only provided one argument for example, our stack would be <"some message", NULL, NULL>
	lua_settop(L, 3);
	if (lua_isstring(L, 1))
	{
		msg = lua_tostring(L, 1);
	}
	if (lua_isstring(L, 2))
	{
		bg = lua_tostring(L, 2);
	}
	if (lua_isstring(L, 3))
	{
		clr = lua_tostring(L, 3);
	}

	Terminal.Custom(msg, bg, clr);

	return 0;
}
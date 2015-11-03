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
#include "../Utilities/standardIncludes.h"
#include "../Modules/mAll.h"

sLuaSystem::sLuaSystem():
	hasMainBeenCalled(false)
{
	lState = luaL_newstate();
	luaL_openlibs(lState);
	SetPackagePath();
    
    mEngine::Bind(lState);
    mInput::Bind(lState);
    mEntity::Bind(lState);

	string path;
	Content::CreateFilePath("main.lua", &path);

	if (luaL_loadfile(lState, path.c_str()))
	{
        Terminal.Error("Could not open main.lua - The program will not run correctly");
		return;
	}

    lua_atpanic(lState, LuaPanic);
    
	//try to parse main.lua
	if (lua_pcall(lState, 0, 0, 0) != 0)
	{
        Terminal.LuaError(lua_tostring(lState, -1));
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
			Terminal.LuaError(string(lua_tostring(lState, -1)));
		}
        lua_settop(lState, 0);

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
		Terminal.LuaError(string(lua_tostring(lState, -1)));	}
    lua_settop(lState, 0);
}

void sLuaSystem::Attempt(string command)
{
    if(luaL_dostring(lState, command.c_str()) != 0){
        Terminal.LuaError(string(lua_tostring(lState, -1)));
    }
}

//TODO(robin): make this use terminal logging?
void sLuaSystem::Dump(lua_State* L)
{
    int i;
    int top = lua_gettop(L);
    
    printf("--=[LUA STACK DUMP]=--\n");
    printf("total in stack %d\n",top);
    
    for (i = 1; i <= top; i++)
    {  /* repeat for each level */
        int t = lua_type(L, i);
        switch (t) {
            case LUA_TSTRING:  /* strings */
                printf("string: '%s'\n", lua_tostring(L, i));
                break;
            case LUA_TBOOLEAN:  /* booleans */
                printf("boolean %s\n",lua_toboolean(L, i) ? "true" : "false");
                break;
            case LUA_TNUMBER:  /* numbers */
                printf("number: %g\n", lua_tonumber(L, i));
                break;
            default:  /* other values */
                printf("%s\n", lua_typename(L, t));
                break;
        }
        printf("  ");  /* put a separator */
    }
    printf("----------------------\n");/* end the listing */
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

int sLuaSystem::LuaPanic(lua_State* L)
{
    Terminal.LuaError("PANIC!");
    Terminal.LuaError(string(lua_tostring(L, -1)));
    Terminal.Warning("The program will exit...");
    
    //TODO close the lua state but keep the engine running
    exit(0);
    return 0;
}
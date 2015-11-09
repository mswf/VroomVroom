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
#include <stdlib.h>
#include "../Utilities/command.h"


sLuaSystem::sLuaSystem():
	hasMainBeenCalled(false),
	lState(NULL),
    atomPath("")
{

}

sLuaSystem::~sLuaSystem()
{
	//TODO(robin) release resources
}

void sLuaSystem::Init()
{
	lState = luaL_newstate();
	luaL_openlibs(lState);
	SetPackagePath();

	mEngine::Bind(lState);
	mInput::Bind(lState);
	mEntity::Bind(lState);
	mUiWindow::Bind(lState);

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
		HandleError(lState);
		return;
	}
}

void sLuaSystem::Main()
{
	if (!hasMainBeenCalled)
	{
		lua_getglobal(lState, "Game");
		lua_getfield(lState, -1, "main");
		if (lua_isfunction(lState, -1) && lua_pcall(lState, 0, 0, 0) != 0)
		{
            HandleError(lState);
		}
		lua_settop(lState, 0);

		hasMainBeenCalled = true;
	}
}

void sLuaSystem::Update(float dt)
{
	lua_getglobal(lState, "Game");
	lua_getfield(lState, -1, "update");
	lua_pushnumber(lState, dt);
	if (lua_isfunction(lState, -1) && lua_pcall(lState, 1, 0, 0) != 0)
	{
		 HandleError(lState);
	}
	lua_settop(lState, 0);
}

void sLuaSystem::SendReloadCallback( const string& filePath )
{
	lua_getglobal(lState, "Game");
	lua_getfield(lState, -1, "onFileChanged");
	lua_pushlstring( lState, filePath.c_str(), filePath.size() );
	if (lua_pcall(lState, 1, 0, 0) != 0)
	{
		HandleError(lState);
	}
	lua_settop(lState, 0);
}


void sLuaSystem::Attempt(string command)
{
	if (luaL_dostring(lState, command.c_str()) != 0)
	{
		HandleError(lState);
	}
}

//TODO(robin): make this use terminal logging?
void sLuaSystem::Dump(lua_State* L)
{
	int i;
	int top = lua_gettop(L);

	printf("--=[LUA STACK DUMP]=--\n");
	printf("total in stack %d\n", top);

	for (i = 1; i <= top; i++)
	{
		/* repeat for each level */
		int t = lua_type(L, i);
		switch (t)
		{
			case LUA_TSTRING:  /* strings */
				printf("string: '%s'\n", lua_tostring(L, i));
				break;
			case LUA_TBOOLEAN:  /* booleans */
				printf("boolean %s\n", lua_toboolean(L, i) ? "true" : "false");
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

void sLuaSystem::SetAtomPath(string path)
{
    atomPath = path;
}

void sLuaSystem::HandleError(lua_State* L)
{
    string error = lua_tostring(L, -1);
    if(atomPath == "")
    {
        Terminal.LuaError(error);
    }
    else
    {
		// start looking from the v"fifth"v character onward, to evade the colon commonly found in Windows filepaths
        int indexA = error.find(":", 5);
        int indexC = error.find("/");
        int indexB = error.find(":",indexA+1);
        
        string filePath;
        Content::CreateFilePath(error.substr(indexC, indexA).c_str(), &filePath);
        
        string lineNumber = error.substr(indexA+1,indexB-(indexA+1));
        
        string linkMessage = "<a href='' onclick=\"";
        linkMessage += "ipc.send('handleCommand',':openfile;";
        linkMessage += filePath;
        linkMessage += ";";
        linkMessage += lineNumber;
        linkMessage += ";');return false;\">";
        linkMessage += error;
        linkMessage += "</a>";
        
        Terminal.LuaLinkedError(linkMessage, error);
    }
}

void sLuaSystem::OpenAtom(string path, int lineNumber)
{
    if(atomPath == "")
    {
        return;
    }
    string commandString = "";
    commandString += atomPath;
    commandString += " ";
    commandString += path;
    commandString += ":";
    commandString += std::to_string(lineNumber);
    RunCommand(commandString);
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

	//TODO(robin) close the lua state but keep the engine running
	exit(0);
	return 0;
}
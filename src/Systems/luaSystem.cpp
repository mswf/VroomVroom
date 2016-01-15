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
#include "../Utilities/helperFunctions.h"

#include <vector>
#include "../Components/entity.h"
#include "../Components/CLua.h"

string sLuaSystem::atomPath("");

sLuaSystem::sLuaSystem():
	lState(nullptr),
    hasMainBeenCalled(false),
	allowCalls(true)
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
	
	lua_newtable(lState);
	lua_setglobal(lState, "Engine");

	mLog::Bind(lState);
	mInput::Bind(lState);
	mEntity::Bind(lState);
	mUi::Bind(lState);
	mMaterial::Bind(lState);
	mModel::Bind(lState);
	mShader::Bind(lState);
	mMeshRenderer::Bind(lState);
	mSystem::Bind(lState);
	mDebugRenderer::Bind(lState);
	mCamera::Bind(lState);
	mImport::Bind(lState);
	mAudio::Bind(lState);
	mWindow::Bind(lState);
	mLight::Bind(lState);

	string path;
	Content::CreateFilePath("main.lua", &path);

	if (luaL_loadfile(lState, path.c_str()))
	{
		Terminal.Error("Could not open main.lua - The program will not run correctly");
		return;
	}

	lua_atpanic(lState, LuaPanic);

	//try to parse main.lua
	Call(lState, 0, 0);
}

void sLuaSystem::Main()
{
	if (!hasMainBeenCalled)
	{
		lua_getglobal(lState, "Game");
		lua_getfield(lState, -1, "main");
		if( lua_isnil(lState, -1))
		{
			Terminal.Warning("Game.main does not exist!");
			lua_settop(lState, 0);
			return;
		}
		
        Call(lState, 0, 0);
        
        lua_settop(lState, 0);
		hasMainBeenCalled = true;
	}
}

void sLuaSystem::Update(float dt)
{
	lua_getglobal(lState, "Game");
	lua_getfield(lState, -1, "update");
	
	if( lua_isnil(lState, -1))
	{
		Terminal.Warning("Game.update does not exist!");
		Halt();
		lua_settop(lState, 0);
		return;
	}
	
	lua_pushnumber(lState, dt);
    
    Call(lState, 1, 0);
	
    lua_settop(lState, 0);
	
	
	//TODO(robin): Lua Components should register themselves to the LuaSystem, rather than searching for them every update
	std::vector<Entity *> entities;
	Entity::GetEntities<CLua>(entities);
	
	std::vector<Entity *>::iterator it = entities.begin();
	for (; it != entities.end(); ++it)
	{
		CLua* comp = Entity::GetComponent<CLua>((*it));
		comp->Update(dt);
	}
}

void sLuaSystem::SendReloadCallback( const string& filePath )
{
	lua_getglobal(lState, "Game");
	lua_getfield(lState, -1, "onFileChanged");
	lua_pushlstring( lState, filePath.c_str(), filePath.size() );
	
    bool prevAllowCalls = allowCalls;
    allowCalls = true;
    bool success = Call(lState, 1, 1);
	if (success)
	{
    	allowCalls = prevAllowCalls;
	}
	
    bool reloaded = false;
    if(lua_isboolean(lState, -1))
    {
       reloaded = lua_toboolean(lState, -1);
    }
    if(reloaded)
    {
        Resume();
    }
    
	lua_settop(lState, 0);
}

void sLuaSystem::EventCallback(const char* name, int argCount, int* args)
{
	lua_getglobal(lState, "Game");
	lua_getfield(lState, -1, name);
	
	if( lua_isnil(lState, -1))
	{
		Terminal.Warning("Game."+string(name)+" does not exist!");
		lua_settop(lState, 0);
		return;
	}
	
	int ii = 0;
	while(ii < argCount)
	{
		lua_pushnumber(lState, *args);
		args++;
		ii++;
	}
	
	Call(lState, argCount, 0);
	
	lua_settop(lState, 0);
}


void sLuaSystem::Attempt(string command)
{
	if(luaL_loadstring(lState, command.c_str()) == 0)
   	{
		bool prevAllowCalls = allowCalls;
		allowCalls = true;
		Call(lState, 0, LUA_MULTRET);
		allowCalls = prevAllowCalls;
	}
}

bool sLuaSystem::Call(lua_State* L, int argCount, int returnCount)
{
    //Dump(L);
    if(!allowCalls)
    {
        return false;
    }
    lua_pushcfunction(L, LuaError);
    lua_insert(L, 1);
    //lua_pop(L,1);
    int errorHandlePos = 1;
    
    //Dump(L);
    int result = lua_pcall(L, argCount, returnCount, errorHandlePos);
    
    if (result == 0)
    {
        return true;
    } else if (result == LUA_ERRRUN){
        //HandleError(L);
        Halt();
        return false;
    } else if (result == LUA_ERRMEM){
        Terminal.LuaError("I'm throwing a very non-speficic Memory Error.\n  xoxo Bobn");
        return false;
    } else if (result == LUA_ERRERR){
        Terminal.LuaError("I'm throwing a very non-speficic Error-handler Error.\n  xoxo Bobn");
        return false;
    }
    //we should never reach this point
    return false;
}

void sLuaSystem::Resume()
{
    if(!allowCalls)
    {
        allowCalls = true;
        Terminal.Log("Resuming Lua Calls", true);
    }

}

void sLuaSystem::Halt()
{
    if(allowCalls)
    {
        allowCalls = false;
        Terminal.Log("Halting Lua Calls", true);
    }

}

void sLuaSystem::Dump(lua_State* L) const
{
	int i;
	int top = lua_gettop(L);
	
	string dumpString = "[LUA STACK DUMP]\n";
	
	dumpString += "\t\ttotal in stack "+std::to_string(top)+"\n";

	for (i = 1; i <= top; i++)
	{
		/* repeat for each level */
		int t = lua_type(L, i);
		switch (t)
		{
			case LUA_TSTRING:  /* strings */
				dumpString += "...string: " + string(lua_tostring(L, i)) + "\n";
				break;
			case LUA_TBOOLEAN:  /* booleans */
				dumpString += "...boolean: " + string((lua_toboolean(L, i) ? "true" : "false")) + "\n";
				break;
			case LUA_TNUMBER:  /* numbers */
				dumpString += "...number: " + std::to_string(lua_tonumber(L, i)) + "\n";
				break;
			default:  /* other values */
				dumpString += "..." + string(lua_typename(L, t)) + "\n";
				break;
		}
	}
	dumpString += "\t\t----------------------\n";
	Terminal.Custom(dumpString, "#E2928E", "#fcf4f3");
}

void sLuaSystem::SetAtomPath(string path)
{
    atomPath = path;
}

void sLuaSystem::HandleError(lua_State* L)
{

}

void sLuaSystem::OpenAtom(string path, int lineNumber) const
{
    if(atomPath == "")
    {
        return;
    }
#ifdef _WIN32
    string subPath = path;
    Content::CreateFilePath(subPath.c_str(), &path);
#endif
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
void sLuaSystem::SetPackagePath() const
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
	LuaError(L);
	Terminal.Warning("The program will exit...");

	//TODO(robin) close the lua state but keep the engine running
	exit(0);
	return 0;
}

int sLuaSystem::LuaError(lua_State* L)
{
    string error = lua_tostring(L, -1);

    lua_Debug entry;
    int depth = 0;
    Terminal.LuaError(error);

    while (lua_getstack(L, depth, &entry))
    {
        int status = lua_getinfo(L, "Sln", &entry);
        assert(status);
        
        string stackLine = CreateStackLine(entry.short_src, entry.currentline, entry.name ? entry.name : "?");
        if(entry.source[0] == '@')
        {
            string linkedError = LuaSystem.CreateErrorLink(stackLine, string(entry.source), entry.currentline);
            Terminal.LuaLinkedError(linkedError, stackLine);
        }
        else
        {
            Terminal.LuaError(stackLine);
        }
        
        printf("%s\n", stackLine.c_str());
        depth++;
    }
    return 1;

    
}

string sLuaSystem::CreateStackLine(string source, int line, string name) {
	
#ifdef _WIN32
	int slashIndex = source.find("/");
	source = source.substr(slashIndex+1);
#else
	source = source.substr(1);
#endif

    string stackLine = "\t@ "+source;
    stackLine += "("+std::to_string(line)+"): ";
    stackLine += name;
    
    return stackLine;
}

string sLuaSystem::CreateErrorLink(string message, string fileName, int lineNumber) {
	
    string filePath;
#ifdef _WIN32
	int slashIndex = fileName.find("/");
    filePath = fileName.substr(slashIndex+1);
#else
    filePath = fileName.substr(1);
#endif

    
    string errorLink = "<a href='' onclick=\"ipc.send('handleCommand',{cmd:':openfile;";
    errorLink += filePath;
    errorLink += ";";
    errorLink += std::to_string(lineNumber);
    errorLink += ";',dontSave:true});return false;\">";
    errorLink += message;
    errorLink += "</a>";

    
    return errorLink;
}
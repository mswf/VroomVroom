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

namespace LuaSystem
{
    void Init()
    {
        if(isInitialized)
        {
            return;
        }
        
        lState = luaL_newstate();
        luaL_openlibs(lState);
        __SetPackagePath__();
        
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
        
        lua_getglobal(lState, "Game");
        lua_getfield(lState, -1, "main");
        if (lua_pcall(lState, 0, 0, 0) != 0)
        {
            //TODO replace couts with proper Console::Error or even Console::LuaError?
            std::cout << "lua error: " << lua_tostring(lState, -1) << std::endl;
        }


        isInitialized = true;
    }
    
    void Update(int dt)
    {
        if(!isInitialized)
        {
            return;
        }
        
        lua_getglobal(lState, "Game");
        lua_getfield(lState, -1, "update");
        lua_pushnumber(lState, dt);
        if (lua_pcall(lState, 1, 0, 0) != 0)
        {
            //TODO replace couts with proper Console::Error or even Console::LuaError?
            std::cout << "lua error: " << lua_tostring(lState, -1) << std::endl;
        }
    }
    
    namespace
    {
        /*
         *  Tells the Lua state to look in the content directory whenever 'require' is used in Lua scripts.
         */
        void __SetPackagePath__()
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
    }
}

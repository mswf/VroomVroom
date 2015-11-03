//
//  LuaSystem.hpp
//  VroomVroom
//
//  Created by Robin Zaagsma on 09/10/15.
//  Copyright © 2015 Valentinas Rimeika. All rights reserved.
//

#ifndef luaSystem_h
#define luaSystem_h

#include "../Utilities/typedef.h"
#include "../Patterns/singleton.h"

#define LuaSystem sLuaSystem::getInstance()

class lua_State;

class sLuaSystem : public Singleton<sLuaSystem>
{
    public:
        sLuaSystem();
        ~sLuaSystem();
        void Init();
        void Main();
        void Update(int);
    
        void Attempt(string);
        void Dump(lua_State*);
    
    private:
        void SetPackagePath();
    
        lua_State* lState;
        bool hasMainBeenCalled;
    
        static int LuaPanic(lua_State* L);
};

#endif /* luaSystem_h */

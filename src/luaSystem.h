//
//  LuaSystem.hpp
//  VroomVroom
//
//  Created by Robin Zaagsma on 09/10/15.
//  Copyright © 2015 Valentinas Rimeika. All rights reserved.
//

#ifndef luaSystem_h
#define luaSystem_h

#include "standardIncludes.h"
#include <lua.hpp>

namespace LuaSystem
{
    void Init();
    void Update(int);
    
    namespace
    {
        bool isInitialized(false);
        lua_State* lState(NULL);

        void __SetPackagePath__();
    }
}

#endif /* luaSystem_h */

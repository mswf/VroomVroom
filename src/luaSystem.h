//
//  LuaSystem.hpp
//  VroomVroom
//
//  Created by Robin Zaagsma on 09/10/15.
//  Copyright © 2015 Valentinas Rimeika. All rights reserved.
//

#ifndef luaSystem_h
#define luaSystem_h

#include "patterns/singleton.h"

#define LuaSystem sLuaSystem::getInstance()

class lua_State;


class sLuaSystem : public Singleton<sLuaSystem>
{
    public:
    sLuaSystem();
    ~sLuaSystem();
    void Main();
    void Update(int);
    
    private:
    void SetPackagePath();
    void BindEngine();
    
    
    
    lua_State* lState;
    bool hasMainBeenCalled;
};

//lua binding functions :)
int __l_log__(lua_State*);


#endif /* luaSystem_h */

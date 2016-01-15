//
//  wEngine.cpp
//  VroomVroom
//
//  Created by Robin Zaagsma on 13/10/15.
//  Copyright © 2015 Valentinas Rimeika. All rights reserved.
//

#include "mLog.h"
#include "../Utilities/typedef.h"
#include "console.h"

void mLog::Bind(lua_State* L)
{
	lua_getglobal(L, "Engine");
	lua_pushcfunction(L, lw_log__);
	lua_setfield(L, -2, "log");
}

lFuncImp(mLog, log)
{
    //default values
    string msg = "";
    string bg = "transparent";
    string clr = "#eee";
    
    //set our lua stack to hold exactly 3 values. If the lua caller only provided one argument for example, our stack would be <"some message", nullptr, nullptr>
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
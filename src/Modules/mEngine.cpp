//
//  wEngine.cpp
//  VroomVroom
//
//  Created by Robin Zaagsma on 13/10/15.
//  Copyright © 2015 Valentinas Rimeika. All rights reserved.
//

#include "mEngine.h"
#include "typedef.h"
#include "console.h"

void mEngine::Bind(lua_State* L)
{
    lStart(Engine)
        lBind(log)
    lEnd(Engine)
}

lFuncImp(mEngine, log)
{
    //default values
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
//
//  mUi.cpp
//  VroomVroom
//
//  Created by Robin Zaagsma on 03/11/15.
//  Copyright © 2015 Valentinas Rimeika. All rights reserved.
//

#include "mUiWindow.h"
#include "../ImGUI/imgui.h"
#include "../Utilities/typedef.h"

void mUiWindow::Bind(lua_State* L){
    lStart(UiWindow)
        lBind(create)
    lEnd(UiWindow)
}

lFuncImp(mUiWindow, create){
    lgString(title, 1, "new window")
    lgInt(width, 2, 400)
    lgInt(height, 3, 250)
    
    lua_settop(L, 0);
    
    lua_newtable(L);
    
    lua_pushstring(L, title.c_str());
    lua_setfield(L, -2, "title");
    
    lua_pushnumber(L, width);
    lua_setfield(L, -2, "width");
    
    lua_pushnumber(L, height);
    lua_setfield(L, -2, "height");
    
    lua_pushboolean(L, false);
    lua_setfield(L, -2, "resizable");
    
    return 1;
}

//TODO:
/*
    create a metatable and a proxy table such that everytime a property on the table is changed, the corresponding cpp instance gets updated.
 */


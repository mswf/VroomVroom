//
//  mUi.cpp
//  VroomVroom
//
//  Created by Robin Zaagsma on 03/11/15.
//  Copyright © 2015 Valentinas Rimeika. All rights reserved.
//

#include "mUiWindow.h"
#include "../Systems/uiSystem.h"
#include "../Systems/luaSystem.h"
#include "../Utilities/typedef.h"
#include "../console.h"

void mUiWindow::Bind(lua_State* L){
    lStart(UiWindow)
        lBind(create)
    lEnd(UiWindow)
    
    luaL_newmetatable(L, "__mtUiWindow");
    
    const luaL_reg __mtUiWindow_methods[] =
    {
        {"__index", lw_mtIndex__},
        {"__newindex", lw_mtNewIndex__},
        lBind(addText)
        lBind(addButton)    };
    
    luaL_openlib(L, 0, __mtUiWindow_methods, 0);

}

lFuncImp(mUiWindow, create){
    lgString(title, 1, "new window")
    lgInt(width, 2, 400)
    lgInt(height, 3, 250)
    
    lua_settop(L, 0);
    
    uiWindow* window = UiSystem.CreateWindow();
    
    window->title = title;
    window->width = width;
    window->height = height;
    
    lua_newtable(L);
    
    //TODO PLS PLS PLS this memory needs to be managed properly,
    //perhaps add some garbage regular userdata to be notified of garbage collection?
    lua_pushlightuserdata(L, window);
    lua_setfield(L, -2, "__coreWindow__");
    
    lua_newtable(L);

    lstString("title", title.c_str());
    lstNumber("width", width)
    lstNumber("height", height)
    lstNumber("x", 0)
    lstNumber("y", 0)
    
    lstBoolean("resizable", window->resizable)
    lstBoolean("movable", window->movable)
    lstBoolean("closable", window->closable)
    lstBoolean("collapsable", window->collapsable)
    
    lua_setfield(L, -2, "__coreProperties__");
    
    //TODO create (light?) userdata to the window, garbage collector et all
    //TODO lua references
    
    luaL_getmetatable(L, "__mtUiWindow");
    lua_setmetatable(L, -2);
    return 1;
}

lFuncImp(mUiWindow, mtIndex)
{
    //first try our core properties
    lua_getfield(L, 1, "__coreProperties__");
    lua_getfield(L, -1, lua_tostring(L, -2));
    if(!lua_isnil(L, -1))
    {
        return 1;
    }
    
    //we failed, so we pop all the values we put on the stack in our first check
    lua_settop(L, 2);
    //then try the methods in our metatable
    luaL_getmetatable(L,"__mtUiWindow");
    lua_getfield(L, -1, lua_tostring(L, -2));
    if(!lua_isnil(L, -1))
    {
        return 1;
    }

    lua_pushnil(L);
    return 1;
}

lFuncImp(mUiWindow, mtNewIndex)
{
    //first check if this key exists in __coreProperties__
    lua_getfield(L, 1, "__coreProperties__");
    lua_getfield(L, -1, lua_tostring(L, -3));
    if(!lua_isnil(L, -1))
    {
        lua_pushvalue(L,-2);
        lua_pushvalue(L,3);
        lua_setfield(L, -2, lua_tostring(L, 2));
        
        lua_getfield(L, 1, "__coreWindow__");
        uiWindow* window = (uiWindow*)lua_touserdata(L,-1);
        lua_pop(L, 1);
        
        lua_getfield(L, -1, "collapsable");
        window->collapsable = lua_toboolean(L, -1);
        lua_pop(L,1);
        
        lua_getfield(L, -1, "resizable");
        window->resizable = lua_toboolean(L, -1);
        lua_pop(L,1);
        
        lua_getfield(L, -1, "closable");
        window->closable = lua_toboolean(L, -1);
        lua_pop(L,1);
        
        lua_getfield(L, -1, "movable");
        window->movable = lua_toboolean(L, -1);
        lua_pop(L,1);
        
        lua_getfield(L, -1, "width");
        window->width = lua_tonumber(L, -1);
        lua_pop(L,1);
        
        lua_getfield(L, -1, "height");
        window->height = lua_tonumber(L, -1);
        lua_pop(L,1);
        
        lua_getfield(L, -1, "x");
        window->x = lua_tonumber(L, -1);
        lua_pop(L,1);
        
        lua_getfield(L, -1, "y");
        window->y = lua_tonumber(L, -1);
        lua_pop(L,1);
        
        lua_getfield(L, -1, "title");
        window->title = lua_tostring(L, -1);
        lua_pop(L,1);
        
        return 0;
    }
    //we failed, so we pop all the values we put on the stack in our first check
    lua_settop(L, 3);
    //it was not a core property, so add it to the base table (we use rawset to bypass an infinite __newindex loop)
    lua_rawset(L, 1);
    return 0;
}

lFuncImp(mUiWindow, addText)
{
    //TODO create a table whose properties are modifiable,
    //TODO add a reference to said table in the uiWindowElement
    lgString(content,2,"lorum ipsum");
    lua_getfield(L, 1, "__coreWindow__");
    uiWindow* window = (uiWindow*)lua_touserdata(L,-1);
    lua_pop(L, 1);
    
    UiSystem.AddText(window)->content = content;
    
    return 0;
}

lFuncImp(mUiWindow, addButton)
{
    Terminal.Warning("uiWindow:addButton is not fully implemented yet");
    
    lgString(label,2,"butts");
    lua_getfield(L, 1, "__coreWindow__");
    uiWindow* window = (uiWindow*)lua_touserdata(L,-1);
    lua_pop(L, 1);
    
    UiSystem.AddButton(window)->label = label;
    
    return 0;

}

//TODO:
/*
    userdata garbage collection
 */


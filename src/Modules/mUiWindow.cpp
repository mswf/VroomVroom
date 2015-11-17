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
    
    luaL_newmetatable(L, "__mtUiElement");
    
    const luaL_reg __mtUiElement_methods[] =
    {
        {"__index", lw_mtIndex__},
        {"__newindex", lw_mtNewIndex__},
        lBind(addText)
		lBind(addButton)
		{0, 0}
	};
    
    luaL_openlib(L, 0, __mtUiElement_methods, 0);

	UiSystem.SetLuaState(L);
}

void mUiWindow::HandleButtonCallback(lua_State* L, int tableKey)
{
	
	lua_pushnumber(L, tableKey);
	lua_gettable(L, LUA_REGISTRYINDEX);
	
	LuaSystem.Dump(L);
	
	
	
	lua_getfield(L, -1, "callback");
	if(lua_isnil(L, -1) == 0)
	{
		lua_pushvalue(L, 1);
		LuaSystem.Call(L, 1, 0);
	}

}

lFuncImp(mUiWindow, create){
    lgString(title, 1, "new window")
    lgInt(width, 2, 400)
    lgInt(height, 3, 250)
    
    lua_settop(L, 0);
    
	uiWindow* window = UiSystem.ConstructWindow();
	uiWindowPropertiesElement* properties = (uiWindowPropertiesElement*)window->propertiesElement->data;
    
    properties->title = title;
    properties->width = width;
    properties->height = height;
	
    lua_newtable(L);
    
    //TODO(robin) PLS PLS PLS this memory needs to be managed properly,
    //perhaps add some garbage regular userdata to be notified of garbage collection?
    lua_pushlightuserdata(L, window->propertiesElement);
    lua_setfield(L, -2, "__coreElement__");
	
	lua_pushlightuserdata(L, window);
	lua_setfield(L, -2, "__coreWindow__");

	
    lua_newtable(L);

    lstString("title", title.c_str());
    lstNumber("width", width)
    lstNumber("height", height)
    lstNumber("x", 0)
    lstNumber("y", 0)
    
    lstBoolean("resizable", properties->resizable)
    lstBoolean("movable", properties->movable)
    lstBoolean("closable", properties->closable)
    lstBoolean("collapsable", properties->collapsable)
    
    lua_setfield(L, -2, "__coreProperties__");
    
    luaL_getmetatable(L, "__mtUiElement");
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
    luaL_getmetatable(L,"__mtUiElement");
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
		lua_getfield(L, 1, "__coreElement__");
        uiWindowElement* window = (uiWindowElement*)lua_touserdata(L,-1);
        lua_pop(L, 1);
		
		if(lua_type(L, 3) == LUA_TSTRING)
		{
			UiSystem.SetNamedProperty(window, lua_tostring(L, 2), string(lua_tostring(L, 3)));
			return 0;
		}
		if(lua_type(L, 3) == LUA_TBOOLEAN)
		{
			UiSystem.SetNamedProperty(window, lua_tostring(L, 2), lua_toboolean(L, 3));
			return 0;
		}
		if(lua_type(L, 3) == LUA_TNUMBER)
		{
			UiSystem.SetNamedProperty(window, lua_tostring(L, 2), lua_tonumber(L, 3));
			return 0;
		}
	}
    //we failed, so we pop all the values we put on the stack in our first check
    lua_settop(L, 3);
    //it was not a core property, so add it to the base table (we use rawset to bypass an infinite __newindex loop)
    lua_rawset(L, 1);
    return 0;
}

lFuncImp(mUiWindow, addText)
{
	lua_settop(L, 2);
    lgString(startText,2,"lorum ipsum");
    lua_getfield(L, 1, "__coreWindow__");
    uiWindow* window = (uiWindow*)lua_touserdata(L,-1);
	lua_pop(L, 1);
	
	
	uiWindowTextElement* text = UiSystem.AddText(window);
	
	text->text = startText;
	
	lua_newtable(L);
	lua_pushlightuserdata(L, text->parent);
	lua_setfield(L, -2, "__coreElement__");
	
	lua_newtable(L);
	lstString("text", text->text.c_str());
	lua_setfield(L, -2, "__coreProperties__");
	
	luaL_getmetatable(L, "__mtUiElement");
	lua_setmetatable(L, -2);
	
	lua_pushvalue(L, 1);
	lua_setfield(L, -2, "parent");
    return 1;
}

lFuncImp(mUiWindow, addButton)
{
	lua_settop(L, 3);
    lgString(label, 2, "butts");
	//lgFunc(callback, 3, -1);
	
    lua_getfield(L, 1, "__coreWindow__");
    uiWindow* window = (uiWindow*)lua_touserdata(L,-1);
    lua_pop(L, 1);
	
	uiWindowButtonElement* button = UiSystem.AddButton(window);
    
    button->label = label;
	
	lua_newtable(L);
	lua_pushlightuserdata(L, button->parent);
	lua_setfield(L, -2, "__coreElement__");
	
	lua_newtable(L);
	lstString("label", button->label.c_str());
	lua_setfield(L, -2, "__coreProperties__");
	
	lua_pushvalue(L, 3);
	lua_setfield(L, -2, "callback");

	luaL_getmetatable(L, "__mtUiElement");
	lua_setmetatable(L, -2);

	lua_pushvalue(L, -1);
	button->luaTableKey = luaL_ref(L, LUA_REGISTRYINDEX);
	
	lua_pushvalue(L, 1);
	lua_setfield(L, -2, "parent");

    return 1;
}

//TODO(robin):
/*
    userdata garbage collection
 */


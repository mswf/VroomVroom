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
	//TODO(tobin): Go do some metatable inheritance witchery
    lStart(UiWindow)
        lBind(create)
    lEnd(UiWindow)
    
    luaL_newmetatable(L, "__mtUiElement");
    const luaL_reg __mtUiElement_methods[] =
    {
        {"__index", lw_mtIndex__},
        {"__newindex", lw_mtNewIndex__},
		{0, 0}
	};
    luaL_openlib(L, 0, __mtUiElement_methods, 0);
	
	
	luaL_newmetatable(L, "__mtUiWindow");
	const luaL_reg __mtUiWindow_methods[] =
	{
		{"__index", lw_mtIndex__},
		{"__newindex", lw_mtNewIndex__},
		lBind(addText)
		lBind(addButton)
		lBind(addTree)
		lBind(addInputText)
		lBind(addCheckbox)
		lBind(addSlider)
		lBind(close)
		{0, 0}
	};
	luaL_openlib(L, 0, __mtUiWindow_methods, 0);
	
	
	luaL_newmetatable(L, "__mtUiContainer");
	const luaL_reg __mtUiContainer_methods[] =
	{
		{"__index", lw_mtIndex__},
		{"__newindex", lw_mtNewIndex__},
		lBind(addText)
		lBind(addButton)
		lBind(addTree)
		lBind(addInputText)
		lBind(addCheckbox)
		lBind(addSlider)
		{0, 0}
	};
	luaL_openlib(L, 0, __mtUiContainer_methods, 0);

	
	UiSystem.SetLuaState(L);
}

void mUiWindow::UnreferenceTable(lua_State* L, int tableKey)
{
	lua_pushnumber(L, tableKey);
	lua_gettable(L, LUA_REGISTRYINDEX);
	lstBoolean("__exists__", false);
	
	luaL_unref(L, LUA_REGISTRYINDEX, tableKey);
}

void mUiWindow::HandleButtonCallback(lua_State* L, int tableKey)
{
	
	lua_pushnumber(L, tableKey);
	lua_gettable(L, LUA_REGISTRYINDEX);
	
	
	lua_getfield(L, -1, "callback");
	if(lua_isnil(L, -1) == 0)
	{
		lua_pushvalue(L, 1);
		LuaSystem.Call(L, 1, 0);
	}
}

void mUiWindow::HandleWindowClose(lua_State* L, int tableKey)
{
	
	lua_pushnumber(L, tableKey);
	lua_gettable(L, LUA_REGISTRYINDEX);
	
	
	lua_getfield(L, -1, "onClose");
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
    
    window->title = title;
    window->width = width;
    window->height = height;
	
    lua_newtable(L);
	
	lua_pushvalue(L, -1);
	window->luaTableKey = luaL_ref(L, LUA_REGISTRYINDEX);
    
    //TODO(robin) PLS PLS PLS this memory needs to be managed properly,
    //perhaps add some garbage regular userdata to be notified of garbage collection?
    lua_pushlightuserdata(L, window);
    lua_setfield(L, -2, "__coreElement__");
	
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
	
	lstBoolean("visible", window->visible)
    
    lua_setfield(L, -2, "__coreProperties__");
	
	lstBoolean("__exists__", true);
    
    luaL_getmetatable(L, "__mtUiWindow");
    lua_setmetatable(L, -2);
	
    return 1;
}

lFuncImp(mUiWindow, close)
{
	lua_getfield(L, 1, "__coreElement__");
	uiWindow* window = (uiWindow*)lua_touserdata(L,-1);
	lua_pop(L, 1);

	UiSystem.RemoveWindow(window);
	
	return 0;
}

lFuncImp(mUiWindow, mtIndex)
{
    //first try our core properties
    lua_getfield(L, 1, "__coreProperties__");
    lua_getfield(L, -1, lua_tostring(L, 2));
    if(!lua_isnil(L, -1))
    {
		//then check if our cpp side of the uiElement still exists
		lua_getfield(L, 1, "__exists__");
		if(lua_toboolean(L, -1))
		{
			lua_pop(L, 1);
			//if it does get the value from our cpp element
			lua_getfield(L, 1, "__coreElement__");
			uiElement* node = (uiElement*)lua_touserdata(L,-1);
			lua_pop(L, 1);
			if(lua_type(L, -1) == LUA_TSTRING)
			{
				lua_pushstring(L, UiSystem.GetNamedProperty<string>(node, lua_tostring(L, 2)).c_str());
			}
			else if(lua_type(L, -1) == LUA_TBOOLEAN)
			{
				lua_pushboolean(L, UiSystem.GetNamedProperty<bool>(node, lua_tostring(L, 2)));
			}
			else if(lua_type(L, -1) == LUA_TNUMBER)
			{
				lua_pushnumber(L, UiSystem.GetNamedProperty<double>(node, lua_tostring(L, 2)));
			}
			else
			{
				lua_pushnil(L);
				return 1;
			}
			lua_pushvalue(L, -1);
			lua_setfield(L, 3, lua_tostring(L, 2));
			return 1;
		}
		else
		{
			//otherwise just return the current value in our __coreProperties__ table
			lua_getfield(L, 1, "__coreProperties__");
			lua_getfield(L, -1, lua_tostring(L, 2));
			return 1;
		}
    }
    
    //we failed, so we pop all the values we put on the stack in our first check
    lua_settop(L, 2);
    //then try the methods in our metatable
	lua_getmetatable(L, 1);
    lua_getfield(L, -1, lua_tostring(L, -2));
	return 1;
}

lFuncImp(mUiWindow, mtNewIndex)
{
    //first check if this key exists in __coreProperties__
    lua_getfield(L, 1, "__coreProperties__");
    lua_getfield(L, -1, lua_tostring(L, -3));
    if(!lua_isnil(L, -1))
    {
		//set the property in our properties field
        lua_pushvalue(L,-2);
        lua_pushvalue(L,3);
        lua_setfield(L, -2, lua_tostring(L, 2));
		
		//check if the cpp uiElement still exists, and if so update the value there too
		lua_getfield(L, 1, "__exists__");
		if(lua_toboolean(L, -1))
		{
			lua_pop(L, 1);
		
			lua_getfield(L, 1, "__coreElement__");
			uiElement* node = (uiElement*)lua_touserdata(L,-1);
			lua_pop(L, 1);
			
			if(lua_type(L, 3) == LUA_TSTRING)
			{
				UiSystem.SetNamedProperty<string>(node, lua_tostring(L, 2), string(lua_tostring(L, 3)));
				return 0;
			}
			if(lua_type(L, 3) == LUA_TBOOLEAN)
			{
				UiSystem.SetNamedProperty<bool>(node, lua_tostring(L, 2), lua_toboolean(L, 3));
				return 0;
			}
			if(lua_type(L, 3) == LUA_TNUMBER)
			{
				UiSystem.SetNamedProperty<double>(node, lua_tostring(L, 2), lua_tonumber(L, 3));
				return 0;
			}
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
    lua_getfield(L, 1, "__coreElement__");
    uiContainer* container = (uiContainer*)lua_touserdata(L,-1);
	lua_pop(L, 1);
	
	
	uiTextElement* text = UiSystem.AddText(container);
	
	text->text = startText;
	
	lua_newtable(L);
	lua_pushlightuserdata(L, text);
	lua_setfield(L, -2, "__coreElement__");
	
	lua_newtable(L);
	lstString("text", text->text.c_str());
	lstString("wrapWidth", text->text.c_str());
	lstString("tooltip", text->tooltip.c_str());
	lstBoolean("visible", text->visible)
	lua_setfield(L, -2, "__coreProperties__");
	
	lua_pushvalue(L, -1);
	text->luaTableKey = luaL_ref(L, LUA_REGISTRYINDEX);
	
	lstBoolean("__exists__", true);
	
	lua_pushvalue(L, 1);
	lua_setfield(L, -2, "parent");
	
	luaL_getmetatable(L, "__mtUiElement");
	lua_setmetatable(L, -2);
	

    return 1;
}

lFuncImp(mUiWindow, addButton)
{
	lua_settop(L, 3);
    lgString(label, 2, "butts");
	
    lua_getfield(L, 1, "__coreElement__");
    uiContainer* container = (uiContainer*)lua_touserdata(L,-1);
    lua_pop(L, 1);
	
	uiButtonElement* button = UiSystem.AddButton(container);
    
    button->label = label;
	
	lua_newtable(L);
	lua_pushlightuserdata(L, button);
	lua_setfield(L, -2, "__coreElement__");
	
	lua_newtable(L);
	lstString("label", button->label.c_str());
	lstString("tooltip", button->tooltip.c_str());
	lstBoolean("visible", button->visible)
	lua_setfield(L, -2, "__coreProperties__");
	
	lua_pushvalue(L, 3);
	lua_setfield(L, -2, "callback");

	

	lua_pushvalue(L, -1);
	button->luaTableKey = luaL_ref(L, LUA_REGISTRYINDEX);
	
	lstBoolean("__exists__", true);
	
	lua_pushvalue(L, 1);
	lua_setfield(L, -2, "parent");
	
	luaL_getmetatable(L, "__mtUiElement");
	lua_setmetatable(L, -2);

    return 1;
}

lFuncImp(mUiWindow, addTree)
{
	lua_settop(L, 3);
	lgString(label, 2, "butts");
	
	lua_getfield(L, 1, "__coreElement__");
	uiContainer* container = (uiContainer*)lua_touserdata(L,-1);
	lua_pop(L, 1);
	
	uiTreeElement* tree = UiSystem.AddTree(container);
	
	tree->label = label;
	
	lua_newtable(L);
	lua_pushlightuserdata(L, tree);
	lua_setfield(L, -2, "__coreElement__");
	
	lua_newtable(L);
	lstString("label", tree->label.c_str());
	lstString("tooltip", tree->tooltip.c_str());
	lstBoolean("opened", tree->opened);
	lstBoolean("visible", tree->visible)
	lua_setfield(L, -2, "__coreProperties__");
	
	lua_pushvalue(L, 3);
	lua_setfield(L, -2, "callback");
	
	
	lua_pushvalue(L, -1);
	tree->luaTableKey = luaL_ref(L, LUA_REGISTRYINDEX);
	
	lstBoolean("__exists__", true);
	
	lua_pushvalue(L, 1);
	lua_setfield(L, -2, "parent");
	
	luaL_getmetatable(L, "__mtUiContainer");
	lua_setmetatable(L, -2);

	
	return 1;
}


lFuncImp(mUiWindow, addInputText)
{
	lua_settop(L, 3);
	lgString(label, 2, "butts");
	lgString(text, 3, "poop");
	
	lua_getfield(L, 1, "__coreElement__");
	uiContainer* container = (uiContainer*)lua_touserdata(L,-1);
	lua_pop(L, 1);
	
	uiInputTextElement* itext = UiSystem.AddInputText(container);
	
	itext->label = label;
	itext->text = text;
	
	lua_newtable(L);
	lua_pushlightuserdata(L, itext);
	lua_setfield(L, -2, "__coreElement__");
	
	lua_newtable(L);
	lstString("label", itext->label.c_str());
	lstString("tooltip", itext->tooltip.c_str());
	lstString("text", itext->text.c_str());
	lstBoolean("visible", itext->visible)
	lua_setfield(L, -2, "__coreProperties__");
	
	lua_pushvalue(L, 3);
	lua_setfield(L, -2, "callback");
	
	lua_pushvalue(L, -1);
	itext->luaTableKey = luaL_ref(L, LUA_REGISTRYINDEX);
	
	lstBoolean("__exists__", true);
	
	lua_pushvalue(L, 1);
	lua_setfield(L, -2, "parent");
	
	luaL_getmetatable(L, "__mtUiElement");
	lua_setmetatable(L, -2);

	
	return 1;
}

lFuncImp(mUiWindow, addCheckbox)
{
	lua_settop(L, 3);
	lgString(label, 2, "butts");
	lgBool(checked, 3, false);
	
	lua_getfield(L, 1, "__coreElement__");
	uiContainer* container = (uiContainer*)lua_touserdata(L,-1);
	lua_pop(L, 1);
	
	uiCheckboxElement* box = UiSystem.AddCheckbox(container);
	
	box->label = label;
	box->checked = checked;
	
	lua_newtable(L);
	lua_pushlightuserdata(L, box);
	lua_setfield(L, -2, "__coreElement__");
	
	lua_newtable(L);
	lstString("label", box->label.c_str());
	lstString("tooltip", box->tooltip.c_str());
	lstBoolean("checked", box->checked);
	lstBoolean("visible", box->visible)
	lua_setfield(L, -2, "__coreProperties__");
	
	lua_pushvalue(L, 3);
	lua_setfield(L, -2, "callback");
	
	
	lua_pushvalue(L, -1);
	box->luaTableKey = luaL_ref(L, LUA_REGISTRYINDEX);
	
	lstBoolean("__exists__", true);
	
	lua_pushvalue(L, 1);
	lua_setfield(L, -2, "parent");
	
	luaL_getmetatable(L, "__mtUiElement");
	lua_setmetatable(L, -2);

	
	return 1;
}

lFuncImp(mUiWindow, addSlider)
{
	lua_settop(L, 2);
	lgString(label, 2, "butts");
	
	lua_getfield(L, 1, "__coreElement__");
	uiContainer* container = (uiContainer*)lua_touserdata(L,-1);
	lua_pop(L, 1);
	
	uiSliderElement* slider = UiSystem.AddSlider(container);
	
	slider->label = label;
	
	lua_newtable(L);
	lua_pushlightuserdata(L, slider);
	lua_setfield(L, -2, "__coreElement__");
	
	lua_newtable(L);
	lstString("label", slider->label.c_str());
	lstNumber("minValue", slider->minValue);
	lstNumber("maxValue", slider->maxValue);
	lstNumber("value", slider->value);
	lstBoolean("rounded", slider->rounded);
	lstString("format", slider->format.c_str());
	
	lstString("tooltip", slider->tooltip.c_str());
	lstBoolean("visible", slider->visible)
	lua_setfield(L, -2, "__coreProperties__");
	
	lua_pushvalue(L, 3);
	lua_setfield(L, -2, "callback");
	
	
	
	lua_pushvalue(L, -1);
	slider->luaTableKey = luaL_ref(L, LUA_REGISTRYINDEX);
	
	lstBoolean("__exists__", true);
	
	lua_pushvalue(L, 1);
	lua_setfield(L, -2, "parent");
	
	luaL_getmetatable(L, "__mtUiElement");
	lua_setmetatable(L, -2);
	
	return 1;
}

//TODO(robin):
/*
    userdata garbage collection
 */


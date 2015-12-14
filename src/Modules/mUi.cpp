//
//  mUi.cpp
//  VroomVroom
//
//  Created by Robin Zaagsma on 03/11/15.
//  Copyright © 2015 Valentinas Rimeika. All rights reserved.
//

#include "mUi.h"
#include "../Systems/uiSystem.h"
#include "../Utilities/typedef.h"
#include "../console.h"

void mUi::Bind(lua_State* L){
	//TODO(tobin): Go do some metatable inheritance witchery
	
	lua_getglobal(L, "Engine");
	lua_newtable(L);
	
    lStart(ui)
        lBind(createWindow)
		lBind(setTooltip)
    lEnd(UiWindow)
	luaL_openlib(L, 0, ui_funcs, 0);
	
	lua_pushboolean(L, false);
	lua_setfield(L, -2, "hasFocus");
	
	lua_setfield(L, -2, "ui");
	

    
    luaL_newmetatable(L, "__mtUiElement");
    const luaL_reg __mtUiElement_methods[] =
    {
        {"__index", lw_mtIndex__},
        {"__newindex", lw_mtNewIndex__},
		lBind(destroy)
		{0, 0}
	};
    luaL_openlib(L, 0, __mtUiElement_methods, 0);
	
	
	
	luaL_newmetatable(L, "__mtUiContainer");
	const luaL_reg __mtUiContainer_methods[] =
	{
		lBind(addText)
		lBind(addButton)
		lBind(addTree)
		lBind(addInputText)
		lBind(addCheckbox)
		lBind(addSlider)
		lBind(addRegion)
		lBind(addHorizontalLayout)
		lBind(remove)
		lBind(removeChildren)
		{0, 0}
	};
	luaL_openlib(L, 0, __mtUiElement_methods, 0);
	luaL_openlib(L, 0, __mtUiContainer_methods, 0);
	
	
	
	luaL_newmetatable(L, "__mtUiWindow");
	const luaL_reg __mtUiWindow_methods[] =
	{
		lBind(close)
		{0, 0}
	};
	luaL_openlib(L, 0, __mtUiElement_methods, 0);
	luaL_openlib(L, 0, __mtUiContainer_methods, 0);
	luaL_openlib(L, 0, __mtUiWindow_methods, 0);
}

void mUi::UnreferenceTable(int tableKey)
{
	lua_State* L = LuaSystem.GetState();
	
	lua_pushnumber(L, tableKey);
	lua_gettable(L, LUA_REGISTRYINDEX);
	lstBoolean("__exists__", false);
	
	luaL_unref(L, LUA_REGISTRYINDEX, tableKey);
	lua_settop(L, 0);
}

void mUi::HandleCallback(int tableKey, const char* funcName)
{
	lua_State* L = LuaSystem.GetState();
	
	lua_pushnumber(L, tableKey);
	lua_gettable(L, LUA_REGISTRYINDEX);
	lua_getfield(L, -1, funcName);
	if(lua_isfunction(L, -1))
	{
		lua_pushvalue(L, 1);
		LuaSystem.Call(L, 1, 0);
	}
	lua_settop(L, 0);
}

void mUi::ToggleFocus(bool focus)
{
	lua_State* L = LuaSystem.GetState();
	lua_getglobal(L, "Engine");
	lua_getfield(L, -1, "ui");
	lua_pushboolean(L, focus);
	lua_setfield(L, -2, "hasFocus");
	
	lua_settop(L, 0);
}

void mUi::BasicElementBind(lua_State* L, uiElement* e, int parentIndex)
{
	lua_pushvalue(L, -1);
	e->luaTableKey = luaL_ref(L, LUA_REGISTRYINDEX);
	
	lua_pushlightuserdata(L, e);
	lua_setfield(L, -2, "__coreElement__");

	lua_newtable(L);
	
	lstBoolean("visible", e->visible)
	lstString("tooltip", e->tooltip.c_str())
	lstNumber("width", e->width)
	
	lua_setfield(L, -2, "__coreProperties__");
	
	lstBoolean("__exists__", true);
	
	if(parentIndex != -1)
	{
		lua_pushvalue(L, parentIndex);
		lua_setfield(L, -2, "parent");
	}
}

lFuncImp(mUi, createWindow){
    lgString(title, 1, "new window")
    lgInt(width, 2, 400)
    lgInt(height, 3, 250)
    
    lua_settop(L, 0);
    
	uiWindow* window = UiSystem.ConstructWindow();
    
    window->title = title;
    window->width = width;
    window->height = height;
	
	lua_newtable(L);
	
	BasicElementBind(L, window, -1);
	
	lua_getfield(L, -1, "__coreProperties__");

    lstString("title", title.c_str());
    lstNumber("width", width)
    lstNumber("height", height)
    lstNumber("x", 0)
    lstNumber("y", 0)
    
    lstBoolean("resizable", window->resizable)
    lstBoolean("movable", window->movable)
    lstBoolean("closable", window->closable)
    lstBoolean("collapsable", window->collapsable)
	lstBoolean("displayTitle", window->displayTitle)
	lstBoolean("expanded", window->expanded)
    
	lua_pop(L, 1);
	
    luaL_getmetatable(L, "__mtUiWindow");
    lua_setmetatable(L, -2);
	
    return 1;
}

lFuncImp(mUi, setTooltip){
	lua_settop(L, 1);
	lgString(tt, 1, "");
	
	UiSystem.SetTooltip(tt);
	
	return 0;
}

lFuncImp(mUi, close)
{
	lua_getfield(L, 1, "__coreElement__");
	uiWindow* window = (uiWindow*)lua_touserdata(L,-1);
	lua_pop(L, 1);

	UiSystem.RemoveWindow(window);
	
	return 0;
}

lFuncImp(mUi, mtIndex)
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

lFuncImp(mUi, mtNewIndex)
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

lFuncImp(mUi, addText)
{
	lua_settop(L, 2);
    lgString(startText,2,"lorum ipsum");
	
    lua_getfield(L, 1, "__coreElement__");
    uiContainer* container = (uiContainer*)lua_touserdata(L,-1);
	lua_pop(L, 1);
	
	
	uiTextElement* text = UiSystem.AddText(container);
	
	text->text = startText;
	
	lua_newtable(L);
	BasicElementBind(L, text, 1);
	
	lua_getfield(L, -1, "__coreProperties__");
	lstString("text", text->text.c_str());
	lstString("wrapWidth", text->text.c_str());
	lua_pop(L, 1);
	
	luaL_getmetatable(L, "__mtUiElement");
	lua_setmetatable(L, -2);
	

    return 1;
}

lFuncImp(mUi, addButton)
{
	lua_settop(L, 3);
    lgString(label, 2, "butts");
	
    lua_getfield(L, 1, "__coreElement__");
    uiContainer* container = (uiContainer*)lua_touserdata(L,-1);
    lua_pop(L, 1);
	
	uiButtonElement* button = UiSystem.AddButton(container);
    
    button->text = label;
	
	lua_newtable(L);
	BasicElementBind(L, button, 1);
	
	lua_getfield(L, -1, "__coreProperties__");
	lstString("text", button->text.c_str());
	lua_pop(L, 1);
	
	lua_pushvalue(L, 3);
	lua_setfield(L, -2, "callback");

	luaL_getmetatable(L, "__mtUiElement");
	lua_setmetatable(L, -2);

    return 1;
}

lFuncImp(mUi, addTree)
{
	lua_settop(L, 3);
	lgString(label, 2, "butts");
	
	lua_getfield(L, 1, "__coreElement__");
	uiContainer* container = (uiContainer*)lua_touserdata(L,-1);
	lua_pop(L, 1);
	
	uiTreeElement* tree = UiSystem.AddTree(container);
	
	tree->label = label;
	
	lua_newtable(L);
	BasicElementBind(L, tree, 1);
	
	lua_getfield(L, -1, "__coreProperties__");	lstString("label", tree->label.c_str());
	lstBoolean("opened", tree->opened);
	lua_pop(L, 1);
	
	luaL_getmetatable(L, "__mtUiContainer");
	lua_setmetatable(L, -2);
	
	return 1;
}


lFuncImp(mUi, addInputText)
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
	BasicElementBind(L, itext, 1);
	
	lua_getfield(L, -1, "__coreProperties__");
	lstString("label", itext->label.c_str());
	lstString("text", itext->text.c_str());
	lua_pop(L, 1);
	
	luaL_getmetatable(L, "__mtUiElement");
	lua_setmetatable(L, -2);
	
	return 1;
}

lFuncImp(mUi, addCheckbox)
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
	BasicElementBind(L, box, 1);
	
	lua_getfield(L, -1, "__coreProperties__");
	lstString("label", box->label.c_str());
	lstBoolean("checked", box->checked);
	lua_pop(L, 1);
	
	luaL_getmetatable(L, "__mtUiElement");
	lua_setmetatable(L, -2);

	return 1;
}

lFuncImp(mUi, addSlider)
{
	lua_settop(L, 2);
	lgString(label, 2, "butts");
	
	lua_getfield(L, 1, "__coreElement__");
	uiContainer* container = (uiContainer*)lua_touserdata(L,-1);
	lua_pop(L, 1);
	
	uiSliderElement* slider = UiSystem.AddSlider(container);
	
	slider->label = label;
	
	lua_newtable(L);
	BasicElementBind(L, slider, 1);
	
	lua_getfield(L, -1, "__coreProperties__");
	lstString("label", slider->label.c_str());
	lstNumber("minValue", slider->minValue);
	lstNumber("maxValue", slider->maxValue);
	lstNumber("value", slider->value);
	lstBoolean("rounded", slider->rounded);
	lstString("format", slider->format.c_str());
	lua_pop(L, 1);
	
	luaL_getmetatable(L, "__mtUiElement");
	lua_setmetatable(L, -2);
	
	return 1;
}

lFuncImp(mUi, addRegion)
{
	lua_settop(L, 1);
	
	lua_getfield(L, 1, "__coreElement__");
	uiContainer* container = (uiContainer*)lua_touserdata(L,-1);
	lua_pop(L, 1);
	
	uiRegionElement* region = UiSystem.AddRegion(container);
	
	lua_newtable(L);
	BasicElementBind(L, region, 1);
	
	lua_getfield(L, -1, "__coreProperties__");
	//lstNumber("width", region->width); //this is a property for all uiElements now
	lstNumber("height", region->height);
	lstBoolean("bordered", region->bordered);
	lua_pop(L, 1);
	
	luaL_getmetatable(L, "__mtUiContainer");
	lua_setmetatable(L, -2);
	
	return 1;
}

lFuncImp(mUi, addHorizontalLayout)
{
	lua_settop(L, 1);
	
	lua_getfield(L, 1, "__coreElement__");
	uiContainer* container = (uiContainer*)lua_touserdata(L,-1);
	lua_pop(L, 1);
	
	uiHorizontalLayoutElement* region = UiSystem.AddHorizontalLayout(container);
	
	lua_newtable(L);
	BasicElementBind(L, region, 1);
	
	lua_getfield(L, -1, "__coreProperties__");
	lstNumber("spacing", region->spacing); //this is a property for all uiElements now
	lstNumber("offset", region->offset); //this is a property for all uiElements now
	lua_pop(L, 1);
	
	luaL_getmetatable(L, "__mtUiContainer");
	lua_setmetatable(L, -2);
	
	return 1;
}

lFuncImp(mUi, destroy)
{
	lua_getfield(L, 1, "__coreElement__");
	uiElement* element = (uiElement*)lua_touserdata(L,-1);
	lua_pop(L, 1);
	
	if(element->parent != NULL){
		UiSystem.RemoveElement(element->parent, element);
	}
	
	lua_pushnil(L);
	lua_setfield(L, 1, "__coreElement__");
	
	return 0;
}

lFuncImp(mUi, remove)
{
	lua_getfield(L, 1, "__coreElement__");
	uiContainer* container = (uiContainer*)lua_touserdata(L,-1);
	lua_pop(L, 1);
	
	lua_getfield(L, 2, "__coreElement__");
	uiElement* element = (uiElement*)lua_touserdata(L,-1);
	lua_pop(L, 2);
	
	if(element->parent == container)
	{
		UiSystem.RemoveElement(container, element);
	}
	
	lua_pushnil(L);
	lua_setfield(L, 2, "__coreElement__");


	return 0;
}

//TODO set coreElement to nil (userdata quirks maybe)?
lFuncImp(mUi, removeChildren)
{
	lua_getfield(L, 1, "__coreElement__");
	uiContainer* container = (uiContainer*)lua_touserdata(L,-1);
	lua_pop(L, 1);
	
	UiSystem.RemoveChildren(container);
	
	return 0;
}
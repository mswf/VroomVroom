//
//  mInput.cpp
//  VroomVroom
//
//  Created by Robin Zaagsma on 13/10/15.
//  Copyright © 2015 Valentinas Rimeika. All rights reserved.
//

#include "mInput.h"
#include "Input.hpp"
#include "../Utilities/standardIncludes.h"
#include "../Systems/luaSystem.h"

Input* mInput::input = NULL;

void mInput::Bind(lua_State* L)
{
    lStart(Input)
        lBind(getMousePosition)
        //lBind(bindKey)
        //lBind(unbindKey)
        //lBind(getBind)
        //lBind(bindExists)
        //lBind(isBound)
        lBind(key)
        lBind(keyDown)
        lBind(keyUp)
        lBind(mouseDown)
        lBind(mouseUp)
    lEnd(Input)
	luaL_openlib(L, "Input" , Input_funcs, 0);\
	lua_pop(L, 1);

}

void mInput::SetInput(Input* I)
{
    input = I;
}

lFuncImp(mInput, getMousePosition)
{
    glm::ivec2 mousePos = input->GetMousePosition();
    
    lua_pushnumber(L, mousePos.x);
    lua_pushnumber(L, mousePos.y);
    
    return 2;
}
/*
lFuncImp(mInput, bindKey)
{
    lua_settop(L, 2);
    lgString(name, 1);
    lgInt(key, 2);
    lua_settop(L, 0);
    
    input->BindKey(name, key);
    
    return 0;
}

lFuncImp(mInput, unbindKey)
{
    lua_settop(L, 1);
    lgString(name, 1);
    lua_settop(L,0);
    
    input->UnbindKey(name);
    
    return 0;
}

lFuncImp(mInput, getBind)
{
    return 0;
}

lFuncImp(mInput, bindExists)
{
    lua_settop(L, 1);
    lgString(name, 1);
    lua_settop(L,0);
    
    bool exists = input->BindExists(name);
    lua_pushboolean(L, exists);

    return 1;
}

lFuncImp(mInput, isBound)
{
    lua_settop(L, 1);
    lgInt(key, 1);
    lua_settop(L,0);
    
    bool exists = input->IsBound(key);
    lua_pushboolean(L, exists);
    
    return 1;
}
*/
lFuncImp(mInput, key)
{
    lua_settop(L, 1);
    lgInt(key, 1, 0);
    lua_settop(L,0);
    
    bool down = input->OnKey( (SDL_Keycode)key );
    lua_pushboolean(L, down);
    
    return 1;
}

lFuncImp(mInput, keyDown)
{
    lua_settop(L, 1);
    lgInt(key, 1, 0);
    lua_settop(L,0);
    
    bool down = input->OnKeyDown( (SDL_Keycode)key );
    lua_pushboolean(L, down);
    
    return 1;

}

lFuncImp(mInput, keyUp)
{
    lua_settop(L, 1);
    lgInt(key, 1, 0);
    lua_settop(L,0);
    
    bool down = input->OnKeyUp( (SDL_Keycode)key );
    lua_pushboolean(L, down);
    
    return 1;
}

lFuncImp(mInput, mouseDown)
{
    lua_settop(L, 1);
    lgInt(button, 1, 0);
    lua_settop(L, 0);
    
    bool isDown = input->OnMouseDown(button);
    
    lua_pushboolean(L, isDown);
    return 1;
}

lFuncImp(mInput, mouseUp)
{
    lua_settop(L, 1);
    lgInt(button, 1, 0);
    lua_settop(L, 0);
    
    bool isDown = input->OnMouseUp(button);
    
    lua_pushboolean(L, isDown);
    return 1;

}
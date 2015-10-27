//
//  mInput.cpp
//  VroomVroom
//
//  Created by Robin Zaagsma on 13/10/15.
//  Copyright © 2015 Valentinas Rimeika. All rights reserved.
//

#include "mInput.h"
#include "console.h"
#include "Input.hpp"


Input* mInput::input = NULL;

void mInput::Bind(lua_State* L)
{
    lStart(Input)
        lBind(getMousePosition)
        lBind(bindKey)
        lBind(unbindKey)
        lBind(getBind)
        lBind(bindExists)
        lBind(isBound)
        lBind(key)
        lBind(keyDown)
        lBind(keyUp)
        lBind(mouseDown)
        lBind(mouseUp)
    lEnd(Input)
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

lFuncImp(mInput, bindKey)
{
    return 0;
}

lFuncImp(mInput, unbindKey)
{
    return 0;
}

lFuncImp(mInput, getBind)
{
    return 0;
}

lFuncImp(mInput, bindExists)
{
    return 0;
}

lFuncImp(mInput, isBound)
{
    return 0;
}

lFuncImp(mInput, key)
{
    return 0;
}

lFuncImp(mInput, keyDown)
{
    return 0;
}

lFuncImp(mInput, keyUp)
{
    return 0;
}

lFuncImp(mInput, mouseDown)
{
    return 0;
}

lFuncImp(mInput, mouseUp)
{
    return 0;
}
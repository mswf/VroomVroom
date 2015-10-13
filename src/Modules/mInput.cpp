//
//  mInput.cpp
//  VroomVroom
//
//  Created by Robin Zaagsma on 13/10/15.
//  Copyright © 2015 Valentinas Rimeika. All rights reserved.
//

#include "mInput.h"

namespace mInput
{
    void Bind(lua_State* L, Input* I)
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
    
    lFunc(getMousePosition)
    {
        return 0;
    }
    
    lFunc(bindKey)
    {
        return 0;
    }
    
    lFunc(unbindKey)
    {
        return 0;
    }
    
    lFunc(getBind)
    {
        return 0;
    }
    
    lFunc(bindExists)
    {
        return 0;
    }
    
    lFunc(isBound)
    {
        return 0;
    }
    
    lFunc(key)
    {
        return 0;
    }
    
    lFunc(keyDown)
    {
        return 0;
    }
    
    lFunc(keyUp)
    {
        return 0;
    }
    
    lFunc(mouseDown)
    {
        return 0;
    }
    
    lFunc(mouseUp)
    {
        return 0;
    }
    
} //mInput
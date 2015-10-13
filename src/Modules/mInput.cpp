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
            lBind(onKey)
            lBind(onKeyDown)
            lBind(onKeyUp)
            lBind(onMouseDown)
            lBind(onMouseUp)
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
    
    lFunc(onKey)
    {
        return 0;
    }
    
    lFunc(onKeyDown)
    {
        return 0;
    }
    
    lFunc(onKeyUp)
    {
        return 0;
    }
    
    lFunc(onMouseDown)
    {
        return 0;
    }
    
    lFunc(onMouseUp)
    {
        return 0;
    }
    
} //mInput
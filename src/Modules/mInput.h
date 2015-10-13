//
//  mInput.hpp
//  VroomVroom
//
//  Created by Robin Zaagsma on 13/10/15.
//  Copyright © 2015 Valentinas Rimeika. All rights reserved.
//

#ifndef mInput_h
#define mInput_h

#include "moduleMacros.h"

class Input;

namespace mInput{
    void Bind(lua_State*, Input*);

    
    lFunc(getMousePosition);
    lFunc(bindKey);
    lFunc(unbindKey);
    lFunc(getBind);
    lFunc(bindExists);
    lFunc(isBound);
    lFunc(key);
    lFunc(keyDown);
    lFunc(keyUp);
    lFunc(mouseDown);
    lFunc(mouseUp);
}

#endif /* mInput_h */

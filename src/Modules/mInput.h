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

class mInput{
    public:
        static void Bind(lua_State*);
        static void SetInput(Input*);

    private:
        lFuncDef(getMousePosition);
        //lFuncDef(bindKey);
        //lFuncDef(unbindKey);
        //lFuncDef(getBind);
        //lFuncDef(bindExists);
        //lFuncDef(isBound);
        lFuncDef(key);
        lFuncDef(keyDown);
        lFuncDef(keyUp);
        lFuncDef(mouseDown);
        lFuncDef(mouseUp);
    
        static Input* input;
};

#endif /* mInput_h */

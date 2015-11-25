//
//  wEngine.hpp
//  VroomVroom
//
//  Created by Robin Zaagsma on 13/10/15.
//  Copyright © 2015 Valentinas Rimeika. All rights reserved.
//

#ifndef mEngine_h
#define mEngine_h

#include "moduleMacros.h"
#include "../Systems/luaSystem.h"

class mLog {
    public:
        static void Bind( lua_State* );
    private:
        mLog();
        ~mLog();
    
        lFuncDef(log);
};

#endif /* mEngine_h */

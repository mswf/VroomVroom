//
//  moduleMacros.h
//  VroomVroom
//
//  Created by Robin Zaagsma on 13/10/15.
//  Copyright © 2015 Valentinas Rimeika. All rights reserved.
//

#ifndef moduleMacros_h
#define moduleMacros_h

#include <lua.hpp>

    #define lFunc(NAME)\
        int lw_##NAME##__( lua_State* L)

    #define lStart(NAME)\
        static const luaL_reg NAME##_funcs[] =\
        {\


    #define lBind(NAME)\
            { #NAME ,lw_##NAME##__},

    #define lEnd(NAME)\
            {0,0}\
        };\
        luaL_openlib(L, #NAME , NAME##_funcs, 0);\
        lua_pop(L, 1);


#endif /* moduleMacros_h */

/*

allows some shorthanding when binding lua modules:

    lfunc(bar)
    {
        return 0;
    }
 
then bind it as follows in the Foo table:
    
    lStart(Foo)
        lBind(bar)
    lEnd(Foo)
 
*/
 
 
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
#include "luaSystem.h"

    #define lFuncDef(NAME)\
        static int lw_##NAME##__( lua_State* L)

    #define lFuncImp(CLASS, NAME)\
        int CLASS::lw_##NAME##__( lua_State* L)

    #define lStart(NAME)\
        const luaL_reg NAME##_funcs[] =\
        {\

    #define lBind(NAME)\
            { #NAME ,lw_##NAME##__},

    #define lEnd(NAME)\
            {0,0}\
        };\
        luaL_openlib(L, #NAME , NAME##_funcs, 0);\
        lua_pop(L, 1);




#define lgString(NAME, IDX)\
    string NAME;\
    if ( lua_isstring(L, IDX) ){\
        NAME = lua_tostring(L, IDX);\
    }

#define lgBool(NAME, IDX)\
    bool NAME;\
    if ( lua_isboolean(L, IDX) ){\
        NAME = lua_toboolean(L, IDX);\
    }

#define lgInt(_NAME_, _IDX_)\
    int _NAME_ ;\
    if ( lua_isnumber(L, _IDX_ ) ){\
        _NAME_ = lua_tonumber(L, _IDX_ );\
    }


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
 
 
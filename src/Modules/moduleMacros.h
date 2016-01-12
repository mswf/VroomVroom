//
//  moduleMacros.h
//  VroomVroom
//
//  Created by Robin Zaagsma on 13/10/15.
//  Copyright © 2015 Valentinas Rimeika. All rights reserved.
//

#ifndef moduleMacros_h
#define moduleMacros_h

#include "../systems/luaSystem.h"

#include <lua.hpp>

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



#define lgString(NAME, IDX, FALLBACK)\
    string NAME;\
    if ( lua_isstring(L, IDX) ){\
        NAME = lua_tostring(L, IDX);\
    } else {\
        NAME = string(FALLBACK);\
    }

#define lgBool(NAME, IDX, FALLBACK)\
    bool NAME;\
    if ( lua_isboolean(L, IDX) ){\
        NAME = lua_toboolean(L, IDX);\
    } else {\
        NAME = FALLBACK;\
    }


#define lgInt(NAME, IDX, FALLBACK)\
    int NAME ;\
    if ( lua_isnumber(L, IDX ) ){\
        NAME = lua_tonumber(L, IDX );\
    } else {\
        NAME = FALLBACK;\
    }


#define lgFloat(NAME, IDX, FALLBACK)\
	float NAME ;\
	if ( lua_isnumber(L, IDX ) ){\
		NAME = lua_tonumber(L, IDX );\
	} else {\
		NAME = FALLBACK;\
	}

#define lgFunc(NAME, IDX, FALLBACK)\
	int NAME ;\
	if ( lua_isfunction(L, IDX ) ){\
		lua_pushvalue(L, IDX);\
		NAME = luaL_ref(L, LUA_REGISTRYINDEX );\
	} else {\
		NAME = FALLBACK;\
	}

#define lgComp(NAME, IDX, TYPE)\
	lua_getfield(L, IDX, "__coreComponent__");\
	TYPE* NAME = (TYPE*)lua_touserdata(L, -1);\
	if(NAME == NULL) {\
		return 0;\
	}

#define lstString(NAME, VALUE)\
    lua_pushstring(L, VALUE);\
    lua_setfield(L, -2, NAME);

#define lstNumber(NAME, VALUE)\
    lua_pushnumber(L, VALUE);\
    lua_setfield(L, -2, NAME);

#define lstBoolean(NAME, VALUE)\
    lua_pushboolean(L, VALUE);\
    lua_setfield(L, -2, NAME);


#define lSetCPPPropertyString(TARGET, NAME)\
	lua_getfield(L, -1, #NAME);\
	TARGET->NAME = lua_tostring(L, -1);\
	lua_pop(L, 1);

#define lSetCPPPropertyNumber(TARGET, NAME)\
	lua_getfield(L, -1, #NAME);\
	TARGET->NAME = lua_tonumber(L, -1);\
	lua_pop(L, 1);

#define lSetCPPPropertyBoolean(TARGET, NAME)\
	lua_getfield(L, -1, #NAME);\
	TARGET->NAME = lua_toboolean(L, -1);\
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
 
 
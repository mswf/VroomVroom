//
//  mEntity.cpp
//  VroomVroom
//
//  Created by Robin Zaagsma on 28/10/15.
//  Copyright © 2015 Valentinas Rimeika. All rights reserved.
//

/*
 Creates an entity class that is compatible with the class system in lua. 
 */

#include "mEntity.h"
#include <lua.hpp>
#include "../Utilities/standardIncludes.h"
#include <new>
#include "../Systems/luaSystem.h"
#include "../Components/entity.h"
#include "../Components/cLua.h"

void mEntity::Bind(lua_State* L)
{
	lua_getglobal(L, "Engine");
	lua_newtable(L);
    lStart(BaseEntity)
        lBind(__engineInit)
    lEnd(BaseEntity)
	luaL_openlib(L, 0, BaseEntity_funcs, 0);
    
    luaL_newmetatable(L, "_mtEntity");
    lua_pushstring(L, "__index");
    lua_pushvalue(L, -2);
    lua_settable(L, -3);
    
    const luaL_reg Entity_methods[] =
    {
        {"__gc", lw_gcDestroy__},
        {0, 0}
    };
    
    luaL_openlib(L, 0, Entity_methods, 0);
}

void mEntity::UnreferenceTable(int tableKey)
{
	lua_State* L = LuaSystem.GetState();
	
	lua_pushnumber(L, tableKey);
	lua_gettable(L, LUA_REGISTRYINDEX);
	lstBoolean("__exists__", false);
	
	luaL_unref(L, LUA_REGISTRYINDEX, tableKey);
	lua_settop(L, 0);
}

void mEntity::HandleCallback(int tableKey, const char* funcName)
{
	lua_State* L = LuaSystem.GetState();
	
	lua_pushnumber(L, tableKey);
	lua_gettable(L, LUA_REGISTRYINDEX);
	lua_getfield(L, -1, funcName);
	if(lua_isfunction(L, -1))
	{
		lua_pushvalue(L, 1);
		LuaSystem.Call(L, 1, 0);
	}
	lua_settop(L, 0);
}


lFuncImp(mEntity, __engineInit)
{
    //create a userdata with the size of an entity, and create a new entity at that point in memory
    //set the metatable of this userdata to _metaEntityUser
    new (lua_newuserdata(L, sizeof(CLua))) CLua();
    luaL_getmetatable(L, "_mtEntity");
    lua_setmetatable(L, -2);
    
    //now add the userdata to our table as with the key "core_"
    lua_setfield(L, -2, "__coreComponent__");
    
    return 0;
}

//bound to __gc, thus called by lua when the userdata is destroyed
lFuncImp(mEntity, gcDestroy)
{
   	if(lua_type(L, 1) != LUA_TUSERDATA)
	{
		Terminal.Warning("Lua gc tried to call <gcDestroy> on a table that is not proper userdata");
		return 0;
	}
    CLua* component = reinterpret_cast<CLua*>(lua_touserdata(L, 1));
	delete component;
	
    return 0;
}
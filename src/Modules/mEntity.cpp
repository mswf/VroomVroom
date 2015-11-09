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

void mEntity::Bind(lua_State* L)
{
    lStart(Entity)
        lBind(__engineInit)
        lBind(doPls)
    lEnd(Entity)
    
    luaL_newmetatable(L, "_metaEntityUser");
    lua_pushstring(L, "__index");
    lua_pushvalue(L, -2);
    lua_settable(L, -3);
    
    const luaL_reg Entity_methods[] =
    {
        {"__gc", lw_destroy__},
        {0, 0}
    };
    
    luaL_openlib(L, 0, Entity_methods, 0);
}

lFuncImp(mEntity, __engineInit)
{
    //create a userdata with the size of an entity, and create a new entity at that point in memory
    //set the metatable of this userdata to _metaEntityUser
    new (lua_newuserdata(L, sizeof(StubEntity))) StubEntity();
    luaL_getmetatable(L, "_metaEntityUser");
    lua_setmetatable(L, -2);
    
    //now add the userdata to our table as with the key "core_"
    lua_setfield(L, -2, "core_ENTITY");
    
    return 0;
}

//bound to __gc, thus called by lua when the userdata is destroyed
lFuncImp(mEntity, destroy)
{
    //TODO(robin) make this a macro?
    try
    {
        luaL_checktype(L, 1, LUA_TUSERDATA);
    }
    catch (std::exception const& err)
    {
        Terminal.Error(err.what());
        return 0;
    }
    catch (...)
    {
        Terminal.Error("unknown exception");
        return 0;
    }
    
    
    StubEntity* entity = reinterpret_cast<StubEntity*>(lua_touserdata(L, 1));
    entity->~StubEntity();
    
    return 0;
}

lFuncImp(mEntity, doPls)
{
    //TODO(robin) make this a macro? or a helper function that returns the entity pointer
    try
    {
        luaL_checktype(L, 1, LUA_TTABLE);
        lua_getfield(L, 1, "core_ENTITY");
        luaL_checktype(L, -1, LUA_TUSERDATA);
    }
    catch (std::exception const& err)
    {
        Terminal.Error(err.what());
        return 0;
    }
    catch (...)
    {
        Terminal.Error("unknown exception");
        return 0;
    }
    
    StubEntity* entity = reinterpret_cast<StubEntity*>(lua_touserdata(L, -1));
    entity->DoPls();
    
    return 0;
}
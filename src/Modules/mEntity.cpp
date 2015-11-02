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
#include "standardIncludes.h"
#include <new>

void mEntity::Bind(lua_State* L)
{
    lStart(Entity)
        lBind(create)
    lEnd(Entity)
    
    luaL_newmetatable(L, "_metaEntity");
    lua_pushstring(L, "__index");
    lua_pushvalue(L, -2);
    lua_settable(L, -3);
    
    const luaL_reg Entity_methods[] =
    {
        {"__gc", lw_destroy__},
        lBind(doPls)
        lBind(doTest)
        {0, 0}
    };
    
    luaL_openlib(L, 0, Entity_methods, 0);
}

lFuncImp(mEntity, create)
{
    lua_newtable(L);
    lua_insert(L, 1);
    //create a table and give it the _metaEntity metatable
    if(lua_gettop(L) == 1)
    {
        lua_newtable(L);
    }
    else if (lua_istable(L, 2))
    {
        lua_settop(L, 2);
    }
    else
    {
        Terminal.LuaError("Error in Entity.Create: Expected table");
    }
    luaL_getmetatable(L, "_metaEntity");
    lua_setmetatable(L, -2);
    
    //create a userdata with the size of an entity, and create a new entity at that point in memory
    //set the metatable of this userdata to _metaEntity as well
    new (lua_newuserdata(L, sizeof(StubEntity))) StubEntity();
    luaL_getmetatable(L, "_metaEntity");
    lua_setmetatable(L, -2);
    
    //now add the userdata to our table as with the key "core_"
    lua_setfield(L, -2, "core_ENTITY");
    
    lua_setmetatable(L, -2);
    
    return 1;
    
    /*
     7
     6
     5
     4
     3  <_metaEntity>
     2  <table>
            <_metaEntity>
     1  <table>
     */
    
}

//bound to __gc, thus called by lua when the userdata is destroyed
lFuncImp(mEntity, destroy)
{
    //TODO make this a macro?
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
    delete entity;
    
    return 0;
}

lFuncImp(mEntity, doPls)
{
    //TODO make this a macro? or a helper function that returns the entity pointer
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

lFuncImp(mEntity, doTest)
{
    try
    {
        luaL_checktype(L, 1, LUA_TTABLE);
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
    
    lua_getfield(L, 1, "test");
    if(lua_isfunction(L, -1))
    {
        Terminal.Log("test function defined");
        if(lua_pcall(L, 0, 0, 0) != 0)
        {
            Terminal.LuaError(lua_tostring(L, -1));
        }
    }
    else
    {
        Terminal.Log("no test function defined");
    }

    return 0;
}
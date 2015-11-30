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
#include "../Components/entity.h"

#include "../Components/cLua.h"
#include "../Components/cCamera.h"
#include "../Components/cMeshRenderer.h"

void mEntity::Bind(lua_State* L)
{
	lua_getglobal(L, "Engine");
	lua_newtable(L);
    lStart(BaseEntity)
        lBind(__engineInit)
		lBind(addChild)
		lBind(addComponent)
    lEnd(BaseEntity)
	luaL_openlib(L, 0, BaseEntity_funcs, 0);
	lua_setfield(L, -2, "baseEntity");
	
	
	
	//metatable
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
    CLua* comp = new (lua_newuserdata(L, sizeof(CLua))) CLua();
    luaL_getmetatable(L, "_mtEntity");
    lua_setmetatable(L, -2);
    
    //now add the userdata to our table
    lua_setfield(L, -2, "__coreComponent__");
	
	
	comp->SetTableKey( luaL_ref(L, LUA_REGISTRYINDEX) );
	
	Entity* e = new Entity();
	Entity::AddComponent(e, comp);

    
    return 0;
}

lFuncImp(mEntity, addChild)
{
	lua_settop(L, 2);
	
	lua_getfield(L, 1, "__coreComponent__");
	CLua* parent = (CLua*)lua_touserdata(L, -1);
	
	lua_getfield(L, 2, "__coreComponent__");
	CLua* child = (CLua*)lua_touserdata(L, -1);
	
	parent->entity->AddChild(child->entity);
	
	return 0;
}

lFuncImp(mEntity, addComponent)
{
	lua_settop(L, 2);
	
	lua_getfield(L, 1, "__coreComponent__");
	CLua* core = (CLua*)lua_touserdata(L, -1);
	
	lua_getfield(L, 2, "__coreComponent__");
	Component* comp = (Component*)lua_touserdata(L, -1);
	
	lua_getfield(L, 2, "__familyId__");
	int familyId = lua_tonumber(L, -1);
	
	
	if(familyId == (int)ComponentTypes::CAMERA)
	{
		Entity::AddComponent(core->entity, (CCamera*)comp);
	}
	if(familyId == (int)ComponentTypes::MESH_RENDERER)
	{
		Entity::AddComponent(core->entity, (CMeshRenderer*)comp);
	}
	
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
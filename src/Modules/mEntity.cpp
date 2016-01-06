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
#include "../Components/cTransform.h"
#include "../Components/cDebugRenderer.h"
#include "../Components/cLight.h"

#include "../engine.h"

void mEntity::Bind(lua_State* L)
{
	lua_getglobal(L, "Engine");
	lua_newtable(L);
    lStart(BaseEntity)
        lBind(__engineInit)
		lBind(addChild)
		lBind(addComponent)
	
		lBind(getChildren)
		lBind(removeChild)

	
		//GETTERS
		lBind(getX)
		lBind(getY)
		lBind(getZ)
		
		lBind(getScaleX)
		lBind(getScaleY)
		lBind(getScaleZ)
		
		lBind(getPitch)
		lBind(getYaw)
		lBind(getRoll)
		
		lBind(getPosition)
		lBind(getScale)
		lBind(getRotation)
	
	
		//SETTERS
		lBind(setX)
		lBind(setY)
		lBind(setZ)
		
		lBind(setScaleX)
		lBind(setScaleY)
		lBind(setScaleZ)
		
		lBind(setPitch)
		lBind(setYaw)
		lBind(setRoll)
		
		lBind(setPosition)
		lBind(setScale)
		lBind(setRotation)
	
		
		//INCREMENT
		lBind(addX)
		lBind(addY)
		lBind(addZ)
		
		lBind(addScaleX)
		lBind(addScaleY)
		lBind(addScaleZ)
		
		lBind(pitch)
		lBind(yaw)
		lBind(roll)
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
	//TODO(robin): change to light userdata
    //create a userdata with the size of an entity, and create a new entity at that point in memory
    //set the metatable of this userdata to _metaEntityUser
    CLua* comp = new (lua_newuserdata(L, sizeof(CLua))) CLua();
    luaL_getmetatable(L, "_mtEntity");
    lua_setmetatable(L, -2);
    
    //now add the userdata to our table
    lua_setfield(L, -2, "__coreComponent__");
	
	lua_pushnumber(L, (int)CLua::familyId);
	lua_setfield(L, -2, "__familyId__");
	
	lua_pushnil(L);
	lua_setfield(L, -2, "parent");
	
	
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
	
	lua_pushvalue(L, 1);
	lua_setfield(L, 2, "parent");
	
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
		lua_pushvalue(L, 2);
		lua_setfield(L, 1, "camera");
	}
	if(familyId == (int)ComponentTypes::MESH_RENDERER)
	{
		Entity::AddComponent(core->entity, (CMeshRenderer*)comp);
		lua_pushvalue(L, 2);
		lua_setfield(L, 1, "meshRenderer");
	}
	if(familyId == (int)ComponentTypes::DEBUG_RENDERER)
	{
		Entity::AddComponent(core->entity, (CDebugRenderer*)comp);
		lua_pushvalue(L, 2);
		lua_setfield(L, 1, "debugRenderer");
	}
	if(familyId == (int)ComponentTypes::LIGHT)
	{
		Entity::AddComponent(core->entity, (CLight*)comp);
		lua_pushvalue(L, 2);
		lua_setfield(L, 1, "light");
	}
	
	
	return 0;
}

lFuncImp(mEntity, getChildren) {
	lua_settop(L, 1);
	
	lua_getfield(L, 1, "__coreComponent__");
	CLua* luaComp = (CLua*)lua_touserdata(L, -1);
	
	std::vector<Entity*> children = luaComp->entity->GetChildren();
	
	lua_newtable(L);
	
	std::vector<Entity*>::const_iterator iter_comp = children.begin();
	std::vector<Entity*>::const_iterator end_comp = children.end();
	int ii = 1;
	for( ; iter_comp != end_comp; ++iter_comp )
	{
		LuaSystem.Dump(L);
		CLua* lua = Entity::GetComponent<CLua>(*iter_comp);
		lua_pushnumber(L, ii);
		lua_pushnumber(L, lua->GetTableKey());
		LuaSystem.Dump(L);
		lua_gettable(L, LUA_REGISTRYINDEX);
		LuaSystem.Dump(L);
		lua_settable(L, -3);
		LuaSystem.Dump(L);
		ii++;
	}
	
	
	return 1;
}

lFuncImp(mEntity, removeChild) {
	lua_settop(L, 2);
	
	lua_getfield(L, 1, "__coreComponent__");
	CLua* parent = (CLua*)lua_touserdata(L, -1);
	
	lua_getfield(L, 2, "__coreComponent__");
	CLua* child = (CLua*)lua_touserdata(L, -1);
	
	parent->entity->RemoveChild(child->entity);
	
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

#define lTransformGet(FUNC, CALL)\
	lFuncImp(mEntity, FUNC )\
	{\
		lua_settop(L,1);\
		\
		lua_getfield(L, 1, "__coreComponent__");\
		if(lua_isnil(L, -1))\
		{\
			Terminal.Warning("no core component found on this entity. What did you do?!!?!!?");\
			return 1;\
		}\
		\
		CLua* component = reinterpret_cast<CLua*>(lua_touserdata(L, -1));\
		CTransform* transform = component->entity->transform;\
		\
		float val = transform->CALL();\
		\
		lua_pushnumber(L, val);\
		\
		return 1;\
	}


//GETTERS
lTransformGet(getX, GetPositionX);
lTransformGet(getY, GetPositionY);
lTransformGet(getZ, GetPositionZ);

lTransformGet(getScaleX, GetScaleX);
lTransformGet(getScaleY, GetScaleY);
lTransformGet(getScaleZ, GetScaleZ);

lTransformGet(getPitch, GetPitch);
lTransformGet(getYaw, GetYaw);
lTransformGet(getRoll, GetRoll);

lFuncImp(mEntity, getPosition)
{
	lua_settop(L,1);
	
	lua_getfield(L, 1, "__coreComponent__");
	if(lua_isnil(L, -1))
	{
		Terminal.Warning("no core component found on this entity. What did you do?!!?!!?");
		return 1;
	}
	
	CLua* component = reinterpret_cast<CLua*>(lua_touserdata(L, -1));
	CTransform* transform = component->entity->transform;
	
	glm::vec3 val = transform->GetPosition();
	
	lua_pushnumber(L, val.x);
	lua_pushnumber(L, val.y);
	lua_pushnumber(L, val.z);
	
	return 3;
}

lFuncImp(mEntity, getScale)
{
	lua_settop(L,1);
	
	lua_getfield(L, 1, "__coreComponent__");
	if(lua_isnil(L, -1))
	{
		Terminal.Warning("no core component found on this entity. What did you do?!!?!!?");
		return 1;
	}
	
	CLua* component = reinterpret_cast<CLua*>(lua_touserdata(L, -1));
	CTransform* transform = component->entity->transform;
	
	glm::vec3 val = transform->GetScale();
	
	lua_pushnumber(L, val.x);
	lua_pushnumber(L, val.y);
	lua_pushnumber(L, val.z);
	
	return 3;
}

lFuncImp(mEntity, getRotation)
{
	lua_settop(L,1);
	
	lua_getfield(L, 1, "__coreComponent__");
	if(lua_isnil(L, -1))
	{
		Terminal.Warning("no core component found on this entity. What did you do?!!?!!?");
		return 1;
	}
	
	CLua* component = reinterpret_cast<CLua*>(lua_touserdata(L, -1));
	CTransform* transform = component->entity->transform;
	
	glm::vec3 val = transform->GetEulerAngles();
	
	lua_pushnumber(L, val.x);
	lua_pushnumber(L, val.y);
	lua_pushnumber(L, val.z);
	
	return 3;
}


#define lTransformSet(FUNC, CALL)\
	lFuncImp(mEntity, FUNC )\
	{\
		lua_settop(L,2);\
		\
		lua_getfield(L, 1, "__coreComponent__");\
		if(lua_isnil(L, -1))\
		{\
			Terminal.Warning("no core component found on this entity. What did you do?!!?!!?");\
			return 0;\
		}\
		\
		CLua* component = reinterpret_cast<CLua*>(lua_touserdata(L, -1));\
		CTransform* transform = component->entity->transform;\
		\
		transform->CALL(lua_tonumber(L, 2));\
		\
		return 0;\
	}

//SETTERS
lTransformSet(setX, SetPositionX);
lTransformSet(setY, SetPositionY);
lTransformSet(setZ, SetPositionZ);

lTransformSet(setScaleX, SetScaleX);
lTransformSet(setScaleY, SetScaleY);
lTransformSet(setScaleZ, SetScaleZ);

lTransformSet(setPitch, SetPitch);
lTransformSet(setYaw, SetYaw);
lTransformSet(setRoll, SetRoll);

lFuncImp(mEntity, setPosition)
{
	lua_settop(L,4);
	
	lua_getfield(L, 1, "__coreComponent__");
	if(lua_isnil(L, -1))
	{
		Terminal.Warning("no core component found on this entity. What did you do?!!?!!?");
		return 1;
	}
	
	CLua* component = reinterpret_cast<CLua*>(lua_touserdata(L, -1));
	CTransform* transform = component->entity->transform;
	
	glm::vec3 val;
	val.x = lua_tonumber(L,2);
	val.y = lua_tonumber(L,3);
	val.z = lua_tonumber(L,4);
	
	transform->SetPosition(val);
	
	return 0;
}

lFuncImp(mEntity, setScale)
{
	lua_settop(L,4);
	
	lua_getfield(L, 1, "__coreComponent__");
	if(lua_isnil(L, -1))
	{
		Terminal.Warning("no core component found on this entity. What did you do?!!?!!?");
		return 1;
	}
	
	CLua* component = reinterpret_cast<CLua*>(lua_touserdata(L, -1));
	CTransform* transform = component->entity->transform;
	
	glm::vec3 val;
	val.x = lua_tonumber(L,2);
	val.y = lua_tonumber(L,3);
	val.z = lua_tonumber(L,4);
	
	transform->SetScale(val);
	
	return 0;
}

lFuncImp(mEntity, setRotation)
{
	lua_settop(L,4);
	
	lua_getfield(L, 1, "__coreComponent__");
	if(lua_isnil(L, -1))
	{
		Terminal.Warning("no core component found on this entity. What did you do?!!?!!?");
		return 1;
	}
	
	CLua* component = reinterpret_cast<CLua*>(lua_touserdata(L, -1));
	CTransform* transform = component->entity->transform;
	
	glm::vec3 val;
	val.x = lua_tonumber(L,2);
	val.y = lua_tonumber(L,3);
	val.z = lua_tonumber(L,4);
	
	transform->SetRotation(val);
	
	return 0;
}

lTransformSet(addX, TranslateX);
lTransformSet(addY, TranslateY);
lTransformSet(addZ, TranslateZ);

lTransformSet(addScaleX, ScaleX);
lTransformSet(addScaleY, ScaleY);
lTransformSet(addScaleZ, ScaleZ);

lTransformSet(pitch, Pitch);
lTransformSet(yaw, Yaw);
lTransformSet(roll, Roll);

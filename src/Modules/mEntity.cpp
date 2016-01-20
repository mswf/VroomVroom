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
//#include <lua.hpp>
#include "../Utilities/standardIncludes.h"
//#include <new>
#include "../Components/entity.h"

#include "../Components/cLua.h"
#include "../Components/cCamera.h"
#include "../Components/cMeshRenderer.h"
#include "../Components/cTransform.h"
#include "../Components/cDebugRenderer.h"
#include "../Components/cLight.h"
#include "../Components/cCollider.h"

#include "../engine.h"

void mEntity::Bind(lua_State* L)
{
	lua_getglobal(L, "Engine");
	lua_newtable(L);
    lStart(BaseEntity)
        lBind(__engineInit)
		lBind(addChild)
		lBind(addComponent)
	
		lBind(getName)
		lBind(setName)
	
		lBind(getChildren)
		lBind(removeChild)
	
		lBind(destroy)
		lBind(destroyChildren)

	
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
	
	
	/*
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
	 */
}

void mEntity::UnreferenceTable(int tableKey)
{
	lua_State* L = LuaSystem.GetState();
	
	lua_pushnumber(L, tableKey);
	lua_gettable(L, LUA_REGISTRYINDEX);
	
	lua_pushlightuserdata(L, nullptr);
	lua_setfield(L, -2, "__coreComponent__");
	
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
	CLua* comp = new CLua();
	
	lua_pushlightuserdata(L, comp);
    //now add the userdata to our table
    lua_setfield(L, -2, "__coreComponent__");
	
	lua_pushnumber(L, static_cast<int>(CLua::familyId));
	lua_setfield(L, -2, "__familyId__");
	
	lua_pushnil(L);
	lua_setfield(L, -2, "parent");
	
	comp->SetTableKey( luaL_ref(L, LUA_REGISTRYINDEX) );
	
	Entity* e = new Entity();
	Entity::AddComponent(e, comp);
	
	const char* name = comp->entity->GetName().c_str();
	
	lua_pushstring(L, name);
	lua_setfield(L, -2, "name");

    
    return 0;
}

lFuncImp(mEntity, addChild)
{
	lua_settop(L, 2);
	
	lgComp(parent, 1, CLua);
	lgComp(child, 2, CLua);
	
	parent->entity->AddChild(child->entity);
	
	lua_pushvalue(L, 1);
	lua_setfield(L, 2, "parent");
	
	return 0;
}

lFuncImp(mEntity, getName)
{
	lua_settop(L, 1);
	
	lgComp(core, 1, CLua);
	
	const char* name = core->entity->GetName().c_str();
	
	lua_pushstring(L, name);
	lua_setfield(L, 1, "name");
	lua_pushstring(L, name);
	
	return 1;
}

lFuncImp(mEntity, setName)
{
	lua_settop(L, 2);
	
	lgComp(core, 1, CLua);
	lgString(name, 2, "undefined");
	
	core->entity->SetName(name);
	
	lua_pushstring(L, name.c_str());
	lua_setfield(L, 1, "name");
	
	return 0;
}

lFuncImp(mEntity, addComponent)
{
	lua_settop(L, 2);
	
	lgComp(core, 1, CLua);
	lgComp(comp, 2, Component);
	
	lua_getfield(L, 2, "__familyId__");
	if (lua_isnil(L, -1))
	{
		
		return 0;
	}
	int familyId = lua_tonumber(L, -1);
	
	
	lua_pushvalue(L, 2);
	if(familyId == static_cast<int>(ComponentTypes::CAMERA))
	{
		Entity::AddComponent(core->entity, static_cast<CCamera*>(comp));
		lua_setfield(L, 1, "camera");
	}
	else if(familyId == static_cast<int>(ComponentTypes::MESH_RENDERER))
	{
		Entity::AddComponent(core->entity, static_cast<CMeshRenderer*>(comp));
		lua_setfield(L, 1, "meshRenderer");
	}
	else if(familyId == static_cast<int>(ComponentTypes::DEBUG_RENDERER))
	{
		Entity::AddComponent(core->entity, static_cast<CDebugRenderer*>(comp));
		lua_setfield(L, 1, "debugRenderer");
	}
	else if(familyId == static_cast<int>(ComponentTypes::LIGHT))
	{
		Entity::AddComponent(core->entity, static_cast<CLight*>(comp));
		lua_setfield(L, 1, "light");
	}
	else if(familyId == static_cast<int>(ComponentTypes::COLLIDER))
	{
		Entity::AddComponent(core->entity, static_cast<CCollider*>(comp));
		lua_setfield(L, 1, "collider");
	}
	else
	{
		return 0;
		//early return so we don't push the 'entity' value to this table that is not a usable component
	}
	lua_pushvalue(L, 1);
	lua_setfield(L, 2, "entity");
	
	
	return 0;
}

lFuncImp(mEntity, getChildren) {
	lua_settop(L, 1);
	
	lgComp(luaComp, 1, CLua);
	
	std::vector<Entity*> children = luaComp->entity->GetChildren();
	
	lua_newtable(L);
	
	std::vector<Entity*>::const_iterator iter_comp = children.begin();
	std::vector<Entity*>::const_iterator end_comp = children.end();
	int ii = 1;
	for( ; iter_comp != end_comp; ++iter_comp )
	{
		CLua* lua = Entity::GetComponent<CLua>(*iter_comp);
		lua_pushnumber(L, ii);
		lua_pushnumber(L, lua->GetTableKey());
		lua_gettable(L, LUA_REGISTRYINDEX);
		lua_settable(L, -3);
		ii++;
	}
	
	
	return 1;
}

lFuncImp(mEntity, removeChild) {
	lua_settop(L, 2);
	
	lgComp(parent, 1, CLua);
	lgComp(child, 2, CLua);
	
	parent->entity->RemoveChild(child->entity);
	
	return 0;
}

lFuncImp(mEntity, destroy) {
	lua_settop(L, 1);
	
	lgComp(comp, 1, CLua);
	
	Entity::Destroy(comp->entity);
	
	return 0;
}

lFuncImp(mEntity, destroyChildren) {
	lua_settop(L, 1);
	
	lgComp(parent, 1, CLua);
	
	parent->entity->DestroyChildren();
	
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

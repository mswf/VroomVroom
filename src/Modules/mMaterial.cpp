//
//  mMaterial.cpp
//  VroomVroom
//
//  Created by Robin Zaagsma on 30/11/15.
//  Copyright © 2015 Valentinas Rimeika. All rights reserved.
//

#include "mMaterial.h"

#include "../console.h"
#include "../DataStructure/resource_manager.h"
#include "../DataStructure/material.h"

void mMaterial::Bind(lua_State* L)
{
	lua_getglobal(L, "Engine");
	lua_pushcfunction(L, lw_loadMaterial__);
	lua_setfield(L, -2, "loadMaterial");
	
	
	lua_newtable(L);
	lStart(BaseMaterial)
		lBind(__engineInit)
		lBind(setShader)
		lBind(setDiffuseTexture)
		lBind(setSpecularTexture)
		lBind(setNormalTexture)
	lEnd(BaseMaterial)
	luaL_openlib(L, 0, BaseMaterial_funcs, 0);
	lua_setfield(L, -2, "baseMaterial");

}

lFuncImp(mMaterial, loadMaterial)
{
	lua_settop(L, 1);
	
	if(!lua_isstring(L, 1))
	{
		Terminal.Warning("Invalid parameter for Engine.loadModel");
		return 0;
	}
	const char* name = lua_tostring(L, 1);
	
	
	lua_newtable(L);
	
	Material* mat = ResourceManager::getInstance().GetMaterialByName(name);
	
	lua_pushlightuserdata(L, mat);
	lua_setfield(L, -2, "__coreMaterial__");
	
	lua_getglobal(L, "Engine");
	lua_newtable(L);
	lua_getfield(L, -2, "baseMaterial");
	lua_setfield(L, -2, "__index");
	lua_setmetatable(L, 2);
	lua_settop(L, 2);
	return 1;
}

lFuncImp(mMaterial, __engineInit)
{
	lua_settop(L, 1);
	
	Material* mat = new Material();
	
	lua_pushlightuserdata(L, mat);
	lua_setfield(L, -2, "__coreMaterial__");
	return 0;
}

lFuncImp(mMaterial, setShader)
{
	lua_settop(L, 2);
	lua_getfield(L, 1, "__coreMaterial__");
	Material* mat = (Material*)lua_touserdata(L, -1);

	//TODO(bobn): make this
	return 0;
}

lFuncImp(mMaterial, setDiffuseTexture)
{
	lua_settop(L, 2);
	lua_getfield(L, 1, "__coreMaterial__");
	LuaSystem.Dump(L);
	Material* mat = (Material*)lua_touserdata(L, -1);

	mat->SetDiffuseTexture(lua_tostring(L,2));
	
	return 0;
}

lFuncImp(mMaterial, setSpecularTexture)
{
	lua_settop(L, 2);
	lua_getfield(L, 1, "__coreMaterial__");
	Material* mat = (Material*)lua_touserdata(L, -1);
	
	mat->SetSpecularTexture(lua_tostring(L,2));

	return 0;
}

lFuncImp(mMaterial, setNormalTexture)
{
	lua_settop(L, 2);
	lua_getfield(L, 1, "__coreMaterial__");
	Material* mat = (Material*)lua_touserdata(L, -1);
	
	mat->SetNormalTexture(lua_tostring(L,2));

	return 0;
}


//
//  mModel.cpp
//  VroomVroom
//
//  Created by Robin Zaagsma on 30/11/15.
//  Copyright © 2015 Valentinas Rimeika. All rights reserved.
//

#include "../console.h"
#include "mModel.h"
#include "../DataStructure/resource_manager.h"

void mModel::Bind(lua_State* L)
{
	lua_getglobal(L, "Engine");
	lua_pushcfunction(L, lw_getModel__);
	lua_setfield(L, -2, "getModel");
}

lFuncImp(mModel, getModel)
{
	lua_settop(L, 1);
	
	if(!lua_isstring(L, 1))
	{
		Terminal.Warning("Invalid parameter for Engine.loadModel");
		return 0;
	}
	const char* name = lua_tostring(L, 1);
	
	
	lua_newtable(L);
	
	ModelInstance* model = ResourceManager::getInstance().GetModel(name);
	
	lua_pushlightuserdata(L, model);
	lua_setfield(L, -2, "__coreModel__");
	
	return 1;
}

//
//  mMeshRenderer.cpp
//  VroomVroom
//
//  Created by Robin Zaagsma on 01/12/15.
//  Copyright © 2015 Valentinas Rimeika. All rights reserved.
//

#include "mMeshRenderer.h"
#include "../components/cMeshRenderer.h"

//DEBUG INCLUDE
#include "../DataStructure/resource_manager.h"

void mMeshRenderer::Bind(lua_State* L)
{
	lua_getglobal(L, "Engine");
	lua_newtable(L);
	lStart(BaseMeshRenderer)
		lBind(__engineInit)
		lBind(setModel)
		lBind(setMaterial)
	lEnd(BaseEntity)
	luaL_openlib(L, 0, BaseMeshRenderer_funcs, 0);
	lua_setfield(L, -2, "baseMeshRenderer");
}

lFuncImp(mMeshRenderer, __engineInit)
{
	CMeshRenderer* mesh = new CMeshRenderer();
	
	lua_pushlightuserdata(L, mesh);
	lua_setfield(L, -2, "__coreComponent__");
	
	lua_pushnumber(L, CMeshRenderer::familyId);
	lua_setfield(L, -2, "__familyId__");
	
	
	
	//DEBUG CODE
	mesh->SetModel(ResourceManager::getInstance().GetModel("/objects/Rabbit/Rabbit.obj"));
	
	return 0;
}

lFuncImp(mMeshRenderer, setModel)
{
	return 0;
}

lFuncImp(mMeshRenderer, setMaterial)
{
	return 0;
}
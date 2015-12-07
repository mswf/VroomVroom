//
//  mDebugRenderer.cpp
//  VroomVroom
//
//  Created by Robin Zaagsma on 01/12/15.
//  Copyright © 2015 Valentinas Rimeika. All rights reserved.
//

#include "mDebugRenderer.h"
//#include "../components/cDebugRenderer.h"

//DEBUG INCLUDE
#include "../DataStructure/resource_manager.h"

void mDebugRenderer::Bind(lua_State* L)
{
	lua_getglobal(L, "Engine");
	lua_newtable(L);
		lStart(BaseDebugRenderer)
		lBind(__engineInit)
		lBind(addLine)
		lBind(addTriangle)
		lBind(clear)
	lEnd(BaseEntity)
	luaL_openlib(L, 0, BaseDebugRenderer_funcs, 0);
	lua_setfield(L, -2, "baseDebugRenderer");
}

lFuncImp(mDebugRenderer, __engineInit)
{
	return 0;
}

lFuncImp(mDebugRenderer, addLine)
{
	return 0;
}

lFuncImp(mDebugRenderer, addTriangle)
{
	return 0;
}

lFuncImp(mDebugRenderer, clear)
{
	return 0;
}
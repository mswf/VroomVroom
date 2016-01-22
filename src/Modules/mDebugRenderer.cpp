//
//  mDebugRenderer.cpp
//  VroomVroom
//
//  Created by Robin Zaagsma on 01/12/15.
//  Copyright © 2015 Valentinas Rimeika. All rights reserved.
//

#include "mDebugRenderer.h"
#include "../components/cDebugRenderer.h"

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
		lBind(setBufferSize)
		lBind(setDrawPoints)
		lBind(setPointSize)
		lBind(clear)
	lEnd(BaseEntity)
	luaL_openlib(L, 0, BaseDebugRenderer_funcs, 0);
	lua_setfield(L, -2, "baseDebugRenderer");
}

lFuncImp(mDebugRenderer, __engineInit)
{
	CDebugRenderer* debug = new CDebugRenderer();
	
	lua_pushlightuserdata(L, debug);
	lua_setfield(L, -2, "__coreComponent__");
	
	lua_pushnumber(L, CDebugRenderer::familyId);
	lua_setfield(L, -2, "__familyId__");
	
	return 0;
}

lFuncImp(mDebugRenderer, addLine)
{
	lua_settop(L, 11);
	lua_getfield(L, 1, "__coreComponent__");
	CDebugRenderer* debug = static_cast<CDebugRenderer*>(lua_touserdata(L, -1));

	lgFloat(x1, 2, 0);
	lgFloat(y1, 3, 0);
	lgFloat(z1, 4, 0);
	
	lgFloat(x2, 5, 1);
	lgFloat(y2, 6, 1);
	lgFloat(z2, 7, 1);
	
	lgFloat(r, 8, 1);
	lgFloat(g, 9, 0);
	lgFloat(b, 10, 1);
	lgFloat(a, 11, 1);
	
	glm::vec3 start(x1, y1, z1);
	glm::vec3 end(x2, y2, z2);
	glm::vec4 color(r, g, b, a);
	Line line(start, end, color);
	
	debug->AddPrimivite(line);
	
	return 0;
}

lFuncImp(mDebugRenderer, addTriangle)
{
	lua_settop(L, 14);
	lua_getfield(L, 1, "__coreComponent__");
	CDebugRenderer* debug = static_cast<CDebugRenderer*>(lua_touserdata(L, -1));
	
	lgFloat(x1, 2, 1);
	lgFloat(y1, 3, 0);
	lgFloat(z1, 4, 0);
	
	lgFloat(x2, 5, 0);
	lgFloat(y2, 6, 1);
	lgFloat(z2, 7, 0);
	
	lgFloat(x3, 8, 0);
	lgFloat(y3, 9, 0);
	lgFloat(z3, 10, 1);
	
	lgFloat(r, 11, 1);
	lgFloat(g, 12, 0);
	lgFloat(b, 13, 1);
	lgFloat(a, 14, 1);
	
	glm::vec3 pA(x1, y1, z1);
	glm::vec3 pB(x2, y2, z2);
	glm::vec3 pC(x3, y3, z3);
	glm::vec4 color(r, g, b, a);	//TODO, udpate this to rgba vec4
	
	Triangle tri(pA, pB, pC, color);
	
	debug->AddPrimivite(tri);
	
	return 0;
}

lFuncImp(mDebugRenderer, clear)
{
	lua_settop(L, 1);
	lua_getfield(L, 1, "__coreComponent__");
	CDebugRenderer* debug = static_cast<CDebugRenderer*>(lua_touserdata(L, -1));
	
	debug->Clear();

	return 0;
}

lFuncImp(mDebugRenderer, setDrawPoints)
{

	lua_settop(L, 2);
	lua_getfield(L, 1, "__coreComponent__");
	CDebugRenderer* debug = static_cast<CDebugRenderer*>(lua_touserdata(L, -1));

	lgBool(drawPoints, 2, true);

	debug->SetDrawPoints(drawPoints);

	return 0;
}

lFuncImp(mDebugRenderer, setPointSize)
{

	lua_settop(L, 2);
	lua_getfield(L, 1, "__coreComponent__");
	CDebugRenderer* debug = static_cast<CDebugRenderer*>(lua_touserdata(L, -1));

	lgFloat(pointSize, 2, 20.0f);

	debug->SetPointSize(pointSize);

	return 0;
}

lFuncImp(mDebugRenderer, setBufferSize)
{
	
	lua_settop(L, 2);
	lua_getfield(L, 1, "__coreComponent__");
	CDebugRenderer* debug = static_cast<CDebugRenderer*>(lua_touserdata(L, -1));
	
	lgInt(pointNumber, 2, 4096);
	
	debug->SetNumberOfPoints(pointNumber);
	
	return 0;
}
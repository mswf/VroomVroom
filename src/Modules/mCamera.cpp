//
//  mCamera.cpp
//  VroomVroom
//
//  Created by Robin Zaagsma on 08/12/15.
//  Copyright © 2015 Valentinas Rimeika. All rights reserved.
//

#include "mCamera.h"
#include "../console.h"
#include "../components/cCamera.h"
#include "../components/cLua.h"
#include "../components/entity.h"

Renderer::RenderSystem* mCamera::renderer = NULL;

void mCamera::Bind(lua_State* L)
{
	lua_getglobal(L, "Engine");
	
	lua_pushcfunction(L, lw_getActiveCamera__);
	lua_setfield(L, -2, "getActiveCamera");
	
	lua_newtable(L);
	lStart(BaseCamera)
		lBind(__engineInit)
		
		lBind(setProjectionType)
		lBind(setAspectRatio)
		lBind(setFOV)
		lBind(setNearPlaneDistance)
		lBind(setFarPlaneDistance)
		
		lBind(makeActive)
	lEnd(BaseEntity)
	luaL_openlib(L, 0, BaseCamera_funcs, 0);
	lua_setfield(L, -2, "baseCamera");
}

void mCamera::SetRenderer(Renderer::RenderSystem* R)
{
	renderer = R;
}

lFuncImp(mCamera, getActiveCamera)
{
	CCamera* cam = renderer->GetCamera();
	
	CLua* luaComp = Entity::GetComponent<CLua>(cam->entity);
	if(luaComp != NULL)
	{
		lua_pushnumber(L, luaComp->GetTableKey());
		lua_gettable(L, LUA_REGISTRYINDEX);
		lua_getfield(L, -1, "camera");
	}
	else
	{
		lua_pushnil(L);
	}
	
	return 1;
}

lFuncImp(mCamera, __engineInit)
{
	float aspect = renderer->GetWindowWidth()/renderer->GetWindowHeight();
	CCamera* cam = new CCamera(Projection::PERSPECTIVE, aspect);
	
	lua_pushlightuserdata(L, cam);
	lua_setfield(L, -2, "__coreComponent__");
	
	lua_pushnumber(L, CCamera::familyId);
	lua_setfield(L, -2, "__familyId__");

	return 0;
}

lFuncImp(mCamera, setProjectionType)
{
	lua_settop(L, 2);
	lgString(type, 2, "none");
	
	lua_getfield(L, 1, "__coreComponent__");
	CCamera* cam = (CCamera*)lua_touserdata(L, -1);
	
	if (type == "PERSPECTIVE")
	{
		cam->SetProjectionType(Projection::PERSPECTIVE);
	}
	else if (type == "ORTHOGRAPHIC")
	{
		cam->SetProjectionType(Projection::ORTHOGRAPHIC);
	}
	else
 	{
		Terminal.Warning("unknown projection type: "+type);
	}
	
	return 0;
}

lFuncImp(mCamera, setAspectRatio)
{
	lua_settop(L, 2);
	lgFloat(aspect, 2, -1);
	
	lua_getfield(L, 1, "__coreComponent__");
	CCamera* cam = (CCamera*)lua_touserdata(L, -1);
	
	if(aspect > 0){
		cam->SetAspectRatio(aspect);
	}
	else
	{
		Terminal.Warning(string("invalid aspect ratio: ")+lua_tostring(L, 2));
	}
	
	return 0;
}

lFuncImp(mCamera, setFOV)
{
	lua_settop(L, 2);
	lgFloat(fov, 2, -1);
	
	lua_getfield(L, 1, "__coreComponent__");
	CCamera* cam = (CCamera*)lua_touserdata(L, -1);
	
	if(fov > 0 && fov <= 360){
		cam->SetFOV(fov);
	}
	else
	{
		Terminal.Warning(string("invalid FOV: ")+lua_tostring(L, 2));
	}
	
	return 0;
}

lFuncImp(mCamera, setNearPlaneDistance)
{
	lua_settop(L, 2);
	lgFloat(dist, 2, -1);
	
	lua_getfield(L, 1, "__coreComponent__");
	CCamera* cam = (CCamera*)lua_touserdata(L, -1);
	
	if(dist > 0){
		cam->SetNearPlaneDistance(dist);
	}
	else
	{
		Terminal.Warning(string("invalid Near Plane Distance: ")+lua_tostring(L, 2));
	}
	
	return 0;
}

lFuncImp(mCamera, setFarPlaneDistance)
{
	lua_settop(L, 2);
	lgFloat(dist, 2, -1);
	
	lua_getfield(L, 1, "__coreComponent__");
	CCamera* cam = (CCamera*)lua_touserdata(L, -1);
	
	if(dist > 0){
		cam->SetFarPlaneDistance(dist);
	}
	else
	{
		Terminal.Warning(string("invalid Far Plane Distance: ")+lua_tostring(L, 2));
	}
	
	return 0;
}

lFuncImp(mCamera, makeActive)
{
	lua_settop(L, 2);
	lgFloat(dist, 2, -1);
	
	lua_getfield(L, 1, "__coreComponent__");
	CCamera* cam = (CCamera*)lua_touserdata(L, -1);
	
	renderer->SetCamera(cam);
	
	return 0;
}
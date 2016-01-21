//
//  mLight.cpp
//  VroomVroom
//
//  Created by Robin Zaagsma on 05/01/16.
//  Copyright © 2016 Valentinas Rimeika. All rights reserved.
//

#include "mLight.h"
#include "../console.h"
#include "../components/cLight.h"
//#include "../components/cLua.h"
#include "../components/entity.h"

void mLight::Bind(lua_State* L)
{
	lua_getglobal(L, "Engine");
	
	lua_newtable(L);
	lStart(BaseLight)
	lBind(__engineInit)
	
	lBind(setType)
	lBind(getType)

	lBind(setIntensity)
	lBind(getIntensity)

	lBind(setColor)
	lBind(getColor)

	lEnd(BaseLight)
	luaL_openlib(L, 0, BaseLight_funcs, 0);
	lua_setfield(L, -2, "baseLight");
}

lFuncImp(mLight, __engineInit)
{
	CLight* light = new CLight();
	
	lua_pushlightuserdata(L, light);
	lua_setfield(L, -2, "__coreComponent__");
	
	lua_pushnumber(L, CLight::familyId);
	lua_setfield(L, -2, "__familyId__");
	
	return 0;
}

lFuncImp(mLight, setType)
{
	lua_settop(L, 2);
	Terminal.Warning("setType for light has not been implemented yet!");
	return 0;
}

lFuncImp(mLight, getType)
{
	lua_settop(L, 2);
	Terminal.Warning("setType for light has not been implemented yet!");
	return 0;
}

lFuncImp(mLight, setIntensity)
{
	lua_settop(L, 2);
	lua_getfield(L, 1, "__coreComponent__");
	CLight* light = static_cast<CLight*>(lua_touserdata(L, -1));
	
	lgFloat(intensity, 2, 1.0f);
	
	light->SetIntensity(intensity);
	
	return 0;
}

lFuncImp(mLight, getIntensity)
{
	lua_settop(L, 2);
	lua_getfield(L, 1, "__coreComponent__");
	CLight* light = static_cast<CLight*>(lua_touserdata(L, -1));

	lua_pushnumber(L, light->GetIntensity());

	return 1;
}

lFuncImp(mLight, setColor)
{
	lua_settop(L, 4);
	lua_getfield(L, 1, "__coreComponent__");
	CLight* light = static_cast<CLight*>(lua_touserdata(L, -1));
	
	lgFloat(r, 2, 1);
	lgFloat(g, 3, 1);
	lgFloat(b, 4, 1);
	lgFloat(a, 5, 1);
	
	glm::vec4 color(r, g, b, a);
	
	light->SetColour(color);
	
	return 0;
}

lFuncImp(mLight, getColor)
{
	lua_settop(L, 4);
	lua_getfield(L, 1, "__coreComponent__");
	CLight* light = static_cast<CLight*>(lua_touserdata(L, -1));

	glm::vec4 color = light->GetColour();

	lua_pushnumber(L, color.r);
	lua_pushnumber(L, color.g);
	lua_pushnumber(L, color.b);
	lua_pushnumber(L, color.a);

	return 4;
}
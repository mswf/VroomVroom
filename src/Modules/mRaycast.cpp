//
//  mRaycast.cpp
//  VroomVroom
//
//  Created by Robin Zaagsma on 19/01/16.
//  Copyright © 2016 Valentinas Rimeika. All rights reserved.
//

#include "mRaycast.h"
#include "../Engine.h"
#include "../glm/vec3.hpp"

void mRaycast::Bind(lua_State* L)
{
	lua_getglobal(L, "Engine");
	
	lua_newtable(L);
	
	lua_pushcfunction(L, lw_castRay__);
	lua_setfield(L, -2, "castRay");
	
	lua_setfield(L, -2, "raycaster");
}

lFuncImp(mRaycast, castRay)
{
	lua_settop(L, 10);
	
	lgFloat(oX, 1, 1.0);
	lgFloat(oY, 2, 1.0);
	lgFloat(oZ, 3, 1.0);
	
	lgFloat(dX, 4, 1.0);
	lgFloat(dY, 5, 1.0);
	lgFloat(dZ, 6, 1.0);
	
	lgFloat(pX, 7, 1.0);
	lgFloat(pY, 8, 1.0);
	lgFloat(pZ, 9, 1.0);
	
	lgFloat(dist, 10, 1.0);
	
	glm::vec3 origin(oX, oY, oZ);
	glm::vec3 direction(dX, dY, dZ);
	glm::vec3 plane(pX, pY, pZ);
	
	glm::vec3 hitPoint = Engine::CastRay(origin, direction, plane, dist);
	
	lua_pushnumber(L, hitPoint.x);
	lua_pushnumber(L, hitPoint.y);
	lua_pushnumber(L, hitPoint.z);
	
	return 3;
}
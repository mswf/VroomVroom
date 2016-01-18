//
//  mCollider.cpp
//  VroomVroom
//
//  Created by Robin Zaagsma on 18/01/16.
//  Copyright © 2016 Valentinas Rimeika. All rights reserved.
//

#include "mCollider.h"
#include "../components/cCollider.h"
#include "../colliders/boxCollider.h"
#include "../colliders/sphereCollider.h"

void mCollider::Bind(lua_State* L)
{
	lua_getglobal(L, "Engine");
	
	lua_newtable(L);
	lStart(BaseBoxCollider)
		{ "__engineInit" ,lw___boxEngineInit__},
		lBind(collide)
		lBind(setCollisionBox)
		lBind(getCollisionBox)
	lEnd(BaseEntity)
	luaL_openlib(L, 0, BaseBoxCollider_funcs, 0);
	lua_setfield(L, -2, "baseBoxCollider");
	
	
	lua_newtable(L);
	lStart(BaseSphereCollider)
		{ "__engineInit" ,lw___sphereEngineInit__},
		lBind(collide)
		lBind(setRadius)
		lBind(getRadius)
		lEnd(BaseEntity)
	luaL_openlib(L, 0, BaseSphereCollider_funcs, 0);
	lua_setfield(L, -2, "baseSphereCollider");

	lua_settop(L, 0);
}

lFuncImp(mCollider, __boxEngineInit)
{
	BoxCollider* col = new BoxCollider();
	col->SetCollisionBox(1,1,1);
	
	lua_pushlightuserdata(L, col);
	lua_setfield(L, -2, "__coreComponent__");
	
	lua_pushnumber(L, CCollider::familyId);
	lua_setfield(L, -2, "__familyId__");
	
	return 0;
}

lFuncImp(mCollider, __sphereEngineInit)
{
	SphereCollider* col = new SphereCollider();
	col->SetRadius(1);
	
	lua_pushlightuserdata(L, col);
	lua_setfield(L, -2, "__coreComponent__");
	
	lua_pushnumber(L, CCollider::familyId);
	lua_setfield(L, -2, "__familyId__");
	
	return 0;
}

lFuncImp(mCollider, collide)
{
	lua_settop(L, 2);
	
	lgComp(self, 1, CCollider);
	lgComp(other, 1, CCollider);
	
	bool result = self->Collide(*other);
	
	lua_pushboolean(L, result);
	
	return 1;
}

lFuncImp(mCollider, getCollisionBox)
{
	lua_settop(L, 1);
	
	lgComp(col, 1, BoxCollider);
	
	glm::vec3 box = col->GetCollisionBox();
	
	lua_pushnumber(L, box.x);
	lua_pushnumber(L, box.y);
	lua_pushnumber(L, box.z);
	
	return 3;
}

lFuncImp(mCollider, setCollisionBox)
{
	lua_settop(L, 4);
	
	lgComp(col, 1, BoxCollider);
	lgFloat(xx, 2, 1.0);
	lgFloat(yy, 4, 1.0);
	lgFloat(zz, 3, 1.0);
	
	col->SetCollisionBox(xx, yy, zz);
	
	return 0;
}

lFuncImp(mCollider, getRadius)
{
	lua_settop(L, 1);
	
	lgComp(col, 1, SphereCollider);
	
	float radius = col->GetRadius();
	
	lua_pushnumber(L, radius);
	
	return 1;
}

lFuncImp(mCollider, setRadius)
{
	lua_settop(L, 2);
	
	lgComp(col, 1, SphereCollider);
	lgFloat(radius, 2, 1.0);
	
	col->SetRadius(radius);
	
	return 0;
}
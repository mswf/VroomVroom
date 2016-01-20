//
//  cLua.cpp
//  VroomVroom
//
//  Created by Robin Zaagsma on 25/11/15.
//  Copyright © 2015 Valentinas Rimeika. All rights reserved.
//

#include "cLua.h"
#include "../console.h"
#include "../Modules/mEntity.h"
#include "../Systems/LuaSystem.h"
#include <lua.h>

const int CLua::familyId = static_cast<int>(ComponentTypes::LUA);

CLua::CLua() :
	tableKey(-1)
{
	
}

CLua::~CLua()
{
	if(tableKey != -1)
	{
		mEntity::HandleCallback(tableKey, "onDestroy");
		int tmp = tableKey;
		tableKey = - 1;
		mEntity::UnreferenceTable(tmp);
	}
}

void CLua::SetTableKey(int key)
{
	if(tableKey != -1)
	{
		Terminal.Warning("Changing tableKey of Lua Component that already had it's tableKey set.\nThis is likely not intentional");
	}
	tableKey = key;
}

const int& CLua::GetTableKey() const
{
	return tableKey;
}

void CLua::Call()
{
}

void CLua::Update(float delta) const
{
	if (tableKey != -1)
	{
		lua_State* L = LuaSystem.GetState();
		
		lua_pushnumber(L, tableKey);
		lua_gettable(L, LUA_REGISTRYINDEX);
		lua_getfield(L, -1, "update");
		if(lua_isfunction(L, -1))
		{
			lua_pushvalue(L, -2);
			lua_pushnumber(L, delta);
			LuaSystem.Call(L, 2, 0);
		}
		lua_settop(L, 0);
	}
}

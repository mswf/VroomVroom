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

const int CLua::FamilyId = (int)ComponentTypes::LUA;

CLua::CLua() :
	tableKey(-1)
{
	
}

CLua::~CLua()
{
	if(tableKey != -1)
	{
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
	tableKey;
}


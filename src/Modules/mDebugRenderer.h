//
//  mMeshRenderer.hpp
//  VroomVroom
//
//  Created by Robin Zaagsma on 01/12/15.
//  Copyright © 2015 Valentinas Rimeika. All rights reserved.
//

#ifndef mDebugRenderer_h
#define mDebugRenderer_h

#include <stdio.h>
#include "moduleMacros.h"

class mDebugRenderer
{
public:
	static void Bind(lua_State*);
private:
	lFuncDef(__engineInit);
	
	lFuncDef(addLine);
	lFuncDef(addTriangle);
	lFuncDef(clear);
};

#endif /* mDebugRenderer_h */

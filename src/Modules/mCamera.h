//
//  mCamera.hpp
//  VroomVroom
//
//  Created by Robin Zaagsma on 08/12/15.
//  Copyright © 2015 Valentinas Rimeika. All rights reserved.
//

#ifndef mCamera_hpp
#define mCamera_hpp

#include <stdio.h>
#include "moduleMacros.h"

#include "../Systems/renderer.h"

class mCamera
{
public:
	static void Bind(lua_State*);
	static void SetRenderer(Renderer*);
private:
	static Renderer* renderer;
	
	lFuncDef(getActiveCamera);
	
	lFuncDef(__engineInit);
	
	lFuncDef(setProjectionType);
	lFuncDef(setAspectRatio);
	lFuncDef(setFOV);
	lFuncDef(getFOV);
	lFuncDef(setNearPlaneDistance);
	lFuncDef(getNearPlaneDistance);
	lFuncDef(setFarPlaneDistance);
	lFuncDef(getFarPlaneDistance);

	lFuncDef(screenToWorldDirection);
	
	
	lFuncDef(makeActive);
	lFuncDef(isActive);


};

#endif /* mCamera_hpp */

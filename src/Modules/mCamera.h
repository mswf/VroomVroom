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
	static void SetRenderer(Renderer::RenderSystem*);
private:
	static Renderer::RenderSystem* renderer;
	
	lFuncDef(getActiveCamera);
	
	lFuncDef(__engineInit);
	
	lFuncDef(setProjectionType);
	lFuncDef(setAspectRatio);
	lFuncDef(setFOV);
	lFuncDef(setNearPlaneDistance);
	lFuncDef(setFarPlaneDistance);
	
	lFuncDef(makeActive);

};

#endif /* mCamera_hpp */

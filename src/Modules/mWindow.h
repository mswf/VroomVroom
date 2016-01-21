//
//  mWindow.h
//  VroomVroom
//
//  Created by Steff Kempink on 16/12/15.
//  Copyright © 2015 Valentinas Rimeika. All rights reserved.
//

#ifndef mWindow_hpp
#define mWindow_hpp

#include "moduleMacros.h"

class mWindow
{
public:
	static void Bind(lua_State*);
private:
	
	lFuncDef(getSize);
	lFuncDef(getWidth);
	lFuncDef(getHeight);

	lFuncDef(setSize);

	lFuncDef(getPosition);
	lFuncDef(setPosition);

	lFuncDef(getFullscreenMode);
	lFuncDef(setFullscreenMode);

};

#endif /* mWindow_hpp */

//
//  mMeshRenderer.hpp
//  VroomVroom
//
//  Created by Robin Zaagsma on 01/12/15.
//  Copyright © 2015 Valentinas Rimeika. All rights reserved.
//

#ifndef mMeshRenderer_hpp
#define mMeshRenderer_hpp

#include <stdio.h>
#include "moduleMacros.h"

class mMeshRenderer
{
	public:
		static void Bind(lua_State*);
	private:
		lFuncDef(__engineInit);
	
		lFuncDef(setModel);
		lFuncDef(setMaterial);
};

#endif /* mMeshRenderer_hpp */

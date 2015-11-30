//
//  mMaterial.hpp
//  VroomVroom
//
//  Created by Robin Zaagsma on 30/11/15.
//  Copyright © 2015 Valentinas Rimeika. All rights reserved.
//

#ifndef mMaterial_hpp
#define mMaterial_hpp

#include <stdio.h>
#include "moduleMacros.h"

class mMaterial
{
	public:
		static void Bind(lua_State*);
	
	private:
		mMaterial();
		~mMaterial();
	
	lFuncDef(__engineInit);
	
	lFuncDef(loadMaterial);
	lFuncDef(setShader);
	lFuncDef(setDiffuseTexture);
	lFuncDef(setSpecularTexture);
	lFuncDef(setNormalTexture);
	
};

#endif /* mMaterial_hpp */

//
//  mImport.hpp
//  VroomVroom
//
//  Created by Robin Zaagsma on 02/12/15.
//  Copyright © 2015 Valentinas Rimeika. All rights reserved.
//

#ifndef mImport_hpp
#define mImport_hpp

#include <stdio.h>
#include "moduleMacros.h"

class mImport {
	public:
		static void Bind(lua_State*);
	private:
		lFuncDef(importModel);
		//lFuncDef(importMesh);
		//lFuncDef(importMaterial);
		lFuncDef(importTexture);
		//lFuncDef(importShader);
	
		lFuncDef(reloadTexture);
		lFuncDef(reloadShader);
	
		lFuncDef(isModelLoaded);
		lFuncDef(isTextureLoaded);
		lFuncDef(isMaterialLoaded);
		lFuncDef(isShaderLoaded);
};

#endif /* mImport_hpp */

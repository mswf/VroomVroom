//
//  mModel.hpp
//  VroomVroom
//
//  Created by Robin Zaagsma on 30/11/15.
//  Copyright © 2015 Valentinas Rimeika. All rights reserved.
//

#ifndef mModel_hpp
#define mModel_hpp

#include <stdio.h>
#include "moduleMacros.h"

class mModel
{
	public:
		static void Bind(lua_State*);
	private:
		mModel();
		~mModel();
	
		lFuncDef(loadModel);
};

#endif /* mModel_hpp */

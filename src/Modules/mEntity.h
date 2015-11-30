//
//  mEntity.hpp
//  VroomVroom
//
//  Created by Robin Zaagsma on 28/10/15.
//  Copyright © 2015 Valentinas Rimeika. All rights reserved.
//

#ifndef mEntity_h
#define mEntity_h

#include "moduleMacros.h"

class mEntity {
    public :
        static void Bind(lua_State* L);
	
		static void UnreferenceTable(int);
		static void HandleCallback(int, const char*);
    private :
        mEntity();
        ~mEntity();
    
        lFuncDef(__engineInit);
		lFuncDef(addChild);
		lFuncDef(addComponent);
	
	
        lFuncDef(gcDestroy);
};

#endif /* mEntity_h */

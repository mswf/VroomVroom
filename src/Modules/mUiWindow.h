//
//  mUi.hpp
//  VroomVroom
//
//  Created by Robin Zaagsma on 03/11/15.
//  Copyright © 2015 Valentinas Rimeika. All rights reserved.
//

#ifndef mUiWindow_h
#define mUiWindow_h

#include "moduleMacros.h"

class mUiWindow {
    public:
        static void Bind(lua_State*);
	
		static void HandleButtonCallback(lua_State*, int);
		static void HandleWindowClose(lua_State*, int);

    private:
        mUiWindow();
        ~mUiWindow();
    
        lFuncDef(create);
		lFuncDef(close);
	
        lFuncDef(mtIndex);
        lFuncDef(mtNewIndex);
    
        lFuncDef(addText);
        lFuncDef(addButton);
		lFuncDef(addTree);
    
        lFuncDef(remove);
};

#endif /* mUiWindow_h */

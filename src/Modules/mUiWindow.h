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

struct uiElement;

class mUiWindow {
    public:
        static void Bind(lua_State*);
	
		static void UnreferenceTable(lua_State*, int);
		static void HandleCallback(lua_State*, int, const char*);
	
    private:
        mUiWindow();
        ~mUiWindow();
	
		static void BasicElementBind(lua_State*, uiElement*, int);
	
        lFuncDef(create);
		lFuncDef(close);
	
        lFuncDef(mtIndex);
        lFuncDef(mtNewIndex);
    
        lFuncDef(addText);
        lFuncDef(addButton);
		lFuncDef(addTree);
		lFuncDef(addInputText);
		lFuncDef(addCheckbox);
		lFuncDef(addSlider);
		lFuncDef(addRegion);
	
        lFuncDef(remove);
};

#endif /* mUiWindow_h */

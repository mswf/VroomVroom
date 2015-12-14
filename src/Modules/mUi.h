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

class mUi {
    public:
        static void Bind(lua_State*);
	
		static void UnreferenceTable(int);
		static void HandleCallback(int, const char*);
		static void ToggleFocus(bool);
	
    private:
        mUi();
        ~mUi();
	
		static void BasicElementBind(lua_State*, uiElement*, int);
	
        lFuncDef(createWindow);
		lFuncDef(setTooltip);
	
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
		lFuncDef(addHorizontalLayout);
	
		lFuncDef(destroy);
        lFuncDef(remove);
		lFuncDef(removeChildren);
};

#endif /* mUiWindow_h */

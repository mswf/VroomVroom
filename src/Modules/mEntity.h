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
	
		lFuncDef(getName);
		lFuncDef(setName);
	
		lFuncDef(getChildren);
		lFuncDef(removeChild);
	
		lFuncDef(destroy);
		lFuncDef(destroyChildren);
	
		//GETTERS
		lFuncDef(getX);
		lFuncDef(getY);
		lFuncDef(getZ);
		
		lFuncDef(getScaleX);
		lFuncDef(getScaleY);
		lFuncDef(getScaleZ);
		
		lFuncDef(getPitch);
		lFuncDef(getYaw);
		lFuncDef(getRoll);
		
		lFuncDef(getPosition);
		lFuncDef(getScale);
		lFuncDef(getRotation);
		
		
		//SETTERS
		lFuncDef(setX);
		lFuncDef(setY);
		lFuncDef(setZ);
		
		lFuncDef(setScaleX);
		lFuncDef(setScaleY);
		lFuncDef(setScaleZ);
		
		lFuncDef(setPitch);
		lFuncDef(setYaw);
		lFuncDef(setRoll);
		
		lFuncDef(setPosition);
		lFuncDef(setScale);
		lFuncDef(setRotation);
		
		
		//INCREMENT
		lFuncDef(addX);
		lFuncDef(addY);
		lFuncDef(addZ);
		
		lFuncDef(addScaleX);
		lFuncDef(addScaleY);
		lFuncDef(addScaleZ);
		
		lFuncDef(pitch);
		lFuncDef(yaw);
		lFuncDef(roll);
		
		lFuncDef(gcDestroy);
};

#endif /* mEntity_h */

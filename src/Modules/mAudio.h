//
//  mAudio.hpp
//  VroomVroom
//
//  Created by Robin Zaagsma on 16/12/15.
//  Copyright © 2015 Valentinas Rimeika. All rights reserved.
//

#ifndef mAudio_hpp
#define mAudio_hpp

#include <stdio.h>

#include "moduleMacros.h"


class mAudio
{
public:
	static void Bind(lua_State*);
private:
	lFuncDef(play);
	
};


#endif /* mAudio_hpp */

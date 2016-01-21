//
//  mAudio.cpp
//  VroomVroom
//
//  Created by Robin Zaagsma on 16/12/15.
//  Copyright © 2015 Valentinas Rimeika. All rights reserved.
//

#include "mAudio.h"
#include "../Engine.h"
#include "../Content.h"
#include <iostream>

void mAudio::Bind(lua_State* L)
{
	lua_getglobal(L, "Engine");
	lua_pushcfunction(L, lw_play__);
	lua_setfield(L, -2, "playSound");
}

lFuncImp(mAudio, play)
{
	lua_settop(L, 1);
	lgString(path, 1, "");
	
	FMOD::Sound      *sound1;
	FMOD::Channel    *channel = 0;
	
	FMOD_RESULT result = Engine::systemLowLevel->createStream( (Content::GetPath() + "/" +path).c_str(), FMOD_DEFAULT, 0, &sound1);

	if (result == FMOD_ERR_FILE_NOTFOUND)
	{
		std::cout <<  "Error: FMOD did not find the file" << std::endl;
		return 0;
	}
	sound1->setMode(FMOD_LOOP_OFF);
	Engine::systemLowLevel->playSound(sound1, 0, false, &channel);

	
	return 0;
}
#include "content.h"
#include "Engine.h"
#if WEIKIE
#include "Tests/TestCode.h"
#endif

#include "fmod.hpp"

#include <iostream>

int main(int argc, char** a)
{
	Engine engine;
	engine.InitSDL();
	engine.InitSDLNet();
	engine.Init();
#if WEIKIE
	TestCode t;
	//t.RunTCPTest();
#endif
    
#ifdef DEBUG
	Content::PrintPath();
#endif
    
	engine.UpdateLoop();
	
	return 0;
}

/*
 FMOD::System     *system;
	FMOD::Sound      *sound1;
	FMOD::ChannelGroup * group = 0;
	FMOD::Channel    *channel = 0;
	FMOD_RESULT       result;
	unsigned int      version;
	void             *extradriverdata = 0;
	FMOD::DSP		 *dspecho;
	
	result = FMOD::System_Create(&system);
	result = system->getVersion(&version);
	if (version < FMOD_VERSION)
	{
 std::cout <<  "Error version is not the FMOD version" << std::endl;
	}
 
	result = system->init(32, FMOD_INIT_NORMAL, extradriverdata);
	if (result == FMOD_ERR_INITIALIZATION)
	{
 std::cout <<  "Error initializing FMOD" << std::endl;
	}
	result = system->createDSPByType( FMOD_DSP_TYPE_ECHO, &dspecho );
	if (result != FMOD_OK)
	{
 std::cout <<  "Error: FMOD did not create digital signal processor type echo" << std::endl;
	}
	result = dspecho->setParameterFloat(FMOD_DSP_ECHO_DELAY, 150.0f);
	result = dspecho->setParameterFloat(FMOD_DSP_ECHO_WETLEVEL, 150.0f);
	result = system->getMasterChannelGroup(&group);
	group->addDSP(FMOD_CHANNELCONTROL_DSP_HEAD, dspecho);
	if (result != FMOD_OK)
	{
 std::cout <<  "Error: FMOD did not add effect to channel" << std::endl;
	}
	
 
	result = system->createSound("piano2.wav", FMOD_DEFAULT, 0, &sound1);
	if (result == FMOD_ERR_FILE_NOTFOUND)
	{
 std::cout <<  "Error: FMOD did not find the file" << std::endl;
	}
	result = sound1->setMode(FMOD_LOOP_OFF);
	//result = system->playSound(sound1, 0, false, &channel);
	*/

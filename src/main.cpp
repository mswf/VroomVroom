#include "content.h"
#include "Engine.h"
#if WEIKIE
#include "Tests/TestCode.h"
#endif

#include "fmod.hpp"

#include <iostream>

int main(int argc, char** a)
{
    FMOD::System     *system;
	FMOD::Sound      *sound1;
	//FMOD::Channel    *channel = 0;
	FMOD_RESULT       result;
	unsigned int      version;
	void             *extradriverdata = 0;
	
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
	
	result = system->createSound("piano2.wav", FMOD_DEFAULT, 0, &sound1);
	if (result == FMOD_ERR_FILE_NOTFOUND)
	{
		std::cout <<  "Error: FMOD did not find the file" << std::endl;
	}
	result = sound1->setMode(FMOD_LOOP_OFF);
	//result = system->playSound(sound1, 0, false, &channel);
	
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

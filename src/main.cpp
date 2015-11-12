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
	FMOD::Sound      *sound1, *sound2, *sound3;
	FMOD::Channel    *channel = 0;
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
	
	result = system->createSound("piano2.wav", FMOD_DEFAULT, 0, &sound1);
	
	result = sound1->setMode(FMOD_LOOP_OFF);
	
	result = system->playSound(sound1, 0, false, &channel);
	
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

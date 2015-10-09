#include "content.h"
#include "Engine.h"
#include "Console.h"


int main(int argc, char** a)
{
    
    Console::Init();
	Engine engine;
	engine.InitSDL();
	engine.InitSDLNet();

	engine.OpenConfig();
	Content::PrintPath();

	engine.UpdateLoop();
    
	return 0;
}

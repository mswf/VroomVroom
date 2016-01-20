#include "content.h"
#include "Engine.h"

int main(int argc, char** a)
{
	Engine engine;
	engine.InitSDL();
	engine.InitSDLNet();
	engine.Init();

#ifdef DEBUG
	Content::PrintPath();
#endif

	engine.UpdateLoop();

	return 0;
}
#include "content.h"
#include "Engine.h"


int main(int argc, char** a)
{
	Engine engine;
	engine.InitSDL();
	engine.InitSDLNet();

	engine.OpenConfig();
	Content::PrintPath();

	engine.UpdateLoop();

	return 0;
}

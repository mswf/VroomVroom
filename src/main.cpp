#include "content.h"
#include "Engine.h"
#if WEIKIE
#include "Tests/TestCode.h"
#endif

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

	engine.OpenConfig();
    
#ifdef DEBUG
	Content::PrintPath();
#endif
    
	engine.UpdateLoop();

	return 0;
}

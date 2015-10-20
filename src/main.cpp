#include "content.h"
#include "Engine.h"

#if WEIKIE
#include "Tests/TestCode.h"
#endif
#include "Utilities/helperFunctions.h"

int main(int argc, char** a)
{
	Engine engine;
	engine.InitSDL();
	engine.InitSDLNet();
	engine.Init();


#if WEIKIE
	TestCode::RunBufferTest();
	//TestCode::RunTCPTest();
#endif

	engine.OpenConfig();

#ifdef DEBUG
	Content::PrintPath();
#endif

	engine.UpdateLoop();
	return 0;
}

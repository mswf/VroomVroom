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

	std::vector<char> buff;

	short test = 3276;
	short testValue2 = 4234;
	char* buffer = new char[4];
	buffer[0] = ((char*)&test)[0];
	buffer[1] = ((char*)&test)[1];
	buffer[2] = ((char*)&testValue2)[0];
	buffer[3] = ((char*)&testValue2)[1];
	short result = *(short*)&buffer[0];
	short result2 = *(short*)&buffer[2];


#if WEIKIE
	//TestCode::RunTCPTest();
#endif

	engine.OpenConfig();

#ifdef DEBUG
	Content::PrintPath();
#endif

	engine.UpdateLoop();
	return 0;
}

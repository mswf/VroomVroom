#include "content.h"
#include "Engine.h"
#include "Networking/TCPClient.h"
#include "Networking/TCPServer.h"


int main(int argc, char** a)
{
	Engine engine;
	engine.InitSDL();
	engine.InitSDLNet();



	TCPServer server(6112);
	TCPClient client("localhost", 6112);
	client.ReceiveMessage();


	engine.OpenConfig();
	Content::PrintPath();
	engine.runMainLua();

	engine.UpdateLoop();




	return 0;
}

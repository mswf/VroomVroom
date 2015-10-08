#include "content.h"
#include "Engine.h"
#include "Networking/udpmessage.h"
#include "Networking/TCPClient.h"
#include "Networking/TCPServer.h"

void UDPTest()
{
	UDPMessage sock(UDPMessage::StringToIP("localhost", 0));
	sock.SendUnreliableMessage(4);
	Uint8 abc = 10;
	SDL_Delay(100);
	sock.ReceiveMessage(abc);

	int size = sizeof("asd");
	int size2 = sizeof(abc);
	int size3 = sizeof(sock);
}

int ServerLoop(void* data)
{
	TCPServer* server = new TCPServer(6112);

	while (true)
	{
		server->AcceptConnections();
		server->ReceiveMessage();
	}
	return 0;
}

int ClientLoop(void* data)
{
	TCPClient* client = new TCPClient("localhost", 6112);
	std::string msg = "lol u ded";
	int size = sizeof(msg);
	client->SendReliableMessage((void*)msg.c_str(), size);
	client->ReceiveMessage();
	return 0;
}

void RunTCPTest()
{
	SDL_Thread* thread1 = SDL_CreateThread(ServerLoop, "serverThread", NULL);
	SDL_Thread* thread2 = SDL_CreateThread(ClientLoop, "clientThread", NULL);


	while (true)
	{

	}
}

int main(int argc, char** a)
{

	Engine engine;
	engine.InitSDL();
	engine.OpenConfig();
	Content::PrintPath();
	engine.runMainLua();

	engine.UpdateLoop();

	return 0;
}

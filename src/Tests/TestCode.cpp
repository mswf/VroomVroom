#include "TestCode.h"
#include <Networking/udpmessage.h>
#include <SDL2/SDL.h>
#include <Networking/TCPClient.h>
#include <Networking/TCPServer.h>
#include <Utilities/random.h>
#include "../Utilities/helperFunctions.h"

TCPServer* TestCode::server;
TCPClient* TestCode::client;
std::vector<string> TestCode::lipsumList;

TestCode::TestCode()
{
	FillRandomStringList();
}


TestCode::~TestCode()
{
}

void TestCode::UDPTest()
{
	UDPMessage sock(UDPMessage::StringToIP("localhost", 0));
	sock.SendUnreliableMessage(4);
	Uint8 abc = 10;
	SDL_Delay(100);
	sock.ReceiveMessage(abc);

}

int TestCode::ServerLoop(void* data)
{
	while (true)
	{
		server->AcceptConnections();
		server->ReceiveMessage();
	}
	return 0;
}

int TestCode::ClientLoop(void* data)
{
	while (true)
	{
		std::vector<NetworkData> messages = client->ReceiveMessage();
		for (std::vector<NetworkData>::iterator i = messages.begin(); i != messages.end(); ++i)
		{
			char* msg = (char*)i->data;
			printf("client Received a message: \"%s\"\n", msg);
		}
		SDL_Delay(100);

	}
	return 0;
}

void TestCode::StringTest()
{
	std::string msg = "Client says hello";

	client->SendMessage(msg);

	std::vector<string> sentMessages;

	for (int i = 0; i < 20; ++i)
	{
		string str = GetRandomString();
		sentMessages.push_back(str);
		server->SendMessage(str);
	}

	SDL_Delay(500);
	std::vector<NetworkData> receivedMessages = client->ReceiveMessage();
	assert(receivedMessages.size() == sentMessages.size());
	for (int i = 0; i < receivedMessages.size(); ++i)
	{
		const char * sentMsg = sentMessages.at(i).c_str();
		string receivedMsg = HelperFunctions::VoidPtrToString(receivedMessages.at(i).data, receivedMessages.at(i).length);
		assert(sentMsg == receivedMsg);
	}

	printf("StringTest Succeeded\n");
}

void TestCode::FillRandomStringList()
{
	lipsumList.push_back("Lorem ipsum dolor sit amet,\n");
	lipsumList.push_back("consectetur adipiscing elit. Proin sed\n");
	lipsumList.push_back("ultricies elit. Aliquam eget turpis massa.\n");
	lipsumList.push_back("Nunc commodo, dolor non condimentum dictum, purus purus tincidunt magna, eget laoreet\n");
	lipsumList.push_back("erat diam ac nullptra. Curabitur feugiat neque at lorem sagittis porta.\n");
	lipsumList.push_back("Nam accumsan iaculis est nec finibus. Pellentesque facilisis diam sed augue vulputate, sed bibendum ex sagittis.\n");
	lipsumList.push_back("nullptra turpis justo, volutpat id\n");
	lipsumList.push_back("nunc id, placerat luctus magna. Morbi\n");
	lipsumList.push_back("lacinia rutrum purus in mattis. \n");
}

std::string TestCode::GetRandomString()
{
	int count = lipsumList.size();
	int rnd = Random::Next(count);
	string txt = lipsumList.at(rnd);
	return txt;
}

void TestCode::IntTest()
{

	client->SendMessage(1);
	server->SendMessage(2);
	server->SendMessage(3);
	server->SendMessage(4);
}

void TestCode::RunTCPTest()
{
	server = new TCPServer(6113);
	client = new TCPClient("localhost", 6113);

	SDL_Thread* thread1 = SDL_CreateThread(ServerLoop, "serverThread", nullptr);
	//SDL_Thread* thread2 = SDL_CreateThread(ClientLoop, "clientThread", nullptr);

	StringTest();
	//IntTest();

	while (true)
	{
		SDL_Delay(100);
	}
}


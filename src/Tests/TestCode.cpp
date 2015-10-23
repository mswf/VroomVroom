#include "TestCode.h"
#include <SDL2/SDL.h>
#include <SDL2_net/SDL_net.h>
#include "../Networking/udpmessage.h"
#include "../Networking/TCPClient.h"
#include "../Networking/TCPServer.h"
#include "../Utilities/random.h"
#include "../Utilities/helperFunctions.h"
#include "../standardIncludes.h"

TCPServer* TestCode::server;
TCPClient* TestCode::client;
std::vector<string> TestCode::lipsumList;
bool TestCode::TCPTestRunning = false;

void TestCode::RunBufferTest()
{
	const int bufferSize = 15;
	char* buffer = new char[bufferSize];

	int16 shortValue = 123;
	int16 shortValue2;
	char charValue = 'a';
	char charValue2;
	float floatValue = 3.1424f;
	float floatValue2;
	double doubleValue = 48723;
	double doubleValue2;
	string strValue = "xyz";
	string strValue2;

	int writeIndex = 0;
	int readIndex = 0;


	HelperFunctions::InsertIntoBuffer(buffer, writeIndex, shortValue);
	assert(writeIndex == 2);

	HelperFunctions::InsertIntoBuffer(buffer, writeIndex, charValue);
	assert(writeIndex == 3);

	HelperFunctions::InsertIntoBuffer(buffer, writeIndex, floatValue);
	assert(writeIndex == 7);

	HelperFunctions::InsertIntoBuffer(buffer, writeIndex, doubleValue);
	assert(writeIndex == 15);

	//HelperFunctions::InsertIntoBuffer(buffer, writeIndex, strValue); //28 bytes what?
	//assert(writeIndex == 43);


	HelperFunctions::ReadFromBuffer(buffer, readIndex, shortValue2);
	assert(readIndex == 2);
	HelperFunctions::ReadFromBuffer(buffer, readIndex, charValue2);
	assert(readIndex == 3);
	HelperFunctions::ReadFromBuffer(buffer, readIndex, floatValue2);
	assert(readIndex == 7);
	HelperFunctions::ReadFromBuffer(buffer, readIndex, doubleValue2);
	assert(readIndex == 15);

	//HelperFunctions::ReadFromBuffer(buffer, readIndex, strValue2);
	//assert(readIndex == 43);

	assert(writeIndex == readIndex);
	assert(writeIndex == bufferSize);
	assert(shortValue == shortValue2);
	assert(charValue == charValue2);
	assert(floatValue == floatValue2);
	assert(doubleValue == doubleValue2);
	//assert(strValue == strValue2);
	delete buffer;
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
	//this only works on static functions
	server->SetConnectionAcceptedEvent(&ServerOnConnectAccept);
	//use this for members
	//server->SetConnectionAcceptedEvent(std::bind(&TestCode::OnClientConnected, this, std::placeholders::_1));


	while (TCPTestRunning)
	{
		server->AcceptConnections();
		server->ReceiveMessage();
	}
	return 0;
}

void TestCode::ServerOnConnectAccept(TCPsocket socket)
{
	printf("asd");
}

int TestCode::ClientLoop(void* data)
{
	while (true)
	{
		std::vector<NetworkData> messages = client->ReceiveMessage();
		for (std::vector<NetworkData>::iterator i = messages.begin(); i != messages.end(); ++i)
		{
			char* msg = static_cast<char*>(i->data);
			printf("client Received a message: \"%s\"\n", msg);
		}
		SDL_Delay(100);

	}
	return 0;
}

void TestCode::StringTest()
{
	std::string msg = "Client says hello";
	FillRandomStringList();

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
	lipsumList.push_back("erat diam ac nulla. Curabitur feugiat neque at lorem sagittis porta.\n");
	lipsumList.push_back("Nam accumsan iaculis est nec finibus. Pellentesque facilisis diam sed augue vulputate, sed bibendum ex sagittis.\n");
	lipsumList.push_back("Nulla turpis justo, volutpat id\n");
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
	TCPTestRunning = true;
	const int port = 6117;
	server = new TCPServer(port);
	client = new TCPClient("localhost", port);

	SDL_Thread* thread1 = SDL_CreateThread(ServerLoop, "serverThread", NULL);
	//SDL_Thread* thread2 = SDL_CreateThread(ClientLoop, "clientThread", NULL);

	StringTest();
	//IntTest();

	//while (true)
	{
		SDL_Delay(1000);
		TCPTestRunning = false;
		SDL_Delay(100);
		//break;
		delete client;
		delete server;
	}
}


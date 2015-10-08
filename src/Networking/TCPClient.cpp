#include "TCPClient.h"
#include "stdio.h"
#include <assert.h>


TCPClient::TCPClient(const std::string hostName, const uint16 port)
{
	Initialize(hostName.c_str(), port);
}


TCPClient::TCPClient(const char* hostName, const uint16 port)
{
	Initialize(hostName, port);
}

TCPClient::~TCPClient()
{
	SDLNet_TCP_Close(socket);
}

void TCPClient::Initialize(const char* hostName, const uint16 port)
{
	if (SDLNet_ResolveHost(&ip, hostName, port) == -1)
	{
		printf("SDLNet_ResolveHost: %s\n", SDLNet_GetError());
		printf("Cannot connect to host/port\n");
		assert(false);
	}


	socket = SDLNet_TCP_Open(&ip);
	if (!socket)
	{
		printf("SDLNet_TCP_Open: %s\n", SDLNet_GetError());
		printf("Could not open TCP connection\n");
		assert(false);
	}
}

void TCPClient::SendMessage(const void* data, const uint32 length) const
{
	int result = SDLNet_TCP_Send(socket, data, length);
	if (result < length)
	{
		printf("SDLNet_TCP_Send: %s\n", SDLNet_GetError());
		printf("serverSocket has been disconnected\n");
		assert(false);
	}
	else
	{
		printf("Client: sent data to server\n");
	}
}

void TCPClient::SendMessage(std::string msg) const
{
	SendMessage((void*) msg.c_str(), msg.size());
}

void TCPClient::SendMessage(int16 msg) const
{
	SendMessage((void*)msg, sizeof(msg));
}

void TCPClient::SendMessage(int32 msg) const
{
	SendMessage((void*)msg, sizeof(msg));
}

void TCPClient::ReceiveMessage()
{
	int result;
	char msg[1024];

	result = SDLNet_TCP_Recv(socket, msg, MAX_MESSAGE_LENGTH);
	if (result <= 0)
	{
		// An error may have occured, but sometimes you can just ignore it
		// It may be good to disconnect sock because it is likely invalid now.
		//printf("Error", msg);
	}
	printf("Received: \"%s\"\n", msg);
}
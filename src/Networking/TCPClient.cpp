#include "TCPClient.h"
#include "stdio.h"
#include <assert.h>


TCPClient::TCPClient(char* hostname, unsigned int port)
{
	if (SDLNet_ResolveHost(&ip, hostname, port) == -1)
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

TCPClient::~TCPClient()
{
	SDLNet_TCP_Close(socket);
}

void TCPClient::SendReliableMessage(void* data, int length)
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
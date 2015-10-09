#include "TCPServer.h"
#include "stdio.h"
#include <assert.h>

TCPServer::TCPServer(unsigned port)
{
	if (SDLNet_ResolveHost(&ip, NULL, port) == -1)
	{
		printf("SDLNet_ResolveHost: %s\n", SDLNet_GetError());
		printf("Cannot connect to host/port \n");
		assert(false);
	}


	serverSocket = SDLNet_TCP_Open(&ip);
	if (!serverSocket)
	{
		printf("SDLNet_TCP_Open: %s\n", SDLNet_GetError());
		printf("Could not open TCP connection\n");
		assert(false);
	}
}

TCPServer::~TCPServer()
{
	SDLNet_TCP_Close(serverSocket);
}

void TCPServer::SendMessage(std::string msg) const
{
	SendMessage((void*)msg.c_str(), sizeof(msg));
}

void TCPServer::AcceptConnections()
{
	TCPsocket new_tcpsock = SDLNet_TCP_Accept(serverSocket);
	if (new_tcpsock == NULL)
	{
		//printf("SDLNet_TCP_Accept: %s\n", SDLNet_GetError());
	}
	else
	{
		// communicate over new_tcpsock
		clients.push_back(new_tcpsock);
		printf("[Server] Client connected\n");
	}
}

void TCPServer::SendMessage(void* data, int length) const
{
	for (std::vector<TCPsocket>::const_iterator i = clients.begin(); i != clients.end(); ++i)
	{
		int result = SDLNet_TCP_Send((TCPsocket) * i, data, length);
		if (result < length)
		{
			printf("SDLNet_TCP_Send: %s\n", SDLNet_GetError());
			printf("[Server] socket has been disconnected \n");
			//assert(false);
		}
		else
		{
			printf("[Server] Data sent.\n");
		}
	}
}

void TCPServer::ReceiveMessage()
{
	int result;
	char msg[1024];

	for (int i = 0; i < clients.size(); ++i)
	{
		result = SDLNet_TCP_Recv(clients[i], msg, MAX_MESSAGE_LENGTH);
		if (result <= 0)
		{
			// An error may have occured, but sometimes you can just ignore it
			// It may be good to disconnect sock because it is likely invalid now.
			//printf("Error", msg);
		}
		else
		{
			printf("[Server] Received: \"%s\"\n", msg);
		}
	}


}
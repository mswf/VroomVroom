#include "TCPServer.h"
#include "stdio.h"
#include <assert.h>
#include "standardIncludes.h"

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
		//SDL is most likely not initialized yet
		printf("SDLNet_TCP_Open: %s\n", SDLNet_GetError());
		printf("Could not open TCP connection\n");
		assert(false);
	}
}

TCPServer::~TCPServer()
{
	SDLNet_TCP_Close(serverSocket);
}

void TCPServer::SetConnectionAcceptedEvent(std::function<void(TCPsocket socket)> callbackFunction)
{
	//std::bind()
	OnConnectionAcceptedCallBack = callbackFunction;
}

void TCPServer::AcceptConnections()
{
	TCPsocket newTcpSock = SDLNet_TCP_Accept(serverSocket);
	if (newTcpSock == NULL)
	{
		//printf("SDLNet_TCP_Accept: %s\n", SDLNet_GetError());
	}
	else
	{
		// communicate over new_tcpsock
		clients.push_back(newTcpSock);
		printf("[Server] Client connected\n");
		//add function callback that has TCPsocket as parameter
		if (OnConnectionAcceptedCallBack != NULL)
		{
			OnConnectionAcceptedCallBack(newTcpSock);
		}
	}
}

void TCPServer::SendData(const void* data, const uint32 length, TCPsocket socket) const
{
	int result = SDLNet_TCP_Send(socket, data, length);
	if (result < length)
	{
		printf("SDLNet_TCP_Send: %s\n", SDLNet_GetError());
		printf("[Server] socket has been disconnected \n");
	}
}

void TCPServer::SendData(const void* data, const uint32 length) const
{
	for (std::vector<TCPsocket>::const_iterator it = clients.begin(); it != clients.end(); ++it)
	{
		SendData(data, length, *it);
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
			//echo to all clients except the client sending it
			EchoData(msg, result, clients[i]);
			//printf("[Server] Received: \"%s\"\n", msg);
		}
	}
}

void TCPServer::EchoData(const void* data, const uint32 length, const TCPsocket socket) const
{
	for (std::vector<TCPsocket>::const_iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if (*it != socket)
		{
			int result = SDLNet_TCP_Send((TCPsocket) * it, data, length);
			if (result < length)
			{
				printf("SDLNet_TCP_Send: %s\n", SDLNet_GetError());
				printf("[Server] socket has been disconnected \n");
			}
		}
	}
}
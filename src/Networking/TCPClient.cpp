#include "TCPClient.h"
#include "stdio.h"
#include <assert.h>
#include "SDL2/SDL.h"

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
	alive = false;
	SDLNet_TCP_Close(socket);
}

void TCPClient::Initialize(const char* hostName, const uint16 port)
{
	if (SDLNet_ResolveHost(&ip, hostName, port) == -1)
	{
		printf("SDLNet_ResolveHost: %s\n", SDLNet_GetError());
		printf("[TCPClient] Cannot connect to host/port\n");
	}

	socket = SDLNet_TCP_Open(&ip);
	if (!socket)
	{
		printf("SDLNet_TCP_Open: %s\n", SDLNet_GetError());
		printf("[TCPClient] Could not open TCP connection\n");
	}
	else
	{
		alive = true;
		printf("[TCPClient] Connected\n");
	}

	listenThread = SDL_CreateThread(ListenForMessages, "clientThread", this);
}

void TCPClient::SendMessage(const void* data, const uint32 length) const
{
	if (length >= MAX_MESSAGE_LENGTH)
	{
		assert(false);
	}

	int result = SDLNet_TCP_Send(socket, data, length);
	if (result < length)
	{
		printf("SDLNet_TCP_Send: %s\n", SDLNet_GetError());
		printf("serverSocket has been disconnected\n");
		assert(false);
	}
	else
	{
		printf("[TCPClient] sent data to server\n");
	}
}

void TCPClient::SendMessage(std::string msg) const
{
	SendMessage((void*)msg.c_str(), sizeof(msg));
}

void TCPClient::SendMessage(int16 msg) const
{
	SendMessage((void*)msg, sizeof(msg));
}

void TCPClient::SendMessage(int32 msg) const
{
	SendMessage((void*)msg, sizeof(msg));
}

std::vector<NetworkData> TCPClient::ReceiveMessage()
{
	std::vector<NetworkData> copy = std::vector<NetworkData>(dataCache);
	dataCache.clear();
	return copy;
}

int TCPClient::ListenForMessages(void* tcpClient)
{
	TCPClient* client = (TCPClient*)tcpClient;
	while (client->alive)
	{
		int result;
		char msg[1024];

		result = SDLNet_TCP_Recv(client->socket, msg, client->MAX_MESSAGE_LENGTH);
		if (result <= 0)
		{
			// An error may have occured, but sometimes you can just ignore it
			// It may be good to disconnect sock because it is likely invalid now.
			//printf("Error", msg);
			return 1;
		}
		printf("[TCPClient] Received: \"%s\"\n", msg);

		NetworkData networkData;
		networkData.data = (void*)msg;
		networkData.length = result;
		client->dataCache.push_back(networkData);

	}
	return 0;
}
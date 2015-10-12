#include "TCPClient.h"
#include "stdio.h"
#include <assert.h>
#include "SDL2/SDL.h"
#include <memory>

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
	SDL_DestroyMutex(mutex);
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
		//SDL is most likely not initialized yet
		printf("SDLNet_TCP_Open: %s\n", SDLNet_GetError());
		printf("[TCPClient] Could not open TCP connection\n");
	}
	else
	{
		alive = true;
		printf("[TCPClient] Connected\n");
	}
	mutex = SDL_CreateMutex();
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

void TCPClient::SendMessage(const std::string& msg) const
{
#if __APPLE__
	SendMessage((void*)msg.c_str(), msg.size());
#else
	SendMessage((void*)msg.c_str(), msg.size());
#endif
}

void TCPClient::SendMessage(const int16& msg) const
{
	SendMessage(&msg, sizeof(msg));
}

void TCPClient::SendMessage(const int32& msg) const
{
	SendMessage(&msg, sizeof(msg));
}

std::vector<NetworkData> TCPClient::ReceiveMessage()
{
	std::vector<NetworkData> copy;
	if (SDL_LockMutex(mutex) == 0)
	{
		if (dataCache.size() > 0)
		{
			printf("something");
			printf("AFTER: %s \n", (char*)dataCache[0].data);

		}

		copy = std::vector<NetworkData>(dataCache);
		dataCache.clear();
		SDL_UnlockMutex(mutex);

	}
	else
	{
		printf(SDL_GetError());
		alive = false;
	}
	return copy;
}

bool TCPClient::IsConnected()
{
	return alive;
}

int TCPClient::ListenForMessages(void* tcpClient)
{
	TCPClient* client = (TCPClient*)tcpClient;
	while (client->alive)
	{
		int result;
		unsigned char msg[MAX_MESSAGE_LENGTH];
		std::fill(msg, msg + MAX_MESSAGE_LENGTH, 0);

		result = SDLNet_TCP_Recv(client->socket, msg, client->MAX_MESSAGE_LENGTH);
		if (result <= 0)
		{
			// An error may have occured, but sometimes you can just ignore it
			// It may be good to disconnect sock because it is likely invalid now.
			client->alive = false;
			return 1;
		}
		printf("[TCPClient] Received: \"%s\"\n", (char*)msg);

		NetworkData networkData;
		networkData.length = result;
		networkData.data = new unsigned char[MAX_MESSAGE_LENGTH];
		memcpy(networkData.data, msg, MAX_MESSAGE_LENGTH);

		if (SDL_LockMutex(client->mutex) == 0)
		{
			client->dataCache.push_back(networkData);
			SDL_UnlockMutex(client->mutex);
		}
		else
		{
			printf(SDL_GetError());
			client->alive = false;
		}
	}
	return 0;
}

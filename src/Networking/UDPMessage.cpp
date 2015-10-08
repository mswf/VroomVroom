#include "udpmessage.h"
#include "SDL2_net/SDL_net.h"
#include <string>
#include <assert.h>


UDPMessage::UDPMessage(IPaddress ip)
{
	socket = SDLNet_UDP_Open(ip.port);
	this->ip = ip;
}

void UDPMessage::SendUnreliableMessage(Uint8 data)
{
	UDPpacket packet;
	packet.address = ip;
	packet.data = &data;
	packet.len = sizeof(data);

	int sent = SDLNet_UDP_Send(socket, -1, &packet);
	if (sent == 0)
	{
		printf("SDLNet_UDP_SendV: %s\n", SDLNet_GetError());
	}
}

IPaddress UDPMessage::StringToIP(std::string stringIP, int port)
{
	IPaddress ip;
	int result = SDLNet_ResolveHost(&ip, stringIP.c_str(), port);
	if (result == -1)
	{
		printf("SDLNet_ResolveHost: %s\n", SDLNet_GetError());
		assert(false);
		//return NULL;
	}
	return ip;
}

bool UDPMessage::ReceiveMessage(Uint8& outValue)
{
	// try to receive a waiting udp packet
	UDPpacket packet;

	int numrecv = SDLNet_UDP_Recv(socket, &packet);

	if (numrecv != 0)
	{
		// do something with packet
		outValue = *packet.data;
		return true;
	}
	return false;
}
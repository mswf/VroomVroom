#include "NetMessage.h"


NetMessage::NetMessage():
	client(NULL)
{
}


NetMessage::~NetMessage()
{
}

void NetMessage::SendMessage(NetMessageType messageType, const void* data, int16 size)
{
	//combine messageType and data
	char* buffer = new char[size + 2];
	buffer[0] << (short)messageType;
	//buffer[1]
	//buffer[2] << *data;
}

void NetMessage::SendMessage(NetMessageType type, NetworkData data)
{

}

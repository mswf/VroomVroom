#include "TCPClient.h"
#include <Utilities/helperFunctions.h>

enum class NetMessageType
{
	SyncPosition = 0, //[enityID,pos,time]

};

class NetMessage
{
	public:
		NetMessage();
		~NetMessage();
		template <typename T>
		void SendMessage(NetMessageType messageType, const T& data);
	private:
		TCPClient* client;
};

template <typename T>
void NetMessage::SendMessage(NetMessageType messageType, const T& data)
{
	int typeSize = sizeof(T);
	int messageTypeSize = sizeof(NetMessageType);
	char* buffer = new char[typeSize + messageTypeSize];
	HelperFunctions::InsertIntoBuffer(buffer, 0, messageType);
	HelperFunctions::InsertIntoBuffer(buffer, messageTypeSize, data);
	client->SendMessage(buffer, typeSize + messageTypeSize);
}

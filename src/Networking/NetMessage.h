#include "TCPClient.h"
#include "../Utilities/helperFunctions.h"
#include "../standardIncludes.h"

enum class NetMessageType : short
{
	SyncPosition = 0,	//[entityID,(vec3)pos,(float)time]
	SyncRotation,		//[entityID,(quat)rot,(float)time]
	SyncVelocity,		//[entityID,(vec3)vel,(float)time]
	SyncNpc
};

class NetMessage
{
	public:
		NetMessage();
		~NetMessage();
		template <typename T>
		void SendMessage(NetMessageType messageType, const T& data);
		NetworkData ReceiveMessage();
	private:
		TCPClient* client;
		const short NET_MESSAGE_TYPE_SIZE = 2;
};

template <typename T>
void NetMessage::SendMessage(const NetMessageType messageType, const T& data)
{
	int typeSize = sizeof(T);
	assert(NET_MESSAGE_TYPE_SIZE == sizeof(NetMessageType));
	char* buffer = new char[typeSize + NET_MESSAGE_TYPE_SIZE];
	HelperFunctions::InsertIntoBuffer(buffer, 0, messageType);
	HelperFunctions::InsertIntoBuffer(buffer, NET_MESSAGE_TYPE_SIZE, data);
	client->SendMessage(buffer, typeSize + NET_MESSAGE_TYPE_SIZE);
	delete buffer;
}


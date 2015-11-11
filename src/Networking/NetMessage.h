#ifndef NET_MESSAGE_H
#define NET_MESSAGE_H

#include "TCPClient.h"
#include "../Utilities/helperFunctions.h"
#include "../standardIncludes.h"

enum class NetMessageType : short
{
	None = 0,
	SyncPosition,		//[entityID,(vec3)pos,(float)time]
	SyncRotation,			//[entityID,(quat)rot,(float)time]
	SyncVelocity,			//[entityID,(vec3)vel,(float)time]
	SyncNpc,				//[entityID,(mat4)matrix]
	SyncPlayer,				//[playerNumber,(mat4)matrix]
	PlayerNumber,			//[playerNumber]
	InitializeCompleted,	//[]
	RequestMessage,
	PlayerMatrixChange,
	TimeSync,
	TimeSyncResponse,
	PlayerVelocityChange
};

class NetMessage
{
	public:
		NetMessage();
		~NetMessage();
		template <typename T>
		void SendNetMessage(NetMessageType messageType, const T& data);
		NetworkData ReceiveMessage();
	private:
		TCPClient* client;
		const short NET_MESSAGE_TYPE_SIZE = 2;
};

template <typename T>
void NetMessage::SendNetMessage(const NetMessageType messageType, const T& data)
{
	int typeSize = sizeof(T);
	assert(NET_MESSAGE_TYPE_SIZE == sizeof(NetMessageType));
	char* buffer = new char[typeSize + NET_MESSAGE_TYPE_SIZE];
	int index = 0;
	HelperFunctions::InsertIntoBuffer(buffer, index, messageType);
	HelperFunctions::InsertIntoBuffer(buffer, index, data);
	client->SendMessageChar(buffer, typeSize + NET_MESSAGE_TYPE_SIZE);
	delete buffer;
}

#endif
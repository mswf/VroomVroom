#include "NetMessage.h"
#include "../glm/glm.hpp"
#include "guiddef.h"


NetMessage::NetMessage():
	client(NULL)
{
}


NetMessage::~NetMessage()
{
}

NetworkData NetMessage::ReceiveMessage()
{
	std::vector<NetworkData> messageList = client->ReceiveMessage();
	for (std::vector<NetworkData>::iterator it = messageList.begin(); it != messageList.end(); ++it)
	{
		NetMessageType messageType;
		int readIndex = 0;
		HelperFunctions::ReadFromBuffer(it->data, readIndex, messageType);
		switch (messageType)
		{
			case NetMessageType::SyncPosition:
			{
				_GUID* id;
				//_GUID* id2(id);

				glm::vec3 pos(0, 0, 0);
				float time;
				HelperFunctions::ReadFromBuffer(it->data, readIndex, id);
				HelperFunctions::ReadFromBuffer(it->data, readIndex, pos);
				HelperFunctions::ReadFromBuffer(it->data, readIndex, time);
				break;
			}
			case NetMessageType::SyncRotation:
			{
				break;
			}
			case NetMessageType::SyncVelocity:
			{
				break;
			}
			case NetMessageType::SyncNpc:
			{
				break;
			}
			default:
			{
				assert(false);
				break;
			}
		}
	}
	return messageList[0];
}

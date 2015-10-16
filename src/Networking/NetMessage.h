#include "TCPClient.h"

enum class NetMessageType
{
	SyncPosition = 0, //[enityID,pos,time]

};

class NetMessage
{
	public:
		NetMessage();
		~NetMessage();
		void SendMessage(NetMessageType messageType, const void* data, int16 size);
		void SendMessage(NetMessageType type, NetworkData data);
	private:
		TCPClient* client;
};


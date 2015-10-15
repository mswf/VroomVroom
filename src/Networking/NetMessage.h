enum class NetMessageType
{
	SyncPosition = 0, //[enityID,pos,time]

};

class NetMessage
{
	public:
		NetMessage();
		~NetMessage();
		void SendMessage(NetMessageType messageType, const void* data);
};


#include <SDL2_net/SDL_net.h>
#include "standardIncludes.h"
#include <vector>

struct NetworkData
{
	void* data;
	uint32 length;
};

class TCPClient
{
	public:
		TCPClient(const std::string hostName, const uint16 port);
		TCPClient(const char* hostName, const uint16 port);
		~TCPClient();
		void SendMessage(const std::string msg) const;
		void SendMessage(const int16 msg) const;
		void SendMessage(const int32 msg) const;

		std::vector<NetworkData> ReceiveMessage();
	private:
		static int ListenForMessages(void* tcpClient);
		void SendMessage(const void* data, const uint32 length) const;
		void Initialize(const char* hostname, const uint16 port);
	private:
		bool alive;
		SDL_Thread* listenThread;
		IPaddress ip;
		TCPsocket socket;
		std::vector<NetworkData> dataCache;

		const unsigned int MAX_MESSAGE_LENGTH = 1024;
};
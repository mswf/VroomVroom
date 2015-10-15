#include <SDL2_net/SDL_net.h>
#include "TCPData.h"
#include <vector>


class TCPClient : public TCPData
{
	public:
		TCPClient(const std::string hostName, const uint16 port);
		TCPClient(const char* hostName, const uint16 port);
		~TCPClient();

		std::vector<NetworkData> ReceiveMessage();
		bool IsConnected() const;
	private:
		static int ListenForMessages(void* tcpClient);
		void SendData(const void* data, const uint32 length) const override;
		void Initialize(const char* hostname, const uint16 port);
	private:
		bool alive;
		SDL_Thread* listenThread;
		IPaddress ip;
		TCPsocket socket;
		std::vector<NetworkData> dataCache;
		SDL_mutex* mutex;

};
#include <SDL2_net/SDL_net.h>
#include "standardIncludes.h"

class TCPClient
{
	public:
		TCPClient(const std::string hostName, const uint16 port);
		TCPClient(const char* hostName, const uint16 port);
		~TCPClient();
		void SendMessage(const std::string msg) const;
		void SendMessage(const int16 msg) const;
		void SendMessage(const int32 msg) const;

		void ReceiveMessage();
	private:
		void SendMessage(const void* data, const uint32 length) const;
		void Initialize(const char* hostname, const uint16 port);

	private:
		IPaddress ip;
		TCPsocket socket;
		const unsigned int MAX_MESSAGE_LENGTH = 1024;
};
#include <SDL2_net/SDL_net.h>

class TCPClient
{
	public:
		TCPClient(char* hostName, unsigned int port);
		~TCPClient();
		void SendReliableMessage(void* data, int length);
		void ReceiveMessage();
	private:
		IPaddress ip;
		TCPsocket socket;
		const unsigned int MAX_MESSAGE_LENGTH = 1024;
};
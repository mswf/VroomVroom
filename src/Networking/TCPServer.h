#include <SDL2_net/SDL_net.h>
#include <vector>

class TCPServer
{
	public:
		TCPServer(unsigned int port);
		~TCPServer();
		void AcceptConnections();
		void SendReliableMessage(void* data);
		void ReceiveMessage();
	private:
		IPaddress ip;
		TCPsocket serverSocket;
		const unsigned int MAX_MESSAGE_LENGTH = 1024;
		std::vector<TCPsocket> clients;
};
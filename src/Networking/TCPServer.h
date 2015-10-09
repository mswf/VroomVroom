#include <SDL2_net/SDL_net.h>
#include <vector>

class TCPServer
{
	public:
		TCPServer(unsigned int port);
		~TCPServer();
		void SendMessage(std::string msg) const;
		void AcceptConnections();
		void SendMessage(void* data, int length) const;
		void ReceiveMessage();
	private:
		IPaddress ip;
		TCPsocket serverSocket;
		const unsigned int MAX_MESSAGE_LENGTH = 1024;
		std::vector<TCPsocket> clients;
};
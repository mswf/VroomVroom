#include <SDL2_net/SDL_net.h>
#include <vector>
#include <Clock.hpp>
#include "TCPData.h"

class TCPServer : public TCPData
{
	public:
		TCPServer(unsigned int port);
		~TCPServer();

		void AcceptConnections();
		void ReceiveMessage();

	protected:
		virtual void SendData(const void* data, const uint32 length) const override;

	private:
		IPaddress ip;
		TCPsocket serverSocket;
		std::vector<TCPsocket> clients;
};
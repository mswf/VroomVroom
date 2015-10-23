#include <SDL2_net/SDL_net.h>
#include <vector>
#include "TCPData.h"

class TCPServer : public TCPData
{
	public:
		TCPServer(unsigned int port);
		~TCPServer();

		void AcceptConnections();
		void ReceiveMessage();
		void EchoData(const void* data, const uint32 length, const TCPsocket socket) const;
	protected:
		virtual void SendData(const void* data, const uint32 length) const override;

	private:
		IPaddress ip;
		TCPsocket serverSocket;
		std::vector<TCPsocket> clients;
};
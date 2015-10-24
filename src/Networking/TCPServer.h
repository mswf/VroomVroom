#include <SDL2_net/SDL_net.h>
#include <vector>
#include "TCPData.h"
#include <functional>

class TCPServer : public TCPData
{
	public:
		TCPServer(unsigned int port);
		~TCPServer();
		void SetConnectionAcceptedEvent(std::function<void(TCPsocket socket)> callbackFunction);
		void AcceptConnections();
		void ReceiveMessage();
		void EchoData(const void* data, const uint32 length, const TCPsocket& socket) const;
		virtual void SendData(const void* data, const uint32 length, const TCPsocket& socket) const;
	protected:
		virtual void SendData(const void* data, const uint32 length) const override;

	private:
		IPaddress ip;
		TCPsocket serverSocket;
		std::vector<TCPsocket> clients;
		std::function<void(TCPsocket socket)> OnConnectionAcceptedCallBack;
};

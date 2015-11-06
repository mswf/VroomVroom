#ifndef udpmessage_h
#define udpmessage_h
#include <codecvt>
#include <SDL2_net/SDL_net.h>
#include "Utilities/helperFunctions.h"

class UDPMessage// : Singleton<UDPMessage>
{
	public:
		UDPMessage(IPaddress ip);
		void SendUnreliableMessage(Uint8 data);
		static IPaddress StringToIP(std::string stringIP, int16 port);
		bool ReceiveMessage(Uint8& outValue);
	private:
		UDPsocket socket;
		IPaddress ip;
};

#endif /* udpmessage_h */

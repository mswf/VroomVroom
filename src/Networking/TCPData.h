#ifndef NETWORK_DATA_H
#define NETWORK_DATA_H

#include "../standardIncludes.h"

struct NetworkData
{
	char* data;
	uint32 length;
};

class TCPData
{
	public:
		void SendMessage(const char* msg, int length) const;
		void SendMessage(const std::string& msg) const;
		void SendMessage(const int16& msg) const;
		void SendMessage(const int32& msg) const;

	protected:
		TCPData();
		virtual ~TCPData();
		virtual void SendData(const void* data, const uint32 length) const;
		static const unsigned int MAX_MESSAGE_LENGTH = 1024;
};

#endif //NETWORK_DATA_H


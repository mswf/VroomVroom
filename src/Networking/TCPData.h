#ifndef NETWORK_DATA_H
#define NETWORK_DATA_H

#include "../Utilities/standardIncludes.h"

struct NetworkData
{
	char* data;
	uint32 length;
};

class TCPData
{
	public:
		void SendMessage(const std::string& msg) const;
		void SendMessage(const int16& msg) const;
		void SendMessage(const int32& msg) const;

	protected:
		TCPData();
		virtual ~TCPData();
		virtual void SendData(const void* data, const uint32 length) const = 0;
		static const unsigned int MAX_MESSAGE_LENGTH = 1024;
};

#endif //NETWORK_DATA_H


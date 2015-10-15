#include "TCPData.h"


TCPData::TCPData()
{
}


TCPData::~TCPData()
{
}

void TCPData::SendData(const void* data, const uint32 length) const
{
	assert(false);
}
void TCPData::SendMessage(const int16& msg) const
{
	SendData(&msg, sizeof(msg));
}
void TCPData::SendMessage(const std::string& msg) const
{
	SendData(msg.c_str(), msg.size());
}

void TCPData::SendMessage(const int32& msg) const
{
	SendData(&msg, sizeof(msg));
}
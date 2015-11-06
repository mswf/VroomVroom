#include "TCPData.h"


TCPData::TCPData()
{
}


TCPData::~TCPData()
{
}

void TCPData::SendMessage(const int16& msg) const
{
	SendData(&msg, sizeof(msg));
}
void TCPData::SendMessage(const std::string& msg) const
{
	// Valentinas: Casting msg.size() cause of warnings
	SendData(msg.c_str(), (uint32)msg.size());
}

void TCPData::SendMessage(const int32& msg) const
{
	SendData(&msg, sizeof(msg));
}
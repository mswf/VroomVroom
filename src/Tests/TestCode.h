#pragma once
#include <codecvt>
#include <vector>


class TCPClient;
class TCPServer;
class TestCode
{
	public:
		TestCode();
		~TestCode();
		void UDPTest();
		void RunTCPTest();
	private:
		static int ServerLoop(void* data);
		static int ClientLoop(void* data);
		static void StringTest();
		static void FillRandomStringList();
		static std::string GetRandomString();
		static void IntTest();
		static TCPClient* client;
		static TCPServer* server;
		static std::vector<std::string> lipsumList;

};


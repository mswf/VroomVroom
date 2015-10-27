//
//  console.cpp
//  VroomVroom
//
//  Created by Robin Zaagsma on 23/09/15.
//  Copyright © 2015 Valentinas Rimeika. All rights reserved.
//
#include "standardIncludes.h"
#include "console.h"
#include <iostream>
#include "IO/file.h"
#include <time.h>
#include "luaSystem.h"

#include "Networking/TCPClient.h"

#define CONSOLE_PORT 1427

sTerminal::sTerminal():
	socket(NULL),
	logFile(NULL)
{
	socket = new TCPClient("localhost", CONSOLE_PORT);
	CreateLogFile();
	Log("Terminal initialized", true);

	string line = "Stardate: <";

	struct tm time_info = GetTimeStruct();

	line += std::to_string(time_info.tm_year + 1900) + "/";
	line += std::to_string(time_info.tm_mon + 1) + "/";
	line += std::to_string(time_info.tm_mday) + "> ";

	Log(line, true);

}

sTerminal::~sTerminal()
{
	//TODO release resources
}

void sTerminal::Log(const string msg, bool showExternally)
{
	string line = "[ENGINE]" + msg;
	std::cout << line << std::endl;
	WriteToFile(line);
	if (showExternally)
	{
		SendToExternal(line, "#93BDC9", "#3E2000");
	}
}

void sTerminal::Warning(const string msg)
{
	string line = "[WARNING]" + msg;
	std::cout << line << std::endl;
	WriteToFile(line);
	SendToExternal(line, "#ff0000", "#ffffff");
}

void sTerminal::Error(string msg)
{
	//TODO implement this :)
}

void sTerminal::Custom(const string msg, string background, string color)
{
	WriteToFile(msg);
	SendToExternal(msg, background, color);
}

void sTerminal::LuaError(const string msg)
{
	string line = "[LUA]" + msg;
	std::cout << line << std::endl;
	WriteToFile(line);
	SendToExternal(line, "#C5251D", "#70B7BE");

}

//TODO replace with callback when messages are recieved reather thann having to poll each update
void sTerminal::Update()
{
	if (!socket->IsConnected())
	{
		return;
	}
	std::vector<NetworkData> messages = socket->ReceiveMessage();
	std::vector<NetworkData>::iterator it;
	for (it = messages.begin(); it != messages.end(); ++it)
	{
		LuaSystem.Attempt(string((char*)(it->data)));
	}
}


//PRIVATE
void sTerminal::WriteToFile(const string msg)
{
	if (!logFile)
	{
		return;
	}
	string line = "<";

	struct tm time_info = GetTimeStruct();

	line += std::to_string(time_info.tm_hour) + ":";
	line += std::to_string(time_info.tm_min) + ":";
	line += std::to_string(time_info.tm_sec) + "> ";
	line += msg + "\r\n";

	logFile->WriteString(line);
}

void sTerminal::SendToExternal(const string msg, const string background, const string color)
{
	string consoleString = "MSG[|]";
	consoleString += msg + "[|]";
	consoleString += "BG[|]" + background + "[|]";
	consoleString += "CLR[|]" + color + "[|]";

	if (socket->IsConnected())
	{
		socket->SendMessage(consoleString);
	}
}

void sTerminal::CreateLogFile()
{
	for (int ii = 3; ii >= 0; ii--)
	{
		string oldName = "log";
		if (ii > 0)
		{
			oldName += std::to_string(ii + 1);
		}
		oldName += ".txt";

		File* oldFile = new File();
		if (oldFile->Open(oldName))
		{
			string newName = "log";
			newName += std::to_string(ii + 2);
			newName += ".txt";

			File* newFile = new File();
			newFile->Create(newName);

			string oldContent = oldFile->ReadString();
			newFile->WriteString(oldContent);

			delete newFile;
		}
		delete oldFile;
	}
	logFile = new File();
	logFile->Create("log.txt");
}

//TODO move this plssss
struct tm GetTimeStruct()
{
	time_t timer;
	time(&timer);
	struct tm time_info;

#if __APPLE__
	time_info = *localtime(&timer);
#else
	localtime_s(&time_info, &timer);
#endif

	return time_info;
}
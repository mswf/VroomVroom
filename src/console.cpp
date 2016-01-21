//
//  console.cpp
//  VroomVroom
//
//  Created by Robin Zaagsma on 23/09/15.
//  Copyright © 2015 Valentinas Rimeika. All rights reserved.
//
#include "Utilities/standardIncludes.h"
#include "console.h"
#include <iostream>
#include "IO/file.h"
#include "Systems/luaSystem.h"

#include "Networking/TCPClient.h"
#include "Utilities/helperFunctions.h"

#define CONSOLE_PORT 1427

sTerminal::sTerminal():
	socket(nullptr),
	logFile(nullptr),
	reconnectDelay(-1)
{
	socket = new TCPClient("localhost", CONSOLE_PORT);
	if (!socket->IsConnected())
	{
		ReattemptConnection(10000);
	}

	CreateLogFile();
	Log("Terminal initialized", true);

	string line = "Stardate: " + HelperFunctions::GetTimeString();

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

void sTerminal::LogOpenGL(const string msg, bool showExternally)
{
	string line = "[OPENGL]" + msg;
	std::cout << line << std::endl;
	WriteToFile(line);
	if (showExternally)
	{
		SendToExternal(line, "#00CCFF", "#FFFFFF");
	}
}

void sTerminal::LogRender(const string msg, bool showExternally)
{
	string line = "[RENDER]" + msg;
	std::cout << line << std::endl;
	WriteToFile(line);
	if (showExternally)
	{
		SendToExternal(line, "#00CC99", "#FFFFFF");
	}
}

void sTerminal::Warning(const string msg)
{
	string line = "[WARNING]" + msg;
	std::cout << line << std::endl;
	WriteToFile(line);
	SendToExternal(line, "rgba(255,153,0,0.8)", "#ffffff");
}

void sTerminal::Error(string msg)
{
	string line = "[ERROR]" + msg;
	std::cout << line << std::endl;
	WriteToFile(line);
	SendToExternal(line, "#ff0000", "#ffffff");
	//TODO(robin): Better error handling
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

void sTerminal::LuaLinkedError(const string msg, const string cleanMsg)
{
	string line = "[LUA]" + msg;
	SendToExternal(line, "#C5251D", "#70B7BE");

	string cleanLine = "[LUA]" + cleanMsg;
	std::cout << cleanLine << std::endl;
	WriteToFile(cleanLine);
}

//TODO replace with callback when messages are recieved rather than having to poll each update
void sTerminal::Update(int deltaTime)
{
	if (socket->IsConnected())
	{
		std::vector<NetworkData> messages = socket->ReceiveMessage();
		std::vector<NetworkData>::iterator it;
		for (it = messages.begin(); it != messages.end(); ++it)
		{
			string msg = HelperFunctions::VoidPtrToString(it->data, it->length);
			if (msg.find("::ATOM;") == 0)
			{
				size_t sepIndex = msg.find(";", 7);
				string filePath = msg.substr(7, sepIndex - 7);
				int lineNumber = std::stoi(msg.substr(sepIndex + 1));
				LuaSystem.OpenAtom(filePath, lineNumber);
			}
			else if (msg.find("::RESUME;") == 0)
			{
				LuaSystem.Resume();
			}
			else if (msg.find("::HALT;") == 0)
			{
				LuaSystem.Halt();
			}
			else
			{
				LuaSystem.Attempt(msg);
			}
			delete it->data;
		}

	}
	if (reconnectDelay > 0)
	{
		ReattemptConnection(reconnectDelay - deltaTime);
	}
	else if (!socket->IsConnected())
	{
		ReattemptConnection(10000);
	}
}

bool sTerminal::IsConnected() const
{
	return socket->IsConnected();
}

void sTerminal::ReattemptConnection(int delay)
{
	if (socket->IsConnected())
	{
		return;
	}
	if (delay <= 0)
	{
		//TODO: delay value is never read
		delay = -1;
		if (socket != nullptr)
		{
			delete socket;
		}
		Log("Attempting to reconnect with external console");
		socket = new TCPClient("localhost", CONSOLE_PORT);
		if (!socket->IsConnected())
		{
			//try again in 10 seconds
			ReattemptConnection(5000);
		}
	}
	else
	{
		reconnectDelay = delay;
	}
}

void sTerminal::Clear() const
{
	if(socket->IsConnected())
	{
		socket->SendMessage(string("AUTOCLEAR[|]<///>"));
	}
}


//PRIVATE
void sTerminal::WriteToFile(const string msg) const
{
	if (!logFile)
	{
		return;
	}

	logFile->WriteString( HelperFunctions::GetTimeString() + msg + "\r\n");
}

void sTerminal::SendToExternal(const string msg, const string background, const string color) const
{
	string timedMessage = HelperFunctions::GetTimeString() + msg;
	string consoleString = "MSG[|]";
	consoleString += timedMessage + "[|]";
	consoleString += "BG[|]" + background + "[|]";
	consoleString += "CLR[|]" + color + "[|]";

	consoleString += "<///>";

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
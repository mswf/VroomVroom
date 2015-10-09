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
#include "luaSystem.h"

#include "Networking/TCPClient.h"

#define CONSOLE_PORT 1427

sTerminal::sTerminal()
{
	socket = new TCPClient("localhost", CONSOLE_PORT);
	/*
	 if(socket->connected)
	 {
	 //TODO verify socket connection?
	 }
	 */
	//TODO create a file to write the log to, increase the counter of existing log, and cap the amount of logs somewhere (preferably the config.lua, so everyone can have their own amount of log files

	Log("console initialized");

}

sTerminal::~sTerminal()
{
	//TODO release resources
}

void sTerminal::Log(string msg, bool showExternally)
{
	msg = "[ENGINE]\t" + msg;
	std::cout << msg << std::endl;
	WriteToFile(msg);
	if (showExternally)
	{
		SendToExternal(msg, "#93BDC9", "#3E2000");
	}
}

void sTerminal::Warning(string msg)
{
	//TODO implement this :)
}

void sTerminal::Error(string msg)
{
	//TODO implement this :)
}

void sTerminal::Custom(string msg, string background, string color)
{
	SendToExternal(msg, background, color);
}

void sTerminal::LuaError(string msg)
{
    msg = "[LUA]\t" + msg;
    std::cout << msg << std::endl;
    WriteToFile(msg);
    SendToExternal(msg, "#C5251D", "#70B7BE");

}

//TODO replace with callback when messages are recieved reather thann having to poll each update
void sTerminal::Update()
{
    if(!socket->IsConnected())
    {
        return;
    }
    std::vector<NetworkData> messages = socket->ReceiveMessage();
    std::vector<NetworkData>::iterator it;
    for(it = messages.begin(); it != messages.end(); ++it) {
        LuaSystem.Attempt(string((char*)(it->data)));
    }
}


//PRIVATE
void sTerminal::WriteToFile(string msg)
{
	//TODO implement this :)
}

void sTerminal::SendToExternal(string msg, string background, string color)
{
	string consoleString = "MSG;";
	consoleString += msg + ";";
	consoleString += "BG;" + background + ";";
	consoleString += "CLR;" + color + ";";
    
    printf("col: %s\n",consoleString.c_str());

	if (socket->IsConnected())
	{
		socket->SendMessage(consoleString);
	}
}
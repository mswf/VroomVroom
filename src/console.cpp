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

#include "Networking/TCPClient.h"

#define CONSOLE_PORT 1427

sConsole::sConsole()
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

sConsole::~sConsole()
{
    //TODO release resources
}

void sConsole::Log(string msg, bool showExternally)
{
    msg = "[ENGINE]\t"+msg;
    std::cout << msg << std::endl;
    WriteToFile(msg);
    if(showExternally)
    {
        SendToExternal(msg, "#93BDC9", "#3E2000");
    }
}
    
void sConsole::Warning(string msg)
{
    //TODO implement this :)
}
    
void sConsole::Error(string msg)
{
    //TODO implement this :)
}
    
void sConsole::Custom(string msg, string background, string color)
{
    SendToExternal(msg, background, color);
}


//PRIVATE
void sConsole::WriteToFile(string msg)
{
    //TODO implement this :)
}
    
void sConsole::SendToExternal(string msg, string background, string color)
{
    //TODO implement this :)
    string consoleString = "MSG;";
    consoleString += msg + ";";
    consoleString += "BG;"+background+";";
    consoleString += "CLR;"+color+";";
        
    if(socket->IsConnected())
    {
        socket->SendMessage(consoleString);
    }
}
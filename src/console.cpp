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

namespace Console
{
    
    namespace
    {
        bool isInitialized(false);
        TCPClient* socket;
    } //anonymous namespace
    
    void Init()
    {
        if(isInitialized){
            return;
        }
        socket = new TCPClient("localhost", CONSOLE_PORT);
        /*
         if(socket->connected)
         {
            //TODO verify socket connection?
         }
        */
        //TODO create a file to write the log to, increase the counter of existing log, and cap the amount of logs somewhere (preferably the config.lua, so everyone can have their own amount of log files
        
        Log("console initialized");
        
        isInitialized = true;
    }
    
    void Log(string msg, bool showExternally)
    {
        msg = "[ENGINE]\t"+msg;
        std::cout << msg << std::endl;
        __WriteToFile__(msg);
        if(showExternally /*&& socket->connected*/)
        {
            __SendToConsole__(msg, "#93BDC9", "#3E2000");
        }
    }
    
    void Warning(string msg)
    {
        //TODO implement this :)
    }
    
    void Error(string msg)
    {
        //TODO implement this :)
    }
    
    void Custom(string msg, string background, string color)
    {
        __SendToConsole__(msg, background, color);
    }
    
    
    //'private' functions
    namespace
    {
        void __WriteToFile__(string msg)
        {
            //TODO implement this :)
        }
    
        void __SendToConsole__(string msg, string background, string color)
        {
            //TODO implement this :)
            string consoleString = "MSG;";
            consoleString += msg + ";";
            consoleString += "BG;"+background+";";
            consoleString += "CLR;"+color+";";
        
            //if(socket->connected)
            {
                socket->SendMessage(consoleString);
            }
        }
    }
    
}//namespace Console
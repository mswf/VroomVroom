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
#include <SDL2/SDL.h>
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
void sTerminal::WriteToFile(const string msg)
{
    if(!logFile)
    {
        return;
    }
    string line = "<";
    
    struct tm time_info = GetTimeStruct();
    
    line += std::to_string(time_info.tm_hour) + ":";
    line += std::to_string(time_info.tm_min) + ":";
    line += std::to_string(time_info.tm_sec) + "> ";
    line += msg + "\r\n";
        
#if __APPLE__
    Sint64 size = line.size();
#else
//TODO: Weikie figure dis out
    Sint64 size = line.size();
#endif

    size_t len = SDL_RWwrite(logFile, line.c_str(), 1, size);
    if (len != size)
    {
        Warning("Line has not been fully written to log.txt");
    }
}

void sTerminal::SendToExternal(const string msg, const string background, const string color)
{
	string consoleString = "MSG;";
	consoleString += msg + ";";
	consoleString += "BG;" + background + ";";
	consoleString += "CLR;" + color + ";";

	if (socket->IsConnected())
	{
		socket->SendMessage(consoleString);
	}
}

void sTerminal::CreateLogFile()
{
    int ii;
    for (ii = 4; ii >= 0; ii--)
    {
        string oldName = "log";
        if(ii != 0)
        {
            oldName += std::to_string(ii+1);
        }
        oldName += ".txt";
        
        SDL_RWops* oldFile = SDL_RWFromFile(oldName.c_str(), "r");
        if(oldFile != NULL){
            if(ii < 4)
            {
                string newName = "log";
                newName += std::to_string(ii+2);
                newName += ".txt";
                
                SDL_RWops* newFile = SDL_RWFromFile(newName.c_str(), "w");
                if(newFile != NULL){
                    Sint64 size = SDL_RWsize(oldFile);
                    char* buf = (char*)malloc(size);
                    if(buf == NULL)
                    {
                        //shit is going down. Give a warning and try the next file
                        Warning("could not copy contents of "+oldName+", logs might be messed up...");
                        Warning("ˆˆMemory could not be allocated properly");
                        continue;
                    }
                    char* res = buf;
                    Sint64 read_total = 0, read = 1;
                    while(read_total < size && read != 0)
                    {
                        read = SDL_RWread(oldFile, buf, 1, (size - read_total));
                        read_total += read;
                        buf += read;
                    }
                    SDL_RWclose(oldFile);
                    //make sure we actually read the whole file
                    if(read_total == size)
                    {
                        size_t len = SDL_RWwrite(newFile, res, 1, size);
                        if(len != size)
                        {
                            Warning("could not copy contents of "+oldName+", logs might be messed up...");
                            Warning("ˆˆDid not fully write file");
                        }
                    }
                    else
                    {
                        Warning("could not copy contents of "+oldName+", logs might be messed up...");
                        Warning("ˆˆFile was not read completely");
                    }
                    free(res);
                    SDL_RWclose(newFile);
                }
            }
        }
    }
    logFile = SDL_RWFromFile("log.txt", "w");
    
    string line = "Stardate: <";
    
    struct tm time_info = GetTimeStruct();
    
    line += std::to_string(time_info.tm_year + 1900) + "/";
    line += std::to_string(time_info.tm_mon + 1) + "/";
    line += std::to_string(time_info.tm_mday) + "> ";

    Log(line, true);
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
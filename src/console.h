//
//  console.hpp
//  VroomVroom
//
//  Created by Robin Zaagsma on 08/10/15.
//  Copyright © 2015 Valentinas Rimeika. All rights reserved.
//

#ifndef console_h
#define console_h

#include "typedef.h"
#include "patterns/singleton.h"

#define Terminal sTerminal::getInstance()

class TCPClient;
class SDL_RWops;

class sTerminal : public Singleton<sTerminal>
{
	public:
		sTerminal();
		~sTerminal();
		void Log(string, bool = false);
		void Warning(string);
		void Error(string);
		void Custom(string, string, string);
        void LuaError(string);
    
        void Update();
	private:
		void WriteToFile(string);
		void SendToExternal(string, string, string);
        void CreateLogFile();

		TCPClient* socket;
        SDL_RWops* logFile;
};

#endif /* console_h */

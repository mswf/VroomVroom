//
//  console.hpp
//  VroomVroom
//
//  Created by Robin Zaagsma on 08/10/15.
//  Copyright © 2015 Valentinas Rimeika. All rights reserved.
//

#ifndef console_h
#define console_h

#include "Utilities/typedef.h"
#include "Patterns/singleton.h"

#define Terminal sTerminal::getInstance()

class TCPClient;
class File;

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
        File* logFile;
};

//TODO move this out of console
struct tm GetTimeStruct();

#endif /* console_h */

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

#define Console sConsole::getInstance()

class TCPClient;

class sConsole : public Singleton<sConsole>
{
    public:
        sConsole();
        ~sConsole();
        void Log(string, bool = false);
        void Warning(string);
        void Error(string);
        void Custom(string, string, string);
    
    private:
        void WriteToFile(string);
        void SendToExternal(string, string, string);
    
        TCPClient* socket;
};

#endif /* console_h */

//
//  console.hpp
//  VroomVroom
//
//  Created by Robin Zaagsma on 08/10/15.
//  Copyright © 2015 Valentinas Rimeika. All rights reserved.
//

#ifndef console_h
#define console_h

#include <stdio.h>
#include "typedef.h"

namespace Console{
    void Init();
    void Log(string, bool = false);
    void Warning(string);
    void Error(string);
    void Custom(string, string, string);
    
    namespace
    {
        void __WriteToFile__(string);
        void __SendToConsole__(string, string, string);
    }
}

#endif /* console_h */

//
//  command.cpp
//  VroomVroom
//
//  Created by Robin Zaagsma on 09/11/15.
//  Copyright © 2015 Valentinas Rimeika. All rights reserved.
//

#include "command.h"
#include <stdlib.h>
#include "../console.h"

void RunCommand(string cmd)
{
    if(!system(NULL))
    {
        Terminal.Warning(cmd);
        Terminal.Warning("could not run command, system is not available");
    }
    system(cmd.c_str());
};

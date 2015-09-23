//
//  content.hpp
//  VroomVroom
//
//  Created by Robin Zaagsma on 23/09/15.
//  Copyright © 2015 Valentinas Rimeika. All rights reserved.
//

#ifndef content_h
#define content_h

#include <stdio.h>

namespace Content
{
    void SetPath(const char* path, unsigned long len);
    const char* GetPath();
    void PrintPath();
    void CreateFilePath(const char* filename, char* outResult);
}

#endif /* content_h */

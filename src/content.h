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
#include <string>

namespace Content
{
    void SetPath(const char* path, unsigned long len);
    const std::string& GetPath();
    void PrintPath();
    void CreateFilePath(const char* filename, std::string* outResult);
}

#endif /* content_h */

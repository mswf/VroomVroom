//
//  content.cpp
//  VroomVroom
//
//  Created by Robin Zaagsma on 23/09/15.
//  Copyright © 2015 Valentinas Rimeika. All rights reserved.
//

#include "content.h"
#include <cstring>
#include <iostream>

namespace Content
{

namespace
{
    char* contentPath = NULL;
} //anonymous namespace
    
void SetPath(const char* path, unsigned long len)
{
    std::cout << "setting content path to " << path << std::endl;

    if(contentPath == NULL)
    {
        contentPath = new char[len];
        std::strcpy(contentPath, path);
    }
    else
    {
        //TODO(robin) give some sort of warning
    }
}
    
const char* GetPath(){
    return contentPath;
}
    
void PrintPath()
{
    std::cout << "content path is " << contentPath << std::endl;
}
    
void CreateFilePath(const char* filename, char* outResult)
{
    std::strcpy(outResult, contentPath);
    std::strcat(outResult,"/");
    std::strcat(outResult, filename);
}
    
}//namespace Content
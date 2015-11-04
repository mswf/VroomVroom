//
//  content.cpp
//  VroomVroom
//
//  Created by Robin Zaagsma on 23/09/15.
//  Copyright © 2015 Valentinas Rimeika. All rights reserved.
//
#include "Utilities/standardIncludes.h"
#include "content.h"
#include <cstring>
#include <iostream>

namespace Content
{

namespace
{
    std::string contentPath = "";
} //anonymous namespace
    
void SetPath(const char* path, unsigned long len)
{
#ifdef DEBUG
    std::cout << "setting content path to " << path << std::endl;
#endif
    
    if(contentPath == "")
    {
        contentPath = std::string(path);
    }
    else
    {
        //TODO(robin) give some sort of warning
    }
}
    
const std::string GetPath(){
    return contentPath;
}

// TODO: Print path to the console
void PrintPath()
{
    std::cout << "content path is " << contentPath << std::endl;
}
    
void CreateFilePath(const char* filename, string* outResult)
{
    *outResult = contentPath;
	*outResult += "/";
	*outResult += string(filename);
}
    
}//namespace Content
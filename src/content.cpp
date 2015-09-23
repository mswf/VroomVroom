//
//  content.cpp
//  VroomVroom
//
//  Created by Robin Zaagsma on 23/09/15.
//  Copyright © 2015 Valentinas Rimeika. All rights reserved.
//
#include "standardIncludes.h"
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
    std::cout << "setting content path to " << path << std::endl;

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
    
void PrintPath()
{
    std::cout << "content path is " << contentPath << std::endl;
}
    
void CreateFilePath(const char* filename, string* outResult)
{
    outResult = new string(contentPath);
	*outResult += "/";
	*outResult += filename;
}
    
}//namespace Content
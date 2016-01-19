//
//  content.cpp
//  VroomVroom
//
//  Created by Robin Zaagsma on 23/09/15.
//  Copyright © 2015 Valentinas Rimeika. All rights reserved.
//
#include "Utilities/standardIncludes.h"
#include "content.h"
//#include <cstring>
//#include <iostream>

namespace Content
{

namespace
{
    std::string contentPath = "";
} //anonymous namespace
    
void SetPath(const char* path, unsigned long len)
{
    if(contentPath == "")
    {
        contentPath = std::string(path);
#ifdef DEBUG
		Terminal.Log( string( "Setting content path to " ) + contentPath );
#endif
    }
    else
    {
        //TODO(robin) give some sort of warning
    }
}
    
const string& GetPath(){
    return contentPath;
}

void PrintPath()
{
	Terminal.Log( string("Content path is ") + contentPath );
}
    
void CreateFilePath(const char* filename, string* outResult)
{
    *outResult = contentPath;
	*outResult += "/";
	*outResult += string(filename);
}
    
}//namespace Content
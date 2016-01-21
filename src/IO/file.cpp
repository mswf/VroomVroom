//
//  File.cpp
//  VroomVroom
//
//  Created by Robin Zaagsma on 12/10/15.
//  Copyright © 2015 Valentinas Rimeika. All rights reserved.
//

#include "File.h"
#include "../Utilities/standardIncludes.h"
#include <SDL2/SDL.h>

File::File() :
    file(nullptr)
{
    
}

File::File(string path) :
    file(nullptr)
{
    if (!Open(path))
    {
        Create(path);
    }
}

File::~File()
{
    Close();
}

bool File::Open(const string path)
{
    file = SDL_RWFromFile(path.c_str(), "r+");
    if(file == nullptr){
        return false;
    }
    fileName = path;
    return true;
}

void File::Create(const string path)
{
    file = SDL_RWFromFile(path.c_str(), "w");
    Close();
    Open(path);
}

Sint64 File::Size() const
{
	if(file == nullptr)
    {
        return 0;
    }
	return SDL_RWsize(file);
}

string File::ReadString()
{
    if(file == nullptr)
    {
        return string("");
    }
    string result;
    ReadStringInto(&result, Size());
    return result;
}

void File::ReadStringInto(string* outString, Sint64 size) const
{
    if(file == nullptr)
    {
        return;
    }
    char* buf = static_cast<char*>(malloc(size + 1));
    if(buf == nullptr)
    {
        return;
    }
    char* res = buf;
    Sint64 read_total = 0, read = 1;
    while(read_total < size && read != 0)
    {
        read = SDL_RWread(file, buf, 1, (size - read_total));
        read_total += read;
        buf += read;
    }
    res[size] = 0;
    
    if(read_total == size)
    {
        *outString = string(res);
    }
    free(res);
}

void File::WriteString(const string& data) const
{
    if(file == nullptr)
    {
        return;
    }
    Sint64 size = data.size();
	//TODO(Robin): Check for errors!
    size_t len = SDL_RWwrite(file, data.c_str(), 1, size);
}

void File::Close()
{
    if(file != nullptr){
        SDL_RWclose(file);
        file = nullptr;
    }
}

void File::Clear()
{
    if(file != nullptr){
        Close();
        
        SDL_RWops* tmp = SDL_RWFromFile(fileName.c_str(), "w");
        SDL_RWclose(tmp);

        file = SDL_RWFromFile(fileName.c_str(), "r+");
    }
   
}

//TODO(Weikie): add a getError function
//
//  File.cpp
//  VroomVroom
//
//  Created by Robin Zaagsma on 12/10/15.
//  Copyright © 2015 Valentinas Rimeika. All rights reserved.
//

#include "File.h"
#include "standardIncludes.h"
#include <SDL2/SDL.h>

File::File() :
    file(NULL)
{
    
}

File::File(string path) :
    file(NULL)
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
    if(file == NULL){
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

Sint64 File::Size()
{
    if(file == NULL)
    {
        return 0;
    }
    else
    {
        return SDL_RWsize(file);
    }
}

string File::ReadString()
{
    if(file == NULL)
    {
        return string("");
    }
    string result;
    ReadStringInto(&result, Size());
    return result;
}

void File::ReadStringInto(string* outString, Sint64 size)
{
    if(file == NULL)
    {
        return;
    }
    char* buf = (char*)malloc(size + 1);
    if(buf == NULL)
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

void File::WriteString(const string& data)
{
    if(file == NULL)
    {
        return;
    }
    Sint64 size = data.size();
    size_t len = SDL_RWwrite(file, data.c_str(), 1, size);
    //TODO check if full data got written
}

void File::Close()
{
    if(file != NULL){
        SDL_RWclose(file);
        file = NULL;
    }
}

void File::Clear()
{
    if(file != NULL){
        Close();
        
        SDL_RWops* tmp = SDL_RWFromFile(fileName.c_str(), "w");
        SDL_RWclose(tmp);

        file = SDL_RWFromFile(fileName.c_str(), "r+");
    }
   
}

//TODO add a getError function
//
//  File.hpp
//  VroomVroom
//
//  Created by Robin Zaagsma on 12/10/15.
//  Copyright © 2015 Valentinas Rimeika. All rights reserved.
//

#ifndef File_hh
#define File_hh

#include "../Utilities/typedef.h"

struct SDL_RWops;

class File {
    public :
        File();
        File(const string);
        ~File();
        bool Open(const string);
        void Create(const string);
        Sint64 Size();
    
        string ReadString();
        void ReadStringInto(string*, Sint64);
        void WriteString(const string &);
    
        void Close();
        void Clear();
    
    private:
        SDL_RWops* file;
        string fileName;
    
};

#endif /* File_h */

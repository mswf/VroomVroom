//
//  mEntity.hpp
//  VroomVroom
//
//  Created by Robin Zaagsma on 28/10/15.
//  Copyright © 2015 Valentinas Rimeika. All rights reserved.
//

#ifndef mEntity_h
#define mEntity_h

#include "moduleMacros.h"

//TODO remove this once we have proper entities
class StubEntity
{
    public :
        StubEntity() {};
        ~StubEntity() {};
        void DoPls() { printf("%i\n", pls); };
        int pls;
};

class mEntity {
    public :
        static void Bind(lua_State* L);
    private :
        mEntity();
        ~mEntity();
    
        lFuncDef(create);
        lFuncDef(destroy);
        lFuncDef(doPls);
};

#endif /* mEntity_h */

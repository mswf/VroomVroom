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

//TODO(robin) remove this once we have proper entities
class StubEntity
{
    public :
        StubEntity() {};
        ~StubEntity() {};
        void DoPls() { printf("pls\n"); };
        int pls;
};

class mEntity {
    public :
        static void Bind(lua_State* L);
    private :
        mEntity();
        ~mEntity();
    
        lFuncDef(__engineInit);
        lFuncDef(destroy);
        lFuncDef(doPls);
};

#endif /* mEntity_h */

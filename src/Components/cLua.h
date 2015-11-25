//
//  cLua.hpp
//  VroomVroom
//
//  Created by Robin Zaagsma on 25/11/15.
//  Copyright © 2015 Valentinas Rimeika. All rights reserved.
//

#ifndef cLua_hpp
#define cLua_hpp

#include <stdio.h>
#include "component.h"

class CLua : public Component
{
	public:
		static const int FamilyId;
	
		CLua();
		~CLua();
	
		void SetTableKey(int);
		void Update(float);
	private:
		int tableKey;
};

#endif /* cLua_hpp */

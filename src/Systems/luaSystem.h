//
//  LuaSystem.hpp
//  VroomVroom
//
//  Created by Robin Zaagsma on 09/10/15.
//  Copyright © 2015 Valentinas Rimeika. All rights reserved.
//

#ifndef luaSystem_h
#define luaSystem_h

#include "../Utilities/typedef.h"
#include "../Patterns/singleton.h"

#define LuaSystem sLuaSystem::getInstance()

//#include <lua.hpp>
struct lua_State;

class sLuaSystem : public Singleton<sLuaSystem>
{
	public:
		sLuaSystem();
		~sLuaSystem();
		void Init();
		void Main();
		void Update(float);
		void SendReloadCallback( const string& filePath );
        void Dump(lua_State*);
    
        void Attempt(string);
        bool Call(lua_State*, int, int);
        void HandleError(lua_State*);
    
        void Resume();
        void Halt();
    
        void SetAtomPath(string);
    
        void OpenAtom(string, int = 0);

	private:
		void SetPackagePath();

		lua_State* lState;
		bool hasMainBeenCalled;
        bool allowCalls;

		static int LuaPanic(lua_State* L);
    
        string atomPath;
};

#endif /* luaSystem_h */

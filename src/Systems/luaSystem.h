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
		void EventCallback(const char*, int, int*);
        void Dump(lua_State*) const;
    
        void Attempt(string);
        bool Call(lua_State*, int, int);
        void HandleError(lua_State*);
    
        void Resume();
        void Halt();

	static void SetAtomPath(string);
    
        void OpenAtom(string, int = 0) const;
	
		lua_State* GetState() { return lState; }

	private:
		void SetPackagePath() const;

		lua_State* lState;
		bool hasMainBeenCalled;
        bool allowCalls;

		static int LuaPanic(lua_State*);
        static int LuaError(lua_State*);
    
        static string CreateStackLine(string, int, string);
        static string CreateErrorLink(string, string, int);
    
        static string atomPath;
};

#endif /* luaSystem_h */

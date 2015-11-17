//
//  uiSystem.hpp
//  VroomVroom
//
//  Created by Robin Zaagsma on 04/11/15.
//  Copyright © 2015 Valentinas Rimeika. All rights reserved.
//

#ifndef uiSystem_h
#define uiSystem_h

#include <stdio.h>

#include "../Utilities/typedef.h"
#include "../Patterns/singleton.h"
#include <map>

struct lua_State;

#define UiSystem sUiSystem::getInstance()

struct uiWindow;


struct uiWindowElement
{
    void (*handle)(uiWindowElement*);
    void (*remove)(uiWindowElement*);
    void* data;
    uiWindowElement* nextElement;
    uiWindowElement* prevElement;
    
    uiWindow* parent;
	
	std::map<string, void*> propertyMap;
};

struct uiWindow
{
    uint8 id;
	
	uiWindowElement* propertiesElement;
	
    uiWindowElement* firstElement;
    uiWindowElement* lastElement;
    uiWindow* nextWindow;
    uiWindow* prevWindow;
	
	int luaTableKey;
	
	//std::map<string, void*> propertyMap;
};

struct uiWindowPropertiesElement;
struct uiWindowTextElement;
struct uiWindowButtonElement;


class sUiSystem : public Singleton<sUiSystem>
{
    public:
        sUiSystem();
        ~sUiSystem();
    
    
        uiWindow* ConstructWindow(void);
        uiWindowTextElement* AddText(uiWindow*);
        uiWindowButtonElement* AddButton(uiWindow*);
    
        void RemoveWindow(uiWindow*);
        void RemoveElement(uiWindow*, uiWindowElement*);
    
        void Render();
	
		void SetLuaState(lua_State*);
	
		template<typename T>
			void SetNamedProperty(uiWindowElement* target, string property, T value)
			{
				std::map<string, void*>::iterator it = target->propertyMap.find(property);
				if(it != target->propertyMap.end())
				{
					T* tv = (T*)(target->propertyMap[property]);
					*tv = value;
				}
			};
	
		template<typename T>
			T GetNamedProperty(uiWindowElement* target, string property)
			{
				std::map<string, void*>::iterator it = target->propertyMap.find(property);
				if(it != target->propertyMap.end())
				{
					T* tv = (T*)(target->propertyMap[property]);
					return *tv;
				}
				return NULL;
			};
	
    private:
        void AddWindow(uiWindow*);
        void AddElement(uiWindow*, uiWindowElement*);
    
        static void HandleText(uiWindowElement*);
        static void HandleButton(uiWindowElement*);
    
        static void RemoveText(uiWindowElement*);
        static void RemoveButton(uiWindowElement*);
    
        void SetNextFreeId();
    
        uint8 firstFreeId;
        uint8 windowCount;
        uiWindow* firstWindow;
        uiWindow* lastWindow;
	
		static lua_State* lState;
};

struct uiWindowPropertiesElement
{
	string title;
	double width;
	double height;
	double x;
	double y;
	
	bool resizable;
	bool collapsable;
	bool closable;
	bool movable;
	
	uiWindowElement* parent;
};

struct uiWindowTextElement
{
	string text;
	double wrapWidth;
	
	uiWindowElement* parent;
};

struct uiWindowButtonElement
{
	string label;
	int luaTableKey;
	
	uiWindowElement* parent;
};

#endif /* uiSystem_h */

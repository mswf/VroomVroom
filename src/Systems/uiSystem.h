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

struct uiNode
{
	int luaTableKey;
	std::map<string, void*> propertyMap;
};


struct uiElement : uiNode
{
    void (*handle)(uiElement*);
    void (*remove)(uiElement*);
    void* data;
    uiElement* nextElement;
    uiElement* prevElement;
    
    uiNode* parent;
};

struct uiWindow : uiNode
{
    uint8 id;
	
	string title;
	double width;
	double height;
	double x;
	double y;
	
	bool resizable;
	bool collapsable;
	bool closable;
	bool movable;
	
    uiElement* firstElement;
    uiElement* lastElement;
    uiWindow* nextWindow;
    uiWindow* prevWindow;
};

struct uiTextElement;
struct uiButtonElement;


class sUiSystem : public Singleton<sUiSystem>
{
    public:
        sUiSystem();
        ~sUiSystem();
    
    
        uiWindow* ConstructWindow(void);
        uiTextElement* AddText(uiWindow*);
        uiButtonElement* AddButton(uiWindow*);
    
        void RemoveWindow(uiWindow*);
        void RemoveElement(uiWindow*, uiElement*);
    
        void Render();
	
		void SetLuaState(lua_State*);
	
		template<typename T>
			void SetNamedProperty(uiNode* target, string property, T value)
			{
				std::map<string, void*>::iterator it = target->propertyMap.find(property);
				if(it != target->propertyMap.end())
				{
					T* tv = (T*)(target->propertyMap[property]);
					*tv = value;
				}
			};
	
		template<typename T>
			T GetNamedProperty(uiNode* target, string property)
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
        void AddElement(uiWindow*, uiElement*);
    
        static void HandleText(uiElement*);
        static void HandleButton(uiElement*);
    
        static void RemoveText(uiElement*);
        static void RemoveButton(uiElement*);
    
        void SetNextFreeId();
    
        uint8 firstFreeId;
        uint8 windowCount;
        uiWindow* firstWindow;
        uiWindow* lastWindow;
	
		static lua_State* lState;
};

struct uiTextElement : uiElement
{
	string text;
	double wrapWidth;
};

struct uiButtonElement : uiElement
{
	string label;
	int luaTableKey;
};



#endif /* uiSystem_h */

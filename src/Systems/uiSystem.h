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

struct uiContainer;

struct uiElement
{
	int luaTableKey;
	std::map<string, void*> propertyMap;
	
    bool (*handle)(uiElement*);
    void (*remove)(uiElement*);
    uiElement* nextElement;
    uiElement* prevElement;
	
	string tooltip;
    
    uiContainer* parent;
};


struct uiContainer : uiElement
{
	uiElement* firstElement;
	uiElement* lastElement;
};

struct uiWindow : uiContainer
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
};

struct uiTextElement;
struct uiButtonElement;
struct uiTreeElement;
struct uiInputTextElement;


class sUiSystem : public Singleton<sUiSystem>
{
    public:
        sUiSystem();
        ~sUiSystem();
    
    
        uiWindow* ConstructWindow(void);
	
        uiTextElement* AddText(uiContainer*);
        uiButtonElement* AddButton(uiContainer*);
		uiTreeElement* AddTree(uiContainer*);
		uiInputTextElement* AddInputText(uiContainer*);
	
		void RemoveWindow(uiWindow*);
        void RemoveChildren(uiContainer*);
        void RemoveElement(uiContainer*, uiElement*);
    
        void Render();
	
		void SetLuaState(lua_State*);
	
		template<typename T>
			void SetNamedProperty(uiElement* target, string property, T value)
			{
				std::map<string, void*>::iterator it = target->propertyMap.find(property);
				if(it != target->propertyMap.end())
				{
					T* tv = (T*)(target->propertyMap[property]);
					*tv = value;
				}
			};
	
		template<typename T>
			T GetNamedProperty(uiElement* target, string property)
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
        void AddElement(uiContainer*, uiElement*);
	
		static void RenderContainer(uiContainer*);
	
        static bool HandleText(uiElement*);
        static bool HandleButton(uiElement*);
		static bool HandleTree(uiElement*);
		static bool HandleInputText(uiElement*);
    
        static void RemoveText(uiElement*);
        static void RemoveButton(uiElement*);
		static void RemoveTree(uiElement*);
		static void RemoveInputText(uiElement*);
	
        void SetNextFreeId();
    
        uint8 firstFreeId;
        uint8 windowCount;
        uiWindow* firstWindow;
        uiWindow* lastWindow;
	
		uiButtonElement* cachedButton;
	
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
};

struct uiTreeElement : uiContainer
{
	string label;
	
	bool opened;
};

struct uiInputTextElement : uiElement
{
	string text;
	string label;
};



#endif /* uiSystem_h */

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
#include <bitset>

#include "../Utilities/typedef.h"
#include "../Patterns/singleton.h"
#include <map>

struct lua_State;

#define UiSystem sUiSystem::getInstance()

#define WINDOW_ID_BUFFER_SIZE 1024

struct uiContainer;

struct uiElement
{
	uiElement():
		luaTableKey(-1),
		handle(NULL),
		nextElement(NULL),
		prevElement(NULL),
		tooltip(""),
		parent(NULL)
	{};
	
	int luaTableKey;
	std::map<string, void*> propertyMap;
	
    bool (*handle)(uiElement*);
    uiElement* nextElement;
    uiElement* prevElement;
	
	string tooltip;
    
    uiContainer* parent;
	
	bool visible;
};


struct uiContainer : uiElement
{
	uiContainer() :
		firstElement(NULL),
		lastElement(NULL)
	{};
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
	bool displayTitleBar;
	bool movable;
};

struct uiTextElement;
struct uiButtonElement;
struct uiTreeElement;
struct uiInputTextElement;
struct uiCheckboxElement;
struct uiSliderElement;
struct uiRegionElement;


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
		uiCheckboxElement* AddCheckbox(uiContainer*);
		uiSliderElement* AddSlider(uiContainer*);
		uiRegionElement* AddRegion(uiContainer*);
	
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
	
		uint8 GetNextFreeId();
		void FreeId(uint8);
    private:
        void AddWindow(uiWindow*);
        void AddElement(uiContainer*, uiElement*);
	
		static void RenderContainer(uiContainer*);
	
        static bool HandleText(uiElement*);
        static bool HandleButton(uiElement*);
		static bool HandleTree(uiElement*);
		static bool HandleInputText(uiElement*);
		static bool HandleCheckbox(uiElement *);
		static bool HandleSlider(uiElement *);
		static bool HandleRegion(uiElement *);
	
		template<typename T>
		static void InitBoundProperty(uiElement* e, const char* name, T* property, T value) {
			*property = value;
			e->propertyMap[name] = property;
		}
    
        uint8 idIndex;
		std::bitset<WINDOW_ID_BUFFER_SIZE> idMap;
	
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
	~uiTreeElement()
	{
		UiSystem.RemoveChildren(this);
	}
	string label;
	
	bool opened;
};

struct uiInputTextElement : uiElement
{
	string text;
	string label;
};

struct uiCheckboxElement : uiElement
{
	string label;
	bool checked;
};

struct uiSliderElement : uiElement
{
	string label;
	double minValue;
	double maxValue;
	double value;
	bool rounded;
	string format;
};

struct uiRegionElement : uiContainer
{
	~uiRegionElement()
	{
		UiSystem.RemoveChildren(this);
		UiSystem.FreeId(this->id);
	}
	int id;
	string name;
	double width;
	double height;
	bool bordered;
};


#endif /* uiSystem_h */

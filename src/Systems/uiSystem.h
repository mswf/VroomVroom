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

#define UiSystem sUiSystem::getInstance()

enum WindowElementTypes
{
    uweTEXT,
    uweBUTTON,
    uweNONE
};

struct uiWindow;

struct uiWindowElement
{
    //WindowElementTypes type;
    void (*handle)(uiWindowElement*);
    void (*remove)(uiWindowElement*);
    void* data;
    uiWindowElement* nextElement;
    uiWindowElement* prevElement;
    
    uiWindow* parent;
};

struct uiWindow
{
    uint8 id;
    string title;
    uint16 width;
    uint16 height;
    uint16 x;
    uint16 y;
    bool resizable;
    bool collapsable;
    bool closable;
    bool movable;
    
    uiWindowElement* firstElement;
    uiWindowElement* lastElement;
    uiWindow* nextWindow;
    uiWindow* prevWindow;
};


struct uiWindowTextElement
{
    string content;
    
    uiWindowElement* parent;
};

struct uiWindowButtonElement
{
    string label;
    void (*callback)();
    
    uiWindowElement* parent;
};

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
};

#endif /* uiSystem_h */

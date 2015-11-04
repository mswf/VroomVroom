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

struct uiWindowElement
{
    //WindowElementTypes type;
    void (*handle)(uiWindowElement*);
    void* data;
    uiWindowElement* nextElement;
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
    
    uiWindowElement* firstElement;
    uiWindowElement* lastElement;
    uiWindow* nextWindow;
};


struct uiWindowTextElement
{
    string content;
};

struct uiWindowButtonElement
{
    string label;
};

class sUiSystem : public Singleton<sUiSystem>
{
    public:
        sUiSystem();
        ~sUiSystem();
    
    
        uiWindow* CreateWindow();
        uiWindowTextElement* AddText(uiWindow*);
        uiWindowButtonElement* AddButton(uiWindow*);
    
        void Render();
    
    private:
        void AddWindow(uiWindow*);
        void AddElement(uiWindow*, uiWindowElement*);
    
        static void HandleText(uiWindowElement*);
        static void HandleButton(uiWindowElement*);
    
        void SetNextFreeId();
    
        uint8 firstFreeId;
        uint8 windowCount;
        uiWindow* firstWindow;
        uiWindow* lastWindow;
};

#endif /* uiSystem_h */

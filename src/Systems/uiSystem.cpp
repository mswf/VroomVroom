//
//  uiSystem.cpp
//  VroomVroom
//
//  Created by Robin Zaagsma on 04/11/15.
//  Copyright © 2015 Valentinas Rimeika. All rights reserved.
//

#include "uiSystem.h"
#include "../ImGUI/imgui.h"

sUiSystem::sUiSystem() :
    firstFreeId(0),
    windowCount(0),
    firstWindow(NULL),
    lastWindow(NULL)
{
    uiWindow* windowa = CreateWindow();
    windowa->title = "Window A";
    uiWindow* windowb = CreateWindow();
    windowb->title = "Double Name";
    uiWindow* windowc = CreateWindow();
    windowc->title = "Double Name";
    
    AddButton(windowa);
    AddText(windowa);
}

sUiSystem::~sUiSystem()
{
    //TODO
}

uiWindow* sUiSystem::CreateWindow()
{
    uint16 id = firstFreeId;
    SetNextFreeId();
    
    uiWindow* window = new uiWindow;
    window->id = id;
    window->firstElement = NULL;
    window->nextWindow = NULL;
    window->title = string("untitled window");
    window->x = 0;
    window->y = 0;
    window->width = 100;
    window->height = 100;
    
    AddWindow(window);
    
    return window;
}

uiWindowButtonElement* sUiSystem::AddButton(uiWindow* w)
{
    uiWindowElement* ee = new uiWindowElement;
    //ee->type = WindowElementTypes::uweBUTTON;
    ee->handle = HandleButton;
    
    uiWindowButtonElement* data = new uiWindowButtonElement;
    data->label = "button";
    
    ee->data = data;
    AddElement(w, ee);
    
    return data;
}

uiWindowTextElement* sUiSystem::AddText(uiWindow* w)
{
    uiWindowElement* ee = new uiWindowElement;
    //ee->type = WindowElementTypes::uweTEXT;
    ee->handle = HandleText;
    
    uiWindowTextElement* data = new uiWindowTextElement;
    data->content = "lorum ipsum";
    
    ee->data = data;
    AddElement(w, ee);
    
    return data;
}

void sUiSystem::Render()
{
    uiWindow* currentWindow = firstWindow;
    
    while(currentWindow != NULL)
    {
        string uniqueName = string(currentWindow->title);
        uniqueName += "##";
        uniqueName += std::to_string(currentWindow->id);
        
        ImGui::SetNextWindowPos(ImVec2(currentWindow->x, currentWindow->y), ImGuiSetCond_Always);
        ImGui::SetNextWindowSize(ImVec2(currentWindow->width, currentWindow->height), ImGuiSetCond_Always);
        ImGui::Begin(uniqueName.c_str());
        
        uiWindowElement* currentElement = currentWindow->firstElement;
        while(currentElement != NULL)
        {
            currentElement->handle(currentElement);
            currentElement = currentElement->nextElement;
        }
        
        //TODO check if repositioning/scaling is allowed
        ImVec2 newPos = ImGui::GetWindowPos();
        currentWindow->x = newPos.x;
        currentWindow->y = newPos.y;
        
        ImVec2 newSize = ImGui::GetWindowSize();
        currentWindow->width = newSize.x;
        currentWindow->height = newSize.y;
        
        ImGui::End();
        
        currentWindow = currentWindow->nextWindow;
    }
    ImGui::Render();
}


//PRIVATE
void sUiSystem::AddWindow(uiWindow* w)
{
    if(firstWindow == NULL)
    {
        firstWindow = w;
        lastWindow = w;
    }
    else
    {
        lastWindow->nextWindow = w;
        lastWindow = w;
    }
}

void sUiSystem::AddElement(uiWindow* w, uiWindowElement* e)
{
    e->nextElement = NULL;
    if(w->firstElement == NULL)
    {
        w->firstElement = e;
        w->lastElement = e;
    }
    else
    {
        w->lastElement->nextElement = e;
        w->lastElement = e;
    }
}

void sUiSystem::HandleButton(uiWindowElement* e)
{
    uiWindowButtonElement* data = (uiWindowButtonElement*)(e->data);
    ImGui::Button(data->label.c_str(), ImVec2(80,30));
}

void sUiSystem::HandleText(uiWindowElement* e)
{
    uiWindowTextElement* data = (uiWindowTextElement*)(e->data);
    ImGui::Text(data->content.c_str());
}

void sUiSystem::SetNextFreeId()
{
    //TODO actually find a free id :)
    firstFreeId++;
}
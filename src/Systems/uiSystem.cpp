//
//  uiSystem.cpp
//  VroomVroom
//
//  Created by Robin Zaagsma on 04/11/15.
//  Copyright © 2015 Valentinas Rimeika. All rights reserved.
//

#include "uiSystem.h"
#include "../ImGUI/imgui.h"
#include "../modules/mUiWindow.h"

lua_State* sUiSystem::lState(NULL);

sUiSystem::sUiSystem() :
	firstFreeId(0),
	windowCount(0),
	firstWindow(NULL),
	lastWindow(NULL)
{

}

sUiSystem::~sUiSystem()
{
	//TODO(robin)
}

uiWindow* sUiSystem::ConstructWindow()
{
	uint8 id = firstFreeId;
	SetNextFreeId();

	uiWindow* window = new uiWindow;
	window->id = id;
	window->firstElement = NULL;
	window->lastElement = NULL;
	window->nextWindow = NULL;
	window->prevWindow = NULL;
	
	window->luaTableKey = -1;
	
	window->title = string("untitled window");
	window->x = 0;
	window->y = 0;
	window->width = 100;
	window->height = 100;

	window->movable = true;
	window->resizable = true;
	window->collapsable = true;
	window->closable = true;
	
	window->propertyMap["title"] = &(window->title);
	window->propertyMap["x"] = &(window->x);
	window->propertyMap["y"] = &(window->y);
	window->propertyMap["width"] = &(window->width);
	window->propertyMap["height"] = &(window->height);
	
	window->propertyMap["movable"] = &(window->movable);
	window->propertyMap["resizable"] = &(window->resizable);
	window->propertyMap["collapsable"] = &(window->collapsable);
	window->propertyMap["closable"] = &(window->closable);

	AddWindow(window);

	return window;
}

uiButtonElement* sUiSystem::AddButton(uiWindow* w)
{
	uiButtonElement* bb = new uiButtonElement;
	//uiWindowElement* ee = new uiWindowElement;
	//ee->type = WindowElementTypes::uweBUTTON;
	bb->handle = HandleButton;
	bb->remove = RemoveButton;

	bb->parent = w;
	
	bb->label = "button";
	bb->luaTableKey = -1;
	
	bb->propertyMap["label"] = &(bb->label);

	AddElement(w, bb);

	return bb;
}

uiTextElement* sUiSystem::AddText(uiWindow* w)
{
	uiTextElement* tt = new uiTextElement;
	tt->handle = HandleText;
	tt->remove = RemoveText;

	tt->parent = w;
	tt->text = "lorum ipsum";
	tt->wrapWidth = 0;
	
	tt->propertyMap["text"] = &(tt->text);
	tt->propertyMap["wrapWidth"] = &(tt->wrapWidth);

	AddElement(w, tt);

	return tt;
}

void sUiSystem::RemoveWindow(uiWindow* w)
{
	uiWindow* currentWindow = lastWindow;
	while (currentWindow != NULL)
	{
		if (currentWindow == w)
		{
			if (currentWindow->prevWindow != NULL)
			{
				currentWindow->prevWindow->nextWindow = currentWindow->nextWindow;
			}
			if (currentWindow->nextWindow != NULL)
			{
				currentWindow->nextWindow->prevWindow = currentWindow->prevWindow;
			}

			if (firstWindow == currentWindow)
			{
				firstWindow = currentWindow->nextWindow;
			}
			if (lastWindow == currentWindow)
			{
				lastWindow = currentWindow->prevWindow;
			}

			//clear all the elements properly
			uiElement* currentElement = currentWindow->lastElement;
			while (currentElement != NULL)
			{
				uiElement* temp = currentElement->prevElement;
				currentElement->remove(currentElement); //clears the data
				currentElement = temp;
			}
			
			if (currentWindow->luaTableKey != -1 && lState != NULL)
			{
				mUiWindow::HandleWindowClose(lState, currentWindow->luaTableKey);
			}

			//delete the window
			delete currentWindow;

			break;
		}

		currentWindow = currentWindow->prevWindow;
	}
}

void sUiSystem::RemoveElement(uiWindow* ww, uiElement* ee)
{
	uiElement* currentElement = ww->lastElement;
	while (currentElement != NULL)
	{
		if (currentElement == ee)
		{

			if (currentElement->prevElement != NULL)
			{
				currentElement->prevElement->nextElement = currentElement->nextElement;
			}
			if (currentElement->nextElement != NULL)
			{
				currentElement->nextElement->prevElement = currentElement->prevElement;
			}
			if (ww->firstElement == currentElement)
			{
				ww->firstElement = currentElement->nextElement;
			}
			if (ww->lastElement == currentElement)
			{
				ww->lastElement = currentElement->nextElement;
			}


			currentElement->remove(currentElement); //clears the data
			delete currentElement;

			break;
		}

		currentElement = currentElement->prevElement;
	}

}

void sUiSystem::Render()
{
	uiWindow* currentWindow = firstWindow;

	while (currentWindow != NULL)
	{
		string uniqueName = string(currentWindow->title);
		uniqueName += "##";
		uniqueName += std::to_string(currentWindow->id);

		ImGui::SetNextWindowPos(ImVec2(currentWindow->x, currentWindow->y), ImGuiSetCond_Always);
		ImGui::SetNextWindowSize(ImVec2(currentWindow->width, currentWindow->height), ImGuiSetCond_Always);

		//TODO(robin) window flags and close button
		int windowFlags = 0;
		if (currentWindow->movable == false)
		{
			windowFlags |= ImGuiWindowFlags_NoMove;
		}
		if (currentWindow->resizable == false)
		{
			windowFlags |= ImGuiWindowFlags_NoResize;
		}
		if (currentWindow->collapsable == false)
		{
			windowFlags |= ImGuiWindowFlags_NoCollapse;
		}

		bool isOpened = true;   //will be set to false if user presses X button
		bool* pOpened = &isOpened;

		if (currentWindow->closable == false)
		{
			pOpened = NULL;
		}

		bool isExpanded = true;
		isExpanded = ImGui::Begin(uniqueName.c_str(), pOpened, windowFlags);

		if (isExpanded && isOpened)
		{
			uiElement* currentElement = currentWindow->firstElement;
			while (currentElement != NULL)
			{
				currentElement->handle(currentElement);
				currentElement = currentElement->nextElement;
			}
		}

		if (currentWindow->movable)
		{
			ImVec2 newPos = ImGui::GetWindowPos();
			currentWindow->x = newPos.x;
			currentWindow->y = newPos.y;
		}

		if (currentWindow->resizable && isExpanded)
		{
			ImVec2 newSize = ImGui::GetWindowSize();
			currentWindow->width = newSize.x;
			currentWindow->height = newSize.y;
		}

		ImGui::End();

		if (!isOpened)
		{
			uiWindow* temp = currentWindow->nextWindow;
			RemoveWindow(currentWindow);
			currentWindow = temp;
			continue;
		}


		currentWindow = currentWindow->nextWindow;
	}
	ImGui::Render();
}

void sUiSystem::SetLuaState(lua_State* L)
{
	lState = L;
}


//PRIVATE
void sUiSystem::AddWindow(uiWindow* w)
{
	if (firstWindow == NULL)
	{
		firstWindow = w;
		lastWindow = w;
	}
	else
	{
		w->prevWindow = lastWindow;
		lastWindow->nextWindow = w;
		lastWindow = w;
	}
}

void sUiSystem::AddElement(uiWindow* w, uiElement* e)
{
	e->parent = w;
	e->nextElement = NULL;
	e->prevElement = NULL;
	if (w->firstElement == NULL)
	{
		w->firstElement = e;
		w->lastElement = e;
	}
	else
	{
		e->prevElement = w->lastElement;
		w->lastElement->nextElement = e;
		w->lastElement = e;
	}
}

void sUiSystem::HandleButton(uiElement* e)
{
	uiButtonElement* bb = (uiButtonElement*)e;
	bool pressed = ImGui::Button(bb->label.c_str(), ImVec2(80, 30));

	if (pressed && bb->luaTableKey != -1 && lState != NULL)
	{
		mUiWindow::HandleButtonCallback(lState, bb->luaTableKey);
	}
}

void sUiSystem::HandleText(uiElement* e)
{
	uiTextElement* tt = (uiTextElement*)e;
	if(tt->wrapWidth <= 0)
	{
		ImGui::TextWrapped(tt->text.c_str());
	}
	else
	{
		ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + tt->wrapWidth);
		ImGui::Text(tt->text.c_str());
		ImGui::PopTextWrapPos();
	}
}

void sUiSystem::RemoveButton(uiElement* e)
{
	uiButtonElement* data = (uiButtonElement*)(e);
	delete data;
}

void sUiSystem::RemoveText(uiElement* e)
{
	uiTextElement* data = (uiTextElement*)(e);
	delete data;
}

void sUiSystem::SetNextFreeId()
{
	//TODO(robin) actually find a free id :)
	firstFreeId++;
}
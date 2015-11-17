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
	
	uiWindowPropertiesElement* properties = new uiWindowPropertiesElement();
	window->propertiesElement = new uiWindowElement();
	window->propertiesElement->data = properties;
	
	properties->title = string("untitled window");
	properties->x = 0;
	properties->y = 0;
	properties->width = 100;
	properties->height = 100;

	properties->movable = true;
	properties->resizable = true;
	properties->collapsable = true;
	properties->closable = true;
	
	window->propertiesElement->propertyMap["title"] = &(properties->title);
	window->propertiesElement->propertyMap["x"] = &(properties->x);
	window->propertiesElement->propertyMap["y"] = &(properties->y);
	window->propertiesElement->propertyMap["width"] = &(properties->width);
	window->propertiesElement->propertyMap["height"] = &(properties->height);
	
	window->propertiesElement->propertyMap["movable"] = &(properties->movable);
	window->propertiesElement->propertyMap["resizable"] = &(properties->resizable);
	window->propertiesElement->propertyMap["collapsable"] = &(properties->collapsable);
	window->propertiesElement->propertyMap["closable"] = &(properties->closable);

	AddWindow(window);

	return window;
}

uiWindowButtonElement* sUiSystem::AddButton(uiWindow* w)
{
	uiWindowElement* ee = new uiWindowElement;
	//ee->type = WindowElementTypes::uweBUTTON;
	ee->handle = HandleButton;
	ee->remove = RemoveButton;

	uiWindowButtonElement* data = new uiWindowButtonElement;
	data->parent = ee;
	data->label = "button";
	data->luaTableKey = -1;
	
	ee->propertyMap["label"] = &(data->label);

	ee->data = data;
	AddElement(w, ee);

	return data;
}

uiWindowTextElement* sUiSystem::AddText(uiWindow* w)
{
	uiWindowElement* ee = new uiWindowElement;
	//ee->type = WindowElementTypes::uweTEXT;
	ee->handle = HandleText;
	ee->remove = RemoveText;

	uiWindowTextElement* data = new uiWindowTextElement;
	data->parent = ee;
	data->text = "lorum ipsum";
	
	ee->propertyMap["text"] = &(data->text);


	ee->data = data;
	AddElement(w, ee);

	return data;
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
			uiWindowElement* currentElement = currentWindow->lastElement;
			while (currentElement != NULL)
			{
				uiWindowElement* temp = currentElement->prevElement;
				currentElement->remove(currentElement); //clears the data
				delete currentElement;
				currentElement = temp;
			}
			
			//remove the properties element;
			uiWindowPropertiesElement* properties = (uiWindowPropertiesElement*)currentWindow->propertiesElement->data;
			delete properties;
			delete currentWindow->propertiesElement;

			//delete the window
			delete currentWindow;

			break;
		}

		currentWindow = currentWindow->prevWindow;
	}
}

void sUiSystem::RemoveElement(uiWindow* ww, uiWindowElement* ee)
{
	uiWindowElement* currentElement = ww->lastElement;
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
		uiWindowPropertiesElement* properties = (uiWindowPropertiesElement*)currentWindow->propertiesElement->data;
		string uniqueName = string(properties->title);
		uniqueName += "##";
		uniqueName += std::to_string(currentWindow->id);

		ImGui::SetNextWindowPos(ImVec2(properties->x, properties->y), ImGuiSetCond_Always);
		ImGui::SetNextWindowSize(ImVec2(properties->width, properties->height), ImGuiSetCond_Always);

		//TODO(robin) window flags and close button
		int windowFlags = 0;
		if (properties->movable == false)
		{
			windowFlags = windowFlags | ImGuiWindowFlags_NoMove;
		}
		if (properties->resizable == false)
		{
			windowFlags = windowFlags | ImGuiWindowFlags_NoResize;
		}
		if (properties->collapsable == false)
		{
			windowFlags = windowFlags | ImGuiWindowFlags_NoCollapse;
		}

		bool isOpened = true;   //will be set to false if user presses X button
		bool* pOpened = &isOpened;

		if (properties->closable == false)
		{
			pOpened = NULL;
		}

		bool isExpanded = true;
		isExpanded = ImGui::Begin(uniqueName.c_str(), pOpened, windowFlags);

		if (isExpanded && isOpened)
		{
			uiWindowElement* currentElement = currentWindow->firstElement;
			while (currentElement != NULL)
			{
				currentElement->handle(currentElement);
				currentElement = currentElement->nextElement;
			}
		}

		if (properties->movable)
		{
			ImVec2 newPos = ImGui::GetWindowPos();
			properties->x = newPos.x;
			properties->y = newPos.y;
		}

		if (properties->resizable && isExpanded)
		{
			ImVec2 newSize = ImGui::GetWindowSize();
			properties->width = newSize.x;
			properties->height = newSize.y;
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

void sUiSystem::AddElement(uiWindow* w, uiWindowElement* e)
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

void sUiSystem::HandleButton(uiWindowElement* e)
{
	uiWindowButtonElement* data = (uiWindowButtonElement*)(e->data);
	bool pressed = ImGui::Button(data->label.c_str(), ImVec2(80, 30));

	if (pressed && data->luaTableKey != -1 && lState != NULL)
	{
		mUiWindow::HandleButtonCallback(lState, data->luaTableKey);
	}
}

void sUiSystem::HandleText(uiWindowElement* e)
{
	uiWindowTextElement* data = (uiWindowTextElement*)(e->data);
	ImGui::Text(data->text.c_str());
}

void sUiSystem::RemoveButton(uiWindowElement* e)
{
	uiWindowButtonElement* data = (uiWindowButtonElement*)(e->data);
	delete data;
}

void sUiSystem::RemoveText(uiWindowElement* e)
{
	uiWindowTextElement* data = (uiWindowTextElement*)(e->data);
	delete data;
}

void sUiSystem::SetNextFreeId()
{
	//TODO(robin) actually find a free id :)
	firstFreeId++;
}
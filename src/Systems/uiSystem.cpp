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
	lastWindow(NULL),
	cachedButton(NULL)
{
	ImGuiStyle& style = ImGui::GetStyle();
	// non-interactive rounding
	style.WindowRounding			= 2.0f;
	style.ChildWindowRounding		= 2.0f;
	style.FrameRounding				= 2.0f;

	// interactive == more rounded
	style.ScrollbarRounding			= 4.0f;
	style.GrabRounding				= 4.0f;

	style.GrabMinSize				=14.0f;


	// Colors
	style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 0.81f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.05f, 0.06f, 0.07f, 1.00f);
	style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_Border] = ImVec4(1.00f, 1.00f, 1.00f, 0.62f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.11f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.90f, 0.80f, 0.80f, 0.40f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.90f, 0.65f, 0.65f, 0.45f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.50f, 0.50f, 1.00f, 0.45f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.40f, 0.40f, 0.80f, 0.20f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.50f, 0.50f, 1.00f, 0.55f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.40f, 0.40f, 0.55f, 0.80f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.40f, 0.40f, 0.80f, 0.15f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.40f, 0.40f, 0.80f, 0.30f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.80f, 0.40f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.80f, 0.50f, 0.50f, 0.40f);
	style.Colors[ImGuiCol_ComboBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.99f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.90f, 0.90f, 0.50f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.80f, 0.50f, 0.50f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.65f, 0.39f, 0.11f, 0.87f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.60f, 0.43f, 0.21f, 1.00f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.80f, 0.62f, 0.26f, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.40f, 0.40f, 0.90f, 0.45f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.45f, 0.45f, 0.90f, 0.80f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.53f, 0.53f, 0.87f, 0.80f);
	style.Colors[ImGuiCol_Column] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.70f, 0.60f, 0.60f, 1.00f);
	style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.90f, 0.70f, 0.70f, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
	style.Colors[ImGuiCol_CloseButton] = ImVec4(0.50f, 0.50f, 0.90f, 0.50f);
	style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.70f, 0.70f, 0.90f, 0.60f);
	style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 0.00f, 1.00f, 0.35f);
	style.Colors[ImGuiCol_TooltipBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.89f);
	style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);




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
	window->nextElement = NULL;
	window->prevElement = NULL;
	
	window->luaTableKey = -1;
	
	window->handle = NULL;
	window->remove = NULL;
	window->parent = NULL;
	
	window->title = string("untitled window");
	window->x = 0;
	window->y = 0;
	window->width = 100;
	window->height = 100;

	window->movable = true;
	window->resizable = true;
	window->collapsable = true;
	window->closable = true;
	
	window->visible = true;
	
	window->propertyMap["title"] = &(window->title);
	window->propertyMap["x"] = &(window->x);
	window->propertyMap["y"] = &(window->y);
	window->propertyMap["width"] = &(window->width);
	window->propertyMap["height"] = &(window->height);
	
	window->propertyMap["movable"] = &(window->movable);
	window->propertyMap["resizable"] = &(window->resizable);
	window->propertyMap["collapsable"] = &(window->collapsable);
	window->propertyMap["closable"] = &(window->closable);
	
	window->propertyMap["visible"] = &(window->visible);

	AddWindow(window);

	return window;
}

uiButtonElement* sUiSystem::AddButton(uiContainer* w)
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

uiTextElement* sUiSystem::AddText(uiContainer* w)
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

uiTreeElement* sUiSystem::AddTree(uiContainer* w)
{
	uiTreeElement* tt = new uiTreeElement;
	tt->firstElement = NULL;
	tt->lastElement = NULL;
	
	tt->handle = HandleTree;
	tt->remove = RemoveTree;
	
	tt->parent = w;
	tt->label = "lorum ipsum";
	tt->opened = true;
	
	tt->propertyMap["label"] = &(tt->label);
	tt->propertyMap["opened"] = &(tt->opened);
	
	AddElement(w, tt);
	
	return tt;
}

uiInputTextElement* sUiSystem::AddInputText(uiContainer* w)
{
	uiInputTextElement* tt = new uiInputTextElement;
	tt->handle = HandleInputText;
	tt->remove = RemoveInputText;
	
	tt->parent = w;
	tt->text = "lorum ipsum";
	tt->label = "nerd";
	
	tt->propertyMap["text"] = &(tt->text);
	tt->propertyMap["label"] = &(tt->label);
	
	AddElement(w, tt);
	
	return tt;
}

uiCheckboxElement* sUiSystem::AddCheckbox(uiContainer* w)
{
	uiCheckboxElement* cb = new uiCheckboxElement;
	cb->handle = HandleCheckbox;
	cb->remove = RemoveCheckbox;
	
	cb->parent = w;
	cb->label = "nerd";
	cb->checked = false;
	
	cb->propertyMap["label"] = &(cb->label);
	cb->propertyMap["checked"] = &(cb->checked);
	
	AddElement(w, cb);
	
	return cb;
}

uiSliderElement* sUiSystem::AddSlider(uiContainer* w)
{
	uiSliderElement* sl = new uiSliderElement;
	sl->handle = HandleSlider;
	sl->remove = RemoveSlider;
	
	sl->parent = w;
	sl->label = "nerd";
	sl->minValue = 0;
	sl->maxValue = 100;
	sl->value = 50;
	sl->format = "%.3f";
	sl->rounded = false;
	
	sl->propertyMap["label"] = &(sl->label);
	sl->propertyMap["minValue"] = &(sl->minValue);
	sl->propertyMap["maxValue"] = &(sl->maxValue);
	sl->propertyMap["value"] = &(sl->value);
	sl->propertyMap["format"] = &(sl->format);
	sl->propertyMap["rounded"] = &(sl->rounded);
	
	AddElement(w, sl);
	
	return sl;
}

void sUiSystem::RemoveWindow(uiWindow* w)
{
	uiWindow* currentWindow = lastWindow;
	while (currentWindow != NULL)
	{
		if (currentWindow == w)
		{
			if (currentWindow->prevElement != NULL)
			{
				currentWindow->prevElement->nextElement = currentWindow->nextElement;
			}
			if (currentWindow->nextElement != NULL)
			{
				currentWindow->nextElement->prevElement = currentWindow->prevElement;
			}

			if (firstWindow == currentWindow)
			{
				firstWindow = (uiWindow*)currentWindow->nextElement;
			}
			if (lastWindow == currentWindow)
			{
				lastWindow = (uiWindow*)currentWindow->prevElement;
			}

			//clear all the elements properly
			RemoveChildren(currentWindow);
			
			if (currentWindow->luaTableKey != -1 && lState != NULL)
			{
				mUiWindow::HandleWindowClose(lState, currentWindow->luaTableKey);
			}

			//delete the window
			delete currentWindow;

			break;
		}

		currentWindow = (uiWindow*)currentWindow->prevElement;
	}
}

void sUiSystem::RemoveChildren(uiContainer* ee)
{
	//clear all the elements properly
	uiElement* currentChild = ee->lastElement;
	while (currentChild != NULL)
	{
		uiElement* temp = currentChild->prevElement;
		currentChild->remove(currentChild); //clears the data
		currentChild = temp;
	}
}

void sUiSystem::RemoveElement(uiContainer* ww, uiElement* ee)
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
		if(!currentWindow->visible)
		{
			currentWindow = (uiWindow*)currentWindow->nextElement;
			continue;
		}
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
			RenderContainer(currentWindow);
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
			uiWindow* temp = (uiWindow*)currentWindow->nextElement;
			RemoveWindow(currentWindow);
			currentWindow = temp;
			continue;
		}


		currentWindow = (uiWindow*)currentWindow->nextElement;
	}
	ImGui::Render();
	
	if(cachedButton != NULL)
	{
		if (cachedButton->luaTableKey != -1 && lState != NULL)
		{
			mUiWindow::HandleButtonCallback(lState, cachedButton->luaTableKey);
		}
		cachedButton = NULL;
	}
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
		w->prevElement = lastWindow;
		lastWindow->nextElement = w;
		lastWindow = w;
	}
}

void sUiSystem::AddElement(uiContainer* w, uiElement* e)
{
	
	e->tooltip = "";
	e->propertyMap["tooltip"] = &(e->tooltip);
	
	e->visible = true;
	e->propertyMap["visible"] = &(e->visible);
	
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

void sUiSystem::RenderContainer(uiContainer* cc)
{
	uiElement* currentElement = cc->firstElement;
	while (currentElement != NULL)
	{
		if(!currentElement->visible)
		{
			currentElement = currentElement->nextElement;
		}
		bool allowToolTip = currentElement->handle(currentElement);
		
		if(allowToolTip && currentElement->tooltip != "" && ImGui::IsItemHovered())
		{
			ImGui::SetTooltip(currentElement->tooltip.c_str());
		}
		
		currentElement = currentElement->nextElement;
	}
};

bool sUiSystem::HandleButton(uiElement* e)
{
	uiButtonElement* bb = (uiButtonElement*)e;
	bool pressed = ImGui::Button(bb->label.c_str(), ImVec2(80, 30));

	if (pressed)// && bb->luaTableKey != -1 && lState != NULL)
	{
		//mUiWindow::HandleButtonCallback(lState, bb->luaTableKey);
		//we cache this button press until all windows are rendered
		UiSystem.cachedButton = bb;
	}
	
	return true;
}

bool sUiSystem::HandleText(uiElement* e)
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
	
	return true;
}

bool sUiSystem::HandleTree(uiElement* e)
{
	uiTreeElement* tt = (uiTreeElement*)e;
	
	ImGui::SetNextTreeNodeOpened(tt->opened, ImGuiSetCond_Always);
	
	
	tt->opened = ImGui::TreeNode(tt->label.c_str());
			if(tt->tooltip != "" && ImGui::IsItemHovered())
			{
				ImGui::SetTooltip(tt->tooltip.c_str());
			}
	
	if (tt->opened)
	{
		RenderContainer(tt);
		ImGui::TreePop();
	}
	
	return false;
}

bool sUiSystem::HandleInputText(uiElement* e)
{
	//TODO scalable buffer size;
	uiInputTextElement* tt = (uiInputTextElement*)e;
	char buffer[64];
	tt->text.copy(buffer, 64);
	
	ImGui::InputText(tt->label.c_str(), buffer, 64);
	
	tt->text = string(buffer);
	
	return true;
}

bool sUiSystem::HandleCheckbox(uiElement* e)
{
	uiCheckboxElement* cb = (uiCheckboxElement*)e;
	
	ImGui::Checkbox(cb->label.c_str(), &(cb->checked));
	
	return true;
}


bool sUiSystem::HandleSlider(uiElement* e)
{
	uiSliderElement* sl = (uiSliderElement*)e;
	
	if(sl->rounded)
	{
		int valueBuffer = (int)sl->value;
		ImGui::SliderInt(sl->label.c_str(), &valueBuffer, sl->minValue, sl->maxValue, sl->format.c_str());
		sl->value = (double)valueBuffer;
	}
	else
	{
		float valueBuffer = (float)sl->value;
	 	ImGui::SliderFloat(sl->label.c_str(), &valueBuffer, sl->minValue, sl->maxValue, sl->format.c_str());
		sl->value = (double)valueBuffer;
	}
	
	return true;
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

void sUiSystem::RemoveTree(uiElement* e)
{
	uiTreeElement* data = (uiTreeElement*)(e);
	UiSystem.RemoveChildren(data);
	delete data;
}

void sUiSystem::RemoveInputText(uiElement* e)
{
	uiInputTextElement* data = (uiInputTextElement*)(e);
	delete data;
}


void sUiSystem::RemoveCheckbox(uiElement* e)
{
	uiCheckboxElement* data = (uiCheckboxElement*)(e);
	delete data;
}

void sUiSystem::RemoveSlider(uiElement* e)
{
	uiSliderElement* data = (uiSliderElement*)(e);
	delete data;
}

void sUiSystem::SetNextFreeId()
{
	//TODO(robin) actually find a free id :)
	firstFreeId++;
}
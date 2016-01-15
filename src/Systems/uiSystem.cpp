//
//  uiSystem.cpp
//  VroomVroom
//
//  Created by Robin Zaagsma on 04/11/15.
//  Copyright © 2015 Valentinas Rimeika. All rights reserved.
//

#include "uiSystem.h"
#include "../ImGUI/imgui.h"
#include "../modules/mUi.h"
#include "../console.h"

uiContainer::~uiContainer()
{
	UiSystem.RemoveChildren(this);
}


sUiSystem::sUiSystem() :
	idIndex(0),
	firstWindow(nullptr),
	lastWindow(nullptr),
	cachedButton(nullptr)
{
	ImGuiStyle& style = ImGui::GetStyle();
	// non-interactive rounding
	style.WindowRounding			= 2.0f;
	style.ChildWindowRounding		= 2.0f;
	style.FrameRounding				= 2.0f;

	// interactive == more rounded
	style.ScrollbarRounding			= 4.0f;
	style.GrabRounding				= 4.0f;

	style.GrabMinSize				= 14.0f;


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
	uint8 id = GetNextFreeId();

	uiWindow* window = new uiWindow;
	window->id = id;
	
	InitBoundProperty(window, "title", &(window->title), string("untitled window"));
	InitBoundProperty(window, "x", &(window->x), 0.0);
	InitBoundProperty(window, "y", &(window->y), 0.0);
	InitBoundProperty(window, "width", &(window->width), 100.0);
	InitBoundProperty(window, "height", &(window->height), 100.0);
	
	InitBoundProperty(window, "expanded", &(window->expanded), true);

	InitBoundProperty(window, "movable", &(window->movable), true);
	InitBoundProperty(window, "resizable", &(window->resizable), true);
	InitBoundProperty(window, "collapsable", &(window->collapsable), true);
	InitBoundProperty(window, "displayTitle", &(window->displayTitle), true);
	InitBoundProperty(window, "closable", &(window->closable), true);
	InitBoundProperty(window, "visible", &(window->visible), true);
	
	InitBoundProperty(window, "tooltip", &(window->tooltip), string(""));

	AddWindow(window);

	return window;
}

uiButtonElement* sUiSystem::AddButton(uiContainer* w)
{
	uiButtonElement* bb = new uiButtonElement;
	bb->handle = HandleButton;

	bb->parent = w;
	bb->prevHovered = false;

	InitBoundProperty(bb, "text", &(bb->text), string("button"));
	InitBoundProperty(bb, "height", &(bb->height), 0.0);


	AddElement(w, bb);

	return bb;
}

uiTextElement* sUiSystem::AddText(uiContainer* w)
{
	uiTextElement* tt = new uiTextElement;
	tt->handle = HandleText;

	tt->parent = w;

	InitBoundProperty(tt, "text", &(tt->text), string("lorum ipsum"));
	InitBoundProperty(tt, "wrapWidth", &(tt->wrapWidth), 0.0);

	AddElement(w, tt);

	return tt;
}

uiTreeElement* sUiSystem::AddTree(uiContainer* w)
{
	uiTreeElement* tt = new uiTreeElement;
	tt->firstElement = nullptr;
	tt->lastElement = nullptr;

	tt->handle = HandleTree;

	tt->parent = w;
	
	InitBoundProperty(tt, "label", &(tt->label), string("lorum ipsum"));
	InitBoundProperty(tt, "opened", &(tt->opened), true);

	AddElement(w, tt);

	return tt;
}

uiInputTextElement* sUiSystem::AddInputText(uiContainer* w)
{
	uiInputTextElement* tt = new uiInputTextElement;
	tt->handle = HandleInputText;

	tt->parent = w;
	tt->prevFocus = false;
	
	InitBoundProperty(tt, "text", &(tt->text), string("lorum ipsum"));
	InitBoundProperty(tt, "label", &(tt->label), string("nert"));

	AddElement(w, tt);

	return tt;
}

uiCheckboxElement* sUiSystem::AddCheckbox(uiContainer* w)
{
	uiCheckboxElement* cb = new uiCheckboxElement;
	cb->handle = HandleCheckbox;

	cb->parent = w;

	InitBoundProperty(cb, "label", &(cb->label), string("nert"));
	InitBoundProperty(cb, "checked", &(cb->checked), false);

	AddElement(w, cb);

	return cb;
}

uiSliderElement* sUiSystem::AddSlider(uiContainer* w)
{
	uiSliderElement* sl = new uiSliderElement;
	sl->handle = HandleSlider;

	InitBoundProperty(sl, "label", &(sl->label), string("nert"));
	InitBoundProperty(sl, "rounded", &(sl->rounded), false);
	InitBoundProperty(sl, "format", &(sl->format), string("%.2f"));
	InitBoundProperty(sl, "minValue", &(sl->minValue), 0.0);
	InitBoundProperty(sl, "maxValue", &(sl->maxValue), 100.0);
	InitBoundProperty(sl, "value", &(sl->value), 50.0);
	InitBoundProperty(sl, "power", &(sl->power), 1.0);

	AddElement(w, sl);

	return sl;
}

uiDragElement* sUiSystem::AddDrag(uiContainer* w)
{
	uiDragElement* dr = new uiDragElement;
	dr->handle = HandleDrag;
	
	InitBoundProperty(dr, "label", &(dr->label), string("nert"));
	InitBoundProperty(dr, "rounded", &(dr->rounded), false);
	InitBoundProperty(dr, "format", &(dr->format), string("%.2f"));
	InitBoundProperty(dr, "minValue", &(dr->minValue), 0.0);
	InitBoundProperty(dr, "maxValue", &(dr->maxValue), 100.0);
	InitBoundProperty(dr, "value", &(dr->value), 50.0);
	InitBoundProperty(dr, "speed", &(dr->speed), 1.0);
	InitBoundProperty(dr, "power", &(dr->power), 1.0);
	
	AddElement(w, dr);
	
	return dr;
}

uiRegionElement* sUiSystem::AddRegion(uiContainer* w)
{
	uiRegionElement* rr = new uiRegionElement;
	rr->handle = HandleRegion;
	
	InitBoundProperty(rr, "height", &(rr->height), 0.0);
	InitBoundProperty(rr, "bordered", &(rr->bordered), false);
	
	AddElement(w, rr);
	
	return rr;
}

uiHorizontalLayoutElement* sUiSystem::AddHorizontalLayout(uiContainer* w)
{
	uiHorizontalLayoutElement* hl = new uiHorizontalLayoutElement;
	hl->handle = HandleHorizontalLayout;
	
	InitBoundProperty(hl, "offset", &(hl->offset), 0.0);
	InitBoundProperty(hl, "spacing", &(hl->spacing), -1.0);


	AddElement(w, hl);
	
	return hl;
}

void sUiSystem::RemoveWindow(uiWindow* w)
{
	uiWindow* currentWindow = lastWindow;
	while (currentWindow != nullptr)
	{
		if (currentWindow == w)
		{
			if (currentWindow->prevElement != nullptr)
			{
				currentWindow->prevElement->nextElement = currentWindow->nextElement;
			}
			if (currentWindow->nextElement != nullptr)
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

			if (currentWindow->luaTableKey != -1)
			{
				HandleCallback(currentWindow, "onClose");
				mUi::UnreferenceTable(currentWindow->luaTableKey);
			}

			//delete the window
			FreeId(currentWindow->id);
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
	while (currentChild != nullptr)
	{
		uiElement* temp = currentChild->prevElement;
		RemoveElement(ee, currentChild);
		currentChild = temp;
	}
	ee->firstElement = nullptr;
	ee->lastElement = nullptr;
}

void sUiSystem::RemoveElement(uiContainer* ww, uiElement* ee)
{
	uiElement* currentElement = ww->lastElement;
	while (currentElement != nullptr)
	{
		if (currentElement == ee)
		{

			if (currentElement->prevElement != nullptr)
			{
				currentElement->prevElement->nextElement = currentElement->nextElement;
			}
			if (currentElement->nextElement != nullptr)
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

			if (currentElement->luaTableKey != -1)
			{
				mUi::UnreferenceTable(currentElement->luaTableKey);
			}
			delete currentElement;
			break;
		}

		currentElement = currentElement->prevElement;
	}

}

void sUiSystem::SetTooltip(const char* tooltip)
{
	manualTooltip = string(tooltip);
}

void sUiSystem::SetTooltip(const string& tooltip)
{
	manualTooltip = tooltip;
}

void sUiSystem::Render()
{
	uiWindow* currentWindow = firstWindow;

	while (currentWindow != nullptr)
	{
		if (!currentWindow->visible)
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
		int windowFlags = ImGuiWindowFlags_NoSavedSettings;
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
		if (currentWindow->displayTitle == false)
		{
			windowFlags |= ImGuiWindowFlags_NoTitleBar;
		}

		ImGui::SetNextWindowCollapsed(!currentWindow->expanded, ImGuiSetCond_Always);

		bool isOpened = true;   //will be set to false if user presses X button
		bool* pOpened = &isOpened;

		if (currentWindow->closable == false)
		{
			pOpened = nullptr;
		}

		bool isExpanded = true;
		isExpanded = ImGui::Begin(uniqueName.c_str(), pOpened, windowFlags);

		if (isExpanded != currentWindow->expanded )
		{
			if(isExpanded)
			{
				HandleCallback(currentWindow, "onExpand");
			}
			else
			{
				HandleCallback(currentWindow, "onCollapse");
			}
			currentWindow->expanded = isExpanded;
		}
		
		if (isExpanded && isOpened)
		{
			RenderContainer(currentWindow);
		}

		if (currentWindow->movable)
		{
			ImVec2 newPos = ImGui::GetWindowPos();
			if(newPos.x != currentWindow->x || newPos.y != currentWindow->y)
			{
				currentWindow->x = newPos.x;
				currentWindow->y = newPos.y;
				HandleCallback(currentWindow, "onMove");
			}
			
		}

		if (currentWindow->resizable && isExpanded)
		{
			ImVec2 newSize = ImGui::GetWindowSize();
			if(newSize.x != currentWindow->width || newSize.y != currentWindow->height)
			{
				currentWindow->width = newSize.x;
				currentWindow->height = newSize.y;
				HandleCallback(currentWindow, "onResize");
			}
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
	if(manualTooltip.length() > 0){
		ImGui::SetTooltip(manualTooltip.c_str());
	}
	ImGui::Render();

	if (cachedButton != nullptr)
	{
		HandleCallback(cachedButton, "onPress");
		cachedButton = nullptr;
	}
	
	mUi::ToggleFocus(ImGui::IsAnyItemActive());
	mUi::SetMouseHoveringOverAnyWindow(ImGui::IsMouseHoveringAnyWindow());
}


//PRIVATE
void sUiSystem::AddWindow(uiWindow* w)
{
	if (firstWindow == nullptr)
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
	InitBoundProperty(e, "visible", &(e->visible), true);
	InitBoundProperty(e, "tooltip", &(e->tooltip), string(""));
	InitBoundProperty(e, "width", &(e->width), 0.0);

	e->parent = w;
	e->nextElement = nullptr;
	e->prevElement = nullptr;

	e->luaTableKey = -1;

	if (w->firstElement == nullptr)
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

void sUiSystem::HandleCallback(uiElement* e, const char* func)
{
	if(e->luaTableKey != -1)
	{
		mUi::HandleCallback(e->luaTableKey, func);
	}
}

void sUiSystem::RenderContainer(uiContainer* cc)
{
	uiElement* currentElement = cc->firstElement;
	while (currentElement != nullptr)
	{
		if (!currentElement->visible)
		{
			currentElement = currentElement->nextElement;
		}
		
		//we push the pointer to the element on the ID-stack, this way even equally-named elements will have unique ids, and ImGui won't get confused!
		ImGui::PushID((long)currentElement);
		ImGui::PushItemWidth(currentElement->width);
		bool allowToolTip = currentElement->handle(currentElement);
		ImGui::PopItemWidth();
		ImGui::PopID();

		if (allowToolTip && currentElement->tooltip != "" && ImGui::IsItemHovered())
		{
			ImGui::SetTooltip(currentElement->tooltip.c_str());
		}

		currentElement = currentElement->nextElement;
	}
};

bool sUiSystem::HandleButton(uiElement* e)
{
	uiButtonElement* bb = (uiButtonElement*)e;
	bool pressed = ImGui::Button(bb->text.c_str(), ImVec2(bb->width, bb->height));

	if (pressed)
	{
		//we cache this button press until all windows are rendered
		UiSystem.cachedButton = bb;
	}
	
	bool hovered = ImGui::IsItemHovered();
	if(hovered != bb->prevHovered)
	{
		bb->prevHovered = hovered;
		if(hovered)
		{
			HandleCallback(bb, "onHoverIn");
		}
		else
		{
			HandleCallback(bb, "onHoverOut");
		}
	
	}

	return true;
}

bool sUiSystem::HandleText(uiElement* e)
{
	uiTextElement* tt = (uiTextElement*)e;
	if (tt->wrapWidth <= 0)
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

	bool opened = ImGui::TreeNode(tt->label.c_str());
	if (tt->tooltip != "" && ImGui::IsItemHovered())
	{
		ImGui::SetTooltip(tt->tooltip.c_str());
	}
	
	if(tt->opened != opened)
	{
		tt->opened = opened;
		if(opened)
		{
			HandleCallback(tt, "onExpand");
		}
		else
		{
			HandleCallback(tt, "onCollapse");
		}
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
	const int BUFFER_SIZE = 64;
	char buffer[BUFFER_SIZE];
	//fill with 0's
	std::fill(&buffer[0], &buffer[BUFFER_SIZE - 1], 0);
	tt->text.copy(buffer, BUFFER_SIZE);
	bool edited = ImGui::InputText(tt->label.c_str(), buffer, BUFFER_SIZE);
	
	if(edited)
	{
		tt->text = string(buffer);
		HandleCallback(tt, "onChange");
	}
	
	if (ImGui::IsItemActive() != tt->prevFocus)
	{
		tt->prevFocus = !tt->prevFocus;
		if(tt->prevFocus)
		{
			HandleCallback(tt, "onFocusGain");
		}
		else
		{
			HandleCallback(tt, "onFocusLose");
		}
	}

	return true;
}

bool sUiSystem::HandleCheckbox(uiElement* e)
{
	uiCheckboxElement* cb = (uiCheckboxElement*)e;
	
	bool prevChecked = cb->checked;

	ImGui::Checkbox(cb->label.c_str(), &(cb->checked));
	
	if(cb->checked != prevChecked)
	{
		HandleCallback(cb, "onChange");
	}

	return true;
}


bool sUiSystem::HandleSlider(uiElement* e)
{
	uiSliderElement* sl = (uiSliderElement*)e;

	if (sl->rounded)
	{
		int valueBuffer = (int)sl->value;
		ImGui::SliderInt(sl->label.c_str(), &valueBuffer, sl->minValue, sl->maxValue, sl->format.c_str());
		
		if(sl->value != (double)valueBuffer)
		{
			sl->value = (double)valueBuffer;
			
			HandleCallback(sl, "onChange");
		}
		
	}
	else
	{
		float valueBuffer = (float)sl->value;
		ImGui::SliderFloat(sl->label.c_str(), &valueBuffer, sl->minValue, sl->maxValue, sl->format.c_str(), sl->power);
		if (sl->value != (double)valueBuffer)
		{
			sl->value = (double)valueBuffer;

			HandleCallback(sl, "onChange");
		}
	}

	return true;
}

bool sUiSystem::HandleDrag(uiElement* e)
{
	uiDragElement* dr = (uiDragElement*)e;
	
	if (dr->rounded)
	{
		int valueBuffer = (int)dr->value;
		ImGui::DragInt(dr->label.c_str(), &valueBuffer, dr->speed, dr->minValue, dr->maxValue, dr->format.c_str());
		
		if(dr->value != (double)valueBuffer)
		{
			dr->value = (double)valueBuffer;
			
			HandleCallback(dr, "onChange");
		}
		
	}
	else
	{
		float valueBuffer = (float)dr->value;
		ImGui::DragFloat(dr->label.c_str(), &valueBuffer, dr->speed, dr->minValue, dr->maxValue, dr->format.c_str(), dr->power);
		if (dr->value != (double)valueBuffer)
		{
			dr->value = (double)valueBuffer;
			
			HandleCallback(dr, "onChange");
		}
	}
	
	return true;
}

bool sUiSystem::HandleRegion(uiElement* e)
{
	uiRegionElement* rr = (uiRegionElement*)e;
	
	ImGui::BeginChild("region", ImVec2(rr->width, rr->height), rr->bordered);
	
	RenderContainer(rr);
	
	ImGui::EndChild();
	
	return false;
}

bool sUiSystem::HandleHorizontalLayout(uiElement* e)
{
	uiHorizontalLayoutElement* hl = (uiHorizontalLayoutElement*)e;
	
	//essentially the same as RenderContainer, except we have to make an extra call in there after every element
	uiElement* currentElement = hl->firstElement;
	while (currentElement != nullptr)
	{
		if (!currentElement->visible)
		{
			currentElement = currentElement->nextElement;
		}
		
		//we push the pointer to the element on the ID-stack, this way even equally-named elements will have unique ids, and ImGui won't get confused!
		ImGui::PushID((long)currentElement);
		ImGui::PushItemWidth(currentElement->width);
		bool allowToolTip = currentElement->handle(currentElement);
		ImGui::PopItemWidth();
		ImGui::PopID();
		
		if (allowToolTip && currentElement->tooltip != "" && ImGui::IsItemHovered())
		{
			ImGui::SetTooltip(currentElement->tooltip.c_str());
		}
		
		currentElement = currentElement->nextElement;
		if(currentElement != nullptr){
			ImGui::SameLine(hl->offset, hl->spacing);
		}
	}

	
	return false;
}


//IDs
uint8 sUiSystem::GetNextFreeId()
{
	uint8 currentId= (idIndex + 1)%WINDOW_ID_BUFFER_SIZE;
	do
	{
		if(idMap[currentId] == false)
		{
			idIndex = currentId;
			idMap[currentId] = true;
			return currentId;
		}
		currentId = (currentId + 1)%WINDOW_ID_BUFFER_SIZE;
	}
	while(currentId != (idIndex + 1)%WINDOW_ID_BUFFER_SIZE);
	
	Terminal.Error("Insufficient IDs to create anymore concurrent window elements requiring an ID. If this keeps occuring, please increase the value of WINDOW_ID_BUFFER_SIZE. Be responsible ;)");
	return -1;
}

void sUiSystem::FreeId(uint8 id)
{
	if(idMap[id] == true)
	{
		idMap[id] = false;
	}
	else
	{
		Terminal.Warning("Trying to free a window ID that was not occupied");
	}
}
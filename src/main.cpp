// ImGui - standalone example application for SDL2 + OpenGL

#define GLM_FORCE_RADIANS
//default platform defines:
//#ifdef _WIN32
//#elif __APPLE__
//#elif __linux__

#include "standardIncludes.h"

#include <stdio.h>
#include <SDL2/SDL.h>
#include "content.h"

#include "Engine.h"


int main(int argc, char** a)
{
	Engine engine;
	engine.InitSDL();

	engine.InitSDL();
	engine.InitLua();
	Content::PrintPath();
	engine.runMainLua();

	//engine.StartLoop();
	engine.SomethingWindow();

	return 0;
}

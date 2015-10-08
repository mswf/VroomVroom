#pragma once
#include "renderer.h"
#include <lua.hpp>
#include <SDL2/SDL_video.h>

class Engine
{
	public:
		Engine();
		~Engine();
		void PollEvent();
		void OpenConfig();
		void runMainLua();
		void CloseWindow(SDL_Window* window, SDL_GLContext glcontext, Renderer::RenderData* data, Renderer::Camera* camera);
		void SetupWindow(SDL_Window*& window, SDL_GLContext& glcontext);
		void InitGlew();
		void ShowSimpleWindowOne(bool& show_test_window, bool& show_another_window);
		void ShowSimpleWindowTwo(bool& show_another_window);
		void ShowSimpleWindowThree(bool& show_test_window);

		void UpdateLoop();
		void InitSDL();
		void InitSDLNet();
	private:
		lua_State* luaState;
		void Update();
};


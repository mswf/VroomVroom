#pragma once
#include "renderer.h"
#include <SDL2/SDL_keyboard.h>

class Engine
{
	public:
		Engine();
		~Engine();
		void PollEvent();
		void InitLua();
		void runMainLua();
		void CloseWindow(SDL_Window* window, SDL_GLContext glcontext, Renderer::RenderData* data, Renderer::Camera* camera);
		void SetupWindow(SDL_Window*& window, SDL_GLContext& glcontext);
		void InitGlew();
		void LoadFonts();
		void ShowSimpleWindowOne(bool& show_test_window, bool& show_another_window);
		void ShowSimpleWindowTwo(bool& show_another_window);
		void ShowSimpleWindowThree(bool& show_test_window);
		void SomethingWindow();
		void Update();
		void StartLoop();
		void InitSDL();
};


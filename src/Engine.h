#pragma once
#include "renderer.h"
#include <lua.hpp>
#include <SDL2/SDL_video.h>

class Engine
{
	public:
		Engine();
		~Engine();
		
		void OpenConfig();
		void runMainLua();
		void UpdateLoop();
		void InitSDL();
    
    private:
        void Update();
        void InitGlew();
        void PollEvent();
        void ShowSimpleWindowOne(bool& show_test_window, bool& show_another_window);
        void ShowSimpleWindowTwo();
        void ShowSimpleWindowThree();
        void CloseWindow(SDL_Window* window, SDL_GLContext glcontext, Renderer::RenderData* data, Renderer::Camera* camera);
        void SetupWindow(SDL_Window*& window, SDL_GLContext& glcontext);
    private:
        lua_State* luaState;
};


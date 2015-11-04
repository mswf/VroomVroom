#ifndef engine_h
#define engine_h

#include "Systems/renderer.h"
#include <lua.hpp>
#include <SDL2/SDL_video.h>
#include "Input.hpp"
#include <map>
#include <vector>
#include "Components/entity.h"
#include "glm/mat4x4.hpp"

class Engine
{
	public:
			
		Engine();
		~Engine();
		void PollEvent();
		void OpenConfig();
		void CloseWindow(SDL_Window* window, SDL_GLContext glcontext);
		void SetupWindow(SDL_Window*& window, SDL_GLContext& glcontext);
		void InitGlew();
		void ShowSimpleWindowOne(bool& show_test_window, bool& show_another_window);
		void ShowSimpleWindowTwo();
		void ShowSimpleWindowThree();
		void UpdateLoop();
		void InitSDL();
		void InitSDLNet();
        void Init();
	
	
	
	private:
	
        //Renderer::RenderSystem* renderer;
        Input* inputManager;
		void Update();
};

#endif

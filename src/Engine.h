#ifndef engine_h
#define engine_h

#include "Systems/renderer.h"
#include <lua.hpp>
#include <SDL2/SDL_video.h>
#include "Input.hpp"
#include "Utilities/FileWatcher/file_watcher_impl.h"
#include "DataStructure/resource_manager.h"
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
		void UpdateLoop();
		void InitSDL();
		void InitSDLNet();
		void Init();



	private:
	
		void Update(float deltaTime);
		void ImportAssets();
		FW::FileWatcher* fileWatcher;
		UpdateListener* listener;
		Input* inputManager;
	
};

#endif

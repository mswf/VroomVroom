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

class Entity;

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

		//TODO(all): PLS GET THIS THE FUCK OUT OF HERE:
		static Entity* root;


	private:
	
		void SetSeed();
		void Update(float deltaTime);
		void ImportAssets();
		FW::FileWatcher* fileWatcher;
		Renderer::RenderSystem* renderer;
		UpdateListener* listener;
		Input* inputManager;
	
		unsigned int skybox_map;
	
};

#endif

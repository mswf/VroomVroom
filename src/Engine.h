#ifndef engine_h
#define engine_h

#include <map>
#include <vector>
#include <lua.hpp>
#include <SDL2/SDL_video.h>
#include <fmod.hpp>
#include <fmod_studio.hpp>
#include "Input.hpp"
#include "glm/mat4x4.hpp"
#include "Components/entity.h"
#include "DataStructure/resource_manager.h"
#include "Utilities/typedef.h"
#include "Utilities/FileWatcher/file_watcher_impl.h"
#include "Systems/renderer.h"

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
		void InitFMOD();
		void UpdateLoop();
		void InitSDL();
		void InitSDLNet();
		void Init();

		static uint32 GetTicks();
	
	private:
	
		void SetSeed();
		void Update(float deltaTime);
		void ImportAssets();
		FMOD::Studio::System* systemStudio;
		FMOD::System* systemLowLevel;
		FW::FileWatcher* fileWatcher;
		Renderer::RenderSystem* renderer;
		UpdateListener* listener;
		Input* inputManager;
	
		unsigned int skybox_map;
	
};

#endif

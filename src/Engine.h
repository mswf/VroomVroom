#ifndef engine_h
#define engine_h

#include "renderer.h"
#include <lua.hpp>
#include <SDL2/SDL_video.h>
#include "Input.hpp"
#include <map>
#include <vector>
#include "component.h"
#include "glm/mat4x4.hpp"
#include "Networking/TCPClient.h"
#include "Networking/TCPServer.h"

struct Entity
{
	Entity(std::string name = "entity_object") {};
	std::string name;
	std::map< int, Component* > entityComponents;
};

class Engine
{
	public:
		Engine();
		~Engine();
		void HostGame(char* hostName, const short port, TCPClient* client, TCPServer* server);
		void JoinGame(TCPClient* client);
		void SetUpCamera();
		void PollEvent();
		void OpenConfig();
		void CloseWindow(SDL_Window* window, SDL_GLContext glcontext, Renderer::RenderData* data, Renderer::Camera* camera);
		void SetupWindow(SDL_Window*& window, SDL_GLContext& glcontext);
		void InitGlew();
		void ShowSimpleWindowOne(bool& show_test_window, bool& show_another_window);
		void ShowSimpleWindowTwo();
		void ShowSimpleWindowThree();
		void PrintEvent(const SDL_Event * event);
		void UpdateLoop();
		void InitSDL();
		void InitSDLNet();
		void Init();
		template<typename T>
		void AddComponent( Entity* e, T* comp );
		template<typename T>
		T* GetComponent( Entity* e );
		template<typename T>
		void GetEntities( std::vector< Entity* > &result );

	private:
		std::multimap< int, Entity* > componentStorage;
		std::vector<Renderer::RenderData*> renderObjectsData;
		short myPlayerNumber;
		Renderer::RenderSystem* renderer;
		Input* inputManager;
		SDL_Window* window;
		Renderer::Camera* camera;
		SDL_GLContext glcontext;
		bool show_test_window;
		bool show_another_window;
		TCPClient* client;
		TCPServer* server;
	private:
		void Update();
		void UpdateGame();
		void CreateCube();
};

#endif

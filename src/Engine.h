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
		void HostGame(char* hostName, const short port);
		void ReceiveSyncPlayer(char* data, int& index);
		void JoinGame();
		void SendSyncPlayer(short& playerNumber, Renderer::RenderData* renderData, const TCPsocket& socket) const;
		void SendPlayerNumber(const TCPsocket& socket) const;
		void SendInitializeComplete(const TCPsocket& socket) const;
		void OnClientConnected(const TCPsocket& socket);
		void SetUpCamera();
		static int ServerLoop(void* data);
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
		void Movement();
		void HandleIncomingNetData();
		void UpdateGame();
		void CreateCube();
};

#endif

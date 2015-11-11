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

struct TransformChange
{
	glm::mat4 translation;
	glm::mat4 rotation;
	glm::mat4 scale;
};

struct Player
{
	Renderer::RenderData* renderObjectData;
	TransformChange playerData;
	glm::vec3 velocity;
	int32 timeBehind;
	uint32 lastSync;
};

class Engine
{
	public:
		Engine();
		~Engine();
		uint32 GetTimeStamp() const;
		void HostGame(char* hostName, const short port);
		void ReceiveSyncPlayer(char* data, int& index);
		void PredictPlayerPosition(Player& player);
		int32 GetServerTime() const;
		void JoinGame();
		void SendSyncPlayer(short& playerNumber, const TCPsocket& socket) const;
		void SendPlayerNumber(const TCPsocket& socket) const;
		void SendInitializeComplete(const TCPsocket& socket) const;
		void SendPlayerMatrixChange(const TCPsocket& socket) const;
		void ReceivePlayerMatrixChange(char* data, int& bufferIndex);
		void SendTimeSync(const TCPsocket& socket) const;
		void ReceiveTimeSync(char* data, int& bufferIndex);
		void SendTimeSyncResponse(const TCPsocket& socket) const;
		void ReceiveTimeSyncResponse(char* data, int& bufferIndex);
		void SendPlayerVelocityChange(const TCPsocket& socket, glm::vec3 velocity) const;
		void ReceivePlayerVelocityChange(char* data, int& bufferIndex);
		void OnClientConnected(const TCPsocket& socket) const;
		void InsertPlayerTime(short playerNumber, int32 timeBehind);
		int32 GetPlayerTime(short playernumber);
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


		std::vector<Player> players;
		//this is fucking lame
		//std::vector<Renderer::RenderData*> renderObjectsData;
		//std::vector<TransformChange> playerData;
		//std::vector<glm::vec3> playerVelocity;
		//std::vector<int32> playerTimeBehind;

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
		bool up;
		bool down;
		bool left;
		bool right;
		bool strafeLeft;
		bool strafeRight;
		uint64 updateCounter;
		bool useDeadReckoning;
		uint32 time;
		int32 timeBehind;
		const int WINDOW_WIDTH = 720;//1280;
		const int WINDOW_HEIGHT = 640; //720;
		bool isServer;
		//glm::mat4 translationChange;
		//glm::mat4 rotationChange;
		//glm::mat4 scaleChange;

	private:
		void Update();
		void PollInputStatus();
		void SendMessagesMethodType(glm::vec3 velocityChange);
		void Movement();
		void HandleIncomingNetData();
		void UpdateGame();
		void CreateCube();
		void CreatePlayerData();
};

#endif

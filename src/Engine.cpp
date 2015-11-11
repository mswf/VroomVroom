#define GLM_FORCE_RADIANS

#include "Engine.h"
#include "renderer.h"
#include "Input.hpp"
#include <SDL2/SDL.h>
#include <SDL2_net/SDL_net.h>
#include "luaSystem.h"
#include "cTransform.h"
#include "content.h"
#include "standardIncludes.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "glew.h"
#include "SDL2/SDL_opengl.h"
#include <iostream>
#include "Utilities/helperFunctions.h"
#include "Networking/NetMessage.h"



Engine::Engine() :
	myPlayerNumber(-1),
	renderer(NULL),
	inputManager(NULL),
	window(NULL),
	camera(NULL),
	glcontext(),
	show_test_window(false),
	show_another_window(false),
	client(NULL),
	server(NULL),
	up(false),
	down(false),
	left(false),
	right(false),
	strafeLeft(false),
	strafeRight(false),
	updateCounter(0),
	useDeadReckoning(false),
	time(0),
	timeBehind(0),
	isServer(false)
{
}

Engine::~Engine()
{
	//TODO: Clean up all entities and their components
	delete renderer;
	delete inputManager;
}


uint32 Engine::GetTimeStamp() const
{
	return SDL_GetTicks();
}

void Engine::HostGame(char* hostName, const short port)
{
	server = new TCPServer(port);
	server->SetConnectionAcceptedEvent(std::bind(&Engine::OnClientConnected, this, std::placeholders::_1));

	if (!client->Connect(hostName, port))
	{
		printf("bad stuff happened somehow");
		assert(false);
	}
	myPlayerNumber = 1;
	isServer = true;
}

void Engine::ReceiveSyncPlayer(char* data, int& bufferIndex)
{
	short playerNumber;
	glm::mat4 matrix;
	uint32 timeSent;
	HelperFunctions::ReadFromBuffer(data, bufferIndex, playerNumber);
	HelperFunctions::ReadFromBuffer(data, bufferIndex, timeSent);
	HelperFunctions::ReadFromBuffer(data, bufferIndex, matrix);



	while (players.size() < playerNumber)
	{
		CreateCube();
	}

	players[playerNumber - 1].lastSync = timeSent;
	players[playerNumber - 1].renderObjectData->model = matrix;

	PredictPlayerPosition(players[playerNumber - 1]);
}

void Engine::PredictPlayerPosition(Player& player)
{
	//calc how long ago it was sent
	int32 timeDiff = GetServerTime() - player.lastSync;
	int updateCount = timeDiff / 16;
	for (int i = 0; i < updateCount; ++i)
	{
		player.playerData.translation = glm::translate(players[myPlayerNumber - 1].playerData.translation, glm::vec3(0.1f, 0.0f, 0.0f));
		//players[myPlayerNumber - 1].playerData.translation = glm::translate(players[myPlayerNumber - 1].playerData.translation, glm::vec3(0.1f, 0.0f, 0.0f));

	}
}

int32 Engine::GetServerTime() const
{
	return timeBehind + SDL_GetTicks();
}
void Engine::JoinGame()
{
	bool initializing = true;
	while (initializing)
	{
		std::vector<NetworkData> messages = client->ReceiveMessage();
		for (auto it = messages.begin(); it != messages.end(); ++it)
		{
			char* data = (*it).data;
			int index = 0;
			NetMessageType messageType;
			HelperFunctions::ReadFromBuffer(data, index, messageType);

			switch (messageType)
			{
				case NetMessageType::SyncPlayer:
				{
					ReceiveSyncPlayer(data, index);

					break;
				}
				case NetMessageType::PlayerNumber:
				{
					HelperFunctions::ReadFromBuffer(data, index, myPlayerNumber);
					while (players.size() < myPlayerNumber)
					{
						CreateCube();
					}
					break;
				}
				case NetMessageType::InitializeCompleted:
				{
					if (myPlayerNumber == -1)
					{
						NetMessage m;
						m.SendNetMessage(NetMessageType::RequestMessage, NetMessageType::SyncPlayer);
						m.SendNetMessage(NetMessageType::RequestMessage, NetMessageType::PlayerNumber);
					}
					else
					{
						initializing = false;
					}
					break;
				}
				default:
				{
					//for now to see what is happening
					//SDL_assert(false);
					//ignore other messages while initializing
					break;
				}
			}

			delete[] (*it).data;
		}
		SDL_Delay(1000);
	}
}

void Engine::SendSyncPlayer(short& playerNumber, const TCPsocket& socket) const
{
	char buffer[72];
	int index;
	index = 0;
	uint32 time = timeBehind + SDL_GetTicks();

	HelperFunctions::InsertIntoBuffer(buffer, index, NetMessageType::SyncPlayer);
	HelperFunctions::InsertIntoBuffer(buffer, index, playerNumber);
	HelperFunctions::InsertIntoBuffer(buffer, index, time);
	//assert(index == 8);
	HelperFunctions::InsertIntoBuffer(buffer, index, players[playerNumber - 1].renderObjectData->model);
	//assert(index == 68);
	//assert(index == 4);
	if (socket == NULL)
	{
		//send to all
		client->SendMessageChar(buffer, index);
	}
	else
	{
		//send to specific
		server->SendData(buffer, index, socket);
	}
}

void Engine::SendPlayerNumber(const TCPsocket& socket) const
{
	char buffer[4];
	int index = 0;
	HelperFunctions::InsertIntoBuffer(buffer, index, NetMessageType::PlayerNumber);
	HelperFunctions::InsertIntoBuffer(buffer, index, (short)(players.size() + 1));
	assert(index == 4);
	server->SendData(buffer, index, socket);
}

void Engine::SendInitializeComplete(const TCPsocket& socket) const
{
	char buffer[68];
	int index = 0;
	HelperFunctions::InsertIntoBuffer(buffer, index, NetMessageType::InitializeCompleted);
	assert(index == 2);
	server->SendData(buffer, index, socket);
}

void Engine::SendPlayerMatrixChange(const TCPsocket& socket) const
{
	char buffer[196];
	int index = 0;
	HelperFunctions::InsertIntoBuffer(buffer, index, NetMessageType::PlayerMatrixChange);
	HelperFunctions::InsertIntoBuffer(buffer, index, myPlayerNumber);
	HelperFunctions::InsertIntoBuffer(buffer, index, players[myPlayerNumber - 1].playerData);
	assert(index == 196);
	if (socket == NULL)
	{
		//send to all
		client->SendMessageChar(buffer, index);
	}
	else
	{
		//send to specific
		server->SendData(buffer, index, socket);
	}
}

void Engine::SendPlayerVelocityChange(const TCPsocket& socket, glm::vec3 velocity) const
{
	char buffer[196];
	int index = 0;
	HelperFunctions::InsertIntoBuffer(buffer, index, NetMessageType::PlayerVelocityChange);
	HelperFunctions::InsertIntoBuffer(buffer, index, myPlayerNumber);
	HelperFunctions::InsertIntoBuffer(buffer, index, velocity);

	if (socket == NULL)
	{
		//send to all
		client->SendMessageChar(buffer, index);
	}
	else
	{
		//send to specific
		server->SendData(buffer, index, socket);
	}
}

void Engine::ReceivePlayerVelocityChange(char* data, int& bufferIndex)
{
	short playerNumber;
	HelperFunctions::ReadFromBuffer(data, bufferIndex, playerNumber);
	glm::vec3 velocity;
	HelperFunctions::ReadFromBuffer(data, bufferIndex, velocity);
	players[playerNumber - 1].velocity = velocity;
}

void Engine::ReceivePlayerMatrixChange(char* data, int& bufferIndex)
{
	short playerNumber;
	HelperFunctions::ReadFromBuffer(data, bufferIndex, playerNumber);
	HelperFunctions::ReadFromBuffer(data, bufferIndex, players[playerNumber - 1].playerData);
}

void Engine::SendTimeSync(const TCPsocket& socket) const
{
	uint32 time = GetTimeStamp();
	char buffer[6];
	int index = 0;
	HelperFunctions::InsertIntoBuffer(buffer, index, NetMessageType::TimeSync);
	HelperFunctions::InsertIntoBuffer(buffer, index, time);
}

void Engine::ReceiveTimeSync(char* data, int& bufferIndex)
{
	HelperFunctions::ReadFromBuffer(data, bufferIndex, time);
	timeBehind = GetTimeStamp() - time;
}

void Engine::SendTimeSyncResponse(const TCPsocket& socket) const
{
	char buffer[6];
	int index = 0;

	HelperFunctions::InsertIntoBuffer(buffer, index, NetMessageType::TimeSyncResponse);
	HelperFunctions::InsertIntoBuffer(buffer, index, myPlayerNumber);
	HelperFunctions::InsertIntoBuffer(buffer, index, timeBehind);
}

void Engine::ReceiveTimeSyncResponse(char* data, int& bufferIndex)
{
	short playerNumber;
	HelperFunctions::ReadFromBuffer(data, bufferIndex, playerNumber);
	HelperFunctions::ReadFromBuffer(data, bufferIndex, time);
	//timeBehind = GetTimeStamp() - time;
}

void Engine::OnClientConnected(const TCPsocket& socket) const
{
	printf("OnClientConnected callback\n");
	for (short i = 1; i != players.size() + 1; ++i)
	{
		SendSyncPlayer(i, socket);
	}

	SendPlayerNumber(socket);
	SDL_Delay(100);

	SendInitializeComplete(socket);
	SDL_Delay(100);

	SendTimeSync(socket);
	SDL_Delay(100);
}

void Engine::InsertPlayerTime(short playerNumber, int32 timeBehind)
{
	while (players.size() < playerNumber)
	{
		players[players.size() - 1].timeBehind = 0;
	}
	players[playerNumber].timeBehind = timeBehind;
}

int32 Engine::GetPlayerTime(short playerNumber)
{
	return players[playerNumber - 1].timeBehind;
}

void Engine::SetUpCamera()
{
	camera = new Renderer::Camera();
	float fov = 90.0f;
	float aspectRatio = 1280.0f / 720.0f;
	float zNear = 1.0f;
	float zFar = 100.0f;
	camera->eye = glm::vec3(1.0, 1.0, 1.0);
	camera->center = glm::vec3(0.0, 0.0, 0.0);
	Renderer::GetCamera(camera, Renderer::Projection::PERSPECTIVE, fov, aspectRatio, zNear, zFar);
}

void Engine::Init()
{
	renderer = new Renderer::RenderSystem();

	inputManager = new Input();
	inputManager->BindKey("shoot", SDL_SCANCODE_SPACE);

	Entity* box = new Entity();
	CTransform* boxT = new CTransform();
	AddComponent(box, boxT);

	OpenConfig();

	SetupWindow(window, glcontext);
	InitGlew();

	ImGui_ImplSdl_Init(window);
	SetUpCamera();

	//init the player/game
	char* hostName = "localhost";
	const short port = 6112;
	client = new TCPClient(hostName, port);

	if (!client->IsConnected())
	{
		HostGame(hostName, port);
		CreateCube();
	}
	else
	{
		JoinGame();
	}
}

void Engine::CreateCube()
{
	Player player;

	Renderer::RenderData* cube = new Renderer::RenderData();
	player.renderObjectData = cube;
	player.velocity = glm::vec3(0);
	players.push_back(player);

	Renderer::GetRenderData(cube);
	if (players.size() == myPlayerNumber)
	{
		SendSyncPlayer(myPlayerNumber, NULL);
	}
	CreatePlayerData();
}

void Engine::CreatePlayerData()
{
	TransformChange data;
	data.rotation = glm::mat4(1);
	data.translation = glm::mat4(1);
	data.scale = glm::mat4(1);
	players[players.size() - 1].playerData = data;
}

void Engine::PollEvent()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		//PrintEvent(&event);
		ImGui_ImplSdl_ProcessEvent(&event);
		if (event.type == SDL_QUIT)
		{
			exit(EXIT_SUCCESS);
		}
		inputManager->Update(&event);

	}
	inputManager->StateReset();
}

void Engine::OpenConfig()
{
	lua_State* L = luaL_newstate();

	string configPath = SDL_GetBasePath() + string("config.lua");
	if (luaL_loadfile(L, configPath.c_str()))
	{
		//TODO error handling
		std::cout << "Could not open config.lua - The program will not run correctly" << std::endl;
		return;
	}

	lua_pcall(L, 0, 0, 0);

	lua_getglobal(L, "CONTENT_PATH");
	if (lua_type(L, -1) != LUA_TSTRING)
	{
		std::cout << "Problem in config.lua - CONTENT_PATH is not set to a valid string" << std::endl;
	}
	size_t len_cp;
	const char* contentPath = lua_tolstring(L, -1, &len_cp);

	Content::SetPath(contentPath, len_cp);

	lua_close(L);
}

void Engine::CloseWindow(SDL_Window* window, SDL_GLContext glcontext, Renderer::RenderData* data, Renderer::Camera* camera)
{
	Renderer::DeleteData(data);
	Renderer::DeleteCamera(camera);
	ImGui_ImplSdl_Shutdown();
	SDL_GL_DeleteContext(glcontext);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Engine::SetupWindow(SDL_Window*& window, SDL_GLContext& glcontext)
{
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_DisplayMode current;
	SDL_GetCurrentDisplayMode(0, &current);
	window = SDL_CreateWindow("VroomVroom", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	glcontext = SDL_GL_CreateContext(window);
}

// TODO: Move to renderer
void Engine::InitGlew()
{
	glewExperimental = true;

	GLenum glewError = glewInit();
	if (glewError != GLEW_OK)
	{
		printf("Error initializing GLEW! %p\n", glewGetErrorString(glewError));
	}
#ifdef DEBUG
	std::cout << "GL version " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLEW version " << glewGetString(GLEW_VERSION_MAJOR) << "." << glewGetString(GLEW_VERSION_MINOR) << std::endl;
#endif
}

void Engine::ShowSimpleWindowOne(bool& show_test_window, bool& show_another_window)
{
	ImVec4 clear_color = ImColor(114, 144, 154);
	static float f = 0.0f;
	ImGui::Text("Hello, world!");
	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
	ImGui::ColorEdit3("clear color", (float*)&clear_color);
	if (ImGui::Button("Test Window"))
	{
		show_test_window ^= 1;
	}
	if (ImGui::Button("Another Window"))
	{
		show_another_window ^= 1;
	}
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}

void Engine::ShowSimpleWindowTwo()
{
	ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_FirstUseEver);
	// Send bool as reference
	ImGui::Begin("Another Window");
	ImGui::Text("Hello");
	ImGui::End();
}

void Engine::ShowSimpleWindowThree()
{
	ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
	// Send bool as reference
	ImGui::ShowTestWindow();
}


void Engine::Update()
{
	//TODO no fake deltatime :)
	//int dt = 16;
	//LuaSystem.Update(dt);

	//TODO this should be refactored out at some point
	//It is neccesary now to poll network events from the socket
	Terminal.Update();
}

void Engine::PollInputStatus()
{
	if (inputManager->OnKeyDown(SDL_SCANCODE_W))
	{
		up = true;
	}
	if (inputManager->OnKeyUp(SDL_SCANCODE_W))
	{
		up = false;
	}
	if (inputManager->OnKeyDown(SDL_SCANCODE_S))
	{
		down = true;
	}
	if (inputManager->OnKeyUp(SDL_SCANCODE_S))
	{
		down = false;
	}

	if (inputManager->OnKeyDown(SDL_SCANCODE_A))
	{
		left = true;
	}
	if (inputManager->OnKeyUp(SDL_SCANCODE_A))
	{
		left = false;
	}
	if (inputManager->OnKeyDown(SDL_SCANCODE_D))
	{
		right = true;
	}
	if (inputManager->OnKeyUp(SDL_SCANCODE_D))
	{
		right = false;
	}
	if (inputManager->OnKeyDown(SDL_SCANCODE_Q))
	{
		strafeLeft = true;
	}
	if (inputManager->OnKeyUp(SDL_SCANCODE_Q))
	{
		strafeLeft = false;
	}
	if (inputManager->OnKeyDown(SDL_SCANCODE_E))
	{
		strafeRight = true;
	}
	if (inputManager->OnKeyUp(SDL_SCANCODE_E))
	{
		strafeRight = false;
	}
	if (inputManager->OnKeyUp(SDL_SCANCODE_F))
	{
		useDeadReckoning = !useDeadReckoning;
		if (useDeadReckoning)
		{
			printf("Using DR\n");
		}
		else
		{
			printf("Not using DR\n");
		}
	}
}

void Engine::SendMessagesMethodType(glm::vec3 velocityChange)
{
	//dead reckoning
	if (useDeadReckoning)
	{
		if (updateCounter % 100 == 0)
		{
			//SendSyncPlayer(myPlayerNumber, NULL);
		}
		//SendPlayerMatrixChange(NULL);
		SendPlayerVelocityChange(NULL, velocityChange);
	}
	else
	{
		SendSyncPlayer(myPlayerNumber, NULL);
	}
}

void Engine::Movement()
{
	players[myPlayerNumber - 1].playerData.translation = glm::mat4(1);
	players[myPlayerNumber - 1].playerData.rotation = glm::mat4(1);
	players[myPlayerNumber - 1].playerData.scale = glm::mat4(1);
	glm::vec3 velocityChange(0, 0, 0);

	if (up)
	{
		players[myPlayerNumber - 1].playerData.translation = glm::translate(players[myPlayerNumber - 1].playerData.translation, glm::vec3(0.1f, 0.0f, 0.0f));
		velocityChange.x = 0.1f;
	}
	else if (down)
	{
		players[myPlayerNumber - 1].playerData.translation = glm::translate(players[myPlayerNumber - 1].playerData.translation, glm::vec3(-0.1f, 0.0f, 0.0f));
		velocityChange.x = -0.1f;
	}
	if (left)
	{
		players[myPlayerNumber - 1].playerData.rotation = glm::rotate(players[myPlayerNumber - 1].playerData.rotation, glm::radians(1.0f), glm::vec3(0.0f, 0.1f, 0.0f));
		velocityChange.y = 0.1f;
	}
	else if (right)
	{
		players[myPlayerNumber - 1].playerData.rotation = glm::rotate(players[myPlayerNumber - 1].playerData.rotation, glm::radians(1.0f), glm::vec3(0.0f, -0.1f, 0.0f));
		velocityChange.y = -0.1f;
	}

	SendMessagesMethodType(velocityChange);

	for (int i = 0; i < players.size(); ++i)
	{
		players[i].renderObjectData->model *= players[myPlayerNumber - 1].playerData.scale * players[myPlayerNumber - 1].playerData.rotation * players[myPlayerNumber - 1].playerData.translation;
	}
}

void Engine::HandleIncomingNetData()
{
	std::vector<NetworkData> messages = client->ReceiveMessage();
	for (auto it = messages.begin(); it != messages.end(); ++it)
	{
		char* data = (*it).data;
		int index = 0;
		NetMessageType messageType = NetMessageType::None;
		HelperFunctions::ReadFromBuffer(data, index, messageType);
		switch (messageType)
		{
			case NetMessageType::SyncPlayer:
			{
				ReceiveSyncPlayer(data, index);
				break;
			}
			case NetMessageType::PlayerMatrixChange:
			{
				ReceivePlayerMatrixChange(data, index);
				break;
			}
			case NetMessageType::TimeSync:
			{
				ReceiveTimeSync(data, index);
				break;
			}
			case NetMessageType::TimeSyncResponse:
			{
				ReceiveTimeSyncResponse(data, index);
				break;
			}
			case NetMessageType::PlayerVelocityChange:
			{
				ReceivePlayerVelocityChange(data, index);
				break;
			}
			case NetMessageType::SyncNpc:
			case NetMessageType::SyncVelocity:
			case NetMessageType::RequestMessage:
			case NetMessageType::SyncPosition:
			case NetMessageType::SyncRotation:
			{
				//not yet implemented
				SDL_assert(false);
				break;
			}
			case NetMessageType::InitializeCompleted:
			case NetMessageType::PlayerNumber:
			case NetMessageType::None:
			{
				//do nothing
				break;
			}
			default:
			{
				//all should be handled I guess?
				//assert(false);
				break;
			}
		}
	}
}

void Engine::UpdateGame()
{
	updateCounter++;
	HandleIncomingNetData();

	if (server != NULL)
	{
		server->AcceptConnections();
	}

	PollInputStatus();
	Movement();
}

void Engine::UpdateLoop()
{
	const float millisecondModifier = 1000.0f;
	const float gameFPS = 60.0f;
	const float gameUpdateInterval = 1 / gameFPS * millisecondModifier;
	uint32 currentTicks = SDL_GetTicks();
	uint32 prevTicks = currentTicks;
	bool running = true;

	//LuaSystem.Main();

	while (running)
	{
		//multithreaded rendering goes here if we decide to do it
		/*if (threadedDrawingBusy)
		continue*/

		//game
		currentTicks = SDL_GetTicks();
		uint32 deltaTimeGame = currentTicks - prevTicks;
		//inputManager->MidiListener();

		while (deltaTimeGame > gameUpdateInterval)
		{
			Update();



			deltaTimeGame -= gameUpdateInterval;

			if (deltaTimeGame < gameUpdateInterval)
			{
				PollEvent();
				UpdateGame();
				prevTicks = currentTicks;
				prevTicks -= deltaTimeGame;
			}
		}

		ImGui_ImplSdl_NewFrame(window);


		//moved clear for weikies hack
		glClearColor( 0.2, 0.2, 0.2, 1.0 );
		glClear(GL_COLOR_BUFFER_BIT); //GL_DEPTH_BUFFER_BIT

		for (int i = 0; i < players.size(); ++i)
		{
			Renderer::Render(SDL_GetTicks(), camera, players[i].renderObjectData);
		}

		ShowSimpleWindowOne(show_test_window, show_another_window);

		if (show_another_window)
		{
			ShowSimpleWindowTwo();
		}
		if (show_test_window)
		{
			ShowSimpleWindowThree();
		}

		ImGui::Render();
		SDL_GL_SwapWindow(window);

		//rendering
		//float normalizedInterpolationValue = deltaTimeGame / gameUpdateInterval
		//	//Do something with locking
		//	render.draw(normalizedInterpolationValue)
	}

	for (int i = 0; i < players.size(); ++i)
	{
		CloseWindow(window, glcontext, players[i].renderObjectData, camera);
	}
}

void Engine::InitSDL()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		printf("Error: %s\n", SDL_GetError());
		assert(false);
	}
}

void Engine::InitSDLNet()
{
	if (SDLNet_Init() != 0)
	{
		printf("Error: %s\n", SDLNet_GetError());
		assert(false);
	}
}

template<typename T>
void Engine::AddComponent( Entity* e, T* comp )
{
	componentStorage.insert( std::pair< int, Entity* >( T::familyId, e ) );
	e->entityComponents.insert( std::pair< int, Component* >( T::familyId, comp ) );
}

template<typename T>
T* Engine::GetComponent( Entity* e )
{
	return (T*)e->entityComponents[T::familyId];

}

template<typename T>
void Engine::GetEntities( std::vector< Entity* > &result )
{
	auto iterPair = componentStorage.equal_range( T::familyId );
	for ( auto iter = iterPair.first; iter != iterPair.second; ++iter )
	{
		result.push_back( iter->second );
	}
}

void Engine::PrintEvent(const SDL_Event * event)
{
	if (event->type == SDL_WINDOWEVENT)
	{
		switch (event->window.event)
		{
			case SDL_WINDOWEVENT_SHOWN:
				SDL_Log("Window %d shown", event->window.windowID);
				break;
			case SDL_WINDOWEVENT_HIDDEN:
				SDL_Log("Window %d hidden", event->window.windowID);
				break;
			case SDL_WINDOWEVENT_EXPOSED:
				SDL_Log("Window %d exposed", event->window.windowID);
				break;
			case SDL_WINDOWEVENT_MOVED:
				SDL_Log("Window %d moved to %d,%d",
						event->window.windowID, event->window.data1,
						event->window.data2);
				break;
			case SDL_WINDOWEVENT_RESIZED:
				SDL_Log("Window %d resized to %dx%d",
						event->window.windowID, event->window.data1,
						event->window.data2);
				break;
			case SDL_WINDOWEVENT_SIZE_CHANGED:
				SDL_Log("Window %d size changed to %dx%d",
						event->window.windowID, event->window.data1,
						event->window.data2);
				break;
			case SDL_WINDOWEVENT_MINIMIZED:
				SDL_Log("Window %d minimized", event->window.windowID);
				break;
			case SDL_WINDOWEVENT_MAXIMIZED:
				SDL_Log("Window %d maximized", event->window.windowID);
				break;
			case SDL_WINDOWEVENT_RESTORED:
				SDL_Log("Window %d restored", event->window.windowID);
				break;
			case SDL_WINDOWEVENT_ENTER:
				SDL_Log("Mouse entered window %d",
						event->window.windowID);
				break;
			case SDL_WINDOWEVENT_LEAVE:
				SDL_Log("Mouse left window %d", event->window.windowID);
				break;
			case SDL_WINDOWEVENT_FOCUS_GAINED:
				SDL_Log("Window %d gained keyboard focus",
						event->window.windowID);
				break;
			case SDL_WINDOWEVENT_FOCUS_LOST:
				SDL_Log("Window %d lost keyboard focus",
						event->window.windowID);
				break;
			case SDL_WINDOWEVENT_CLOSE:
				SDL_Log("Window %d closed", event->window.windowID);
				break;
			default:
				SDL_Log("Window %d got unknown event %d",
						event->window.windowID, event->window.event);
				break;
		}
	}
}


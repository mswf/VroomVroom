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

Engine::Engine() :
	cube1(NULL),
	cube2(NULL),
	renderer(NULL),
	inputManager(NULL)
{
}

Engine::~Engine()
{
	//TODO: Clean up all entities and their components
	delete renderer;
	delete inputManager;
}

void Engine::Init()
{
	renderer = new Renderer::RenderSystem();

	inputManager = new Input();
	inputManager->BindKey("shoot", SDL_SCANCODE_SPACE);

	Entity* box = new Entity();
	CTransform* boxT = new CTransform();
	AddComponent(box, boxT);
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
	window = SDL_CreateWindow("VroomVroom", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
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
	int dt = 16;
	//LuaSystem.Update(dt);

	//TODO this should be refactored out at some point
	//It is neccesary now to poll network events from the socket
	Terminal.Update();
}

void Engine::UpdateGame()
{
	glm::mat4 translation	= glm::mat4(1);
	glm::mat4 rotation		= glm::mat4(1);
	glm::mat4 scale			= glm::mat4(1);
	glm::mat4 translation2	= glm::mat4(1);
	glm::mat4 rotation2		= glm::mat4(1);
	glm::mat4 scale2		= glm::mat4(1);

	if (inputManager->OnKey(SDL_SCANCODE_W))
	{
		translation = glm::translate(translation, glm::vec3(0.1f, 0.0f, 0.0f));
	}
	if (inputManager->OnKey(SDL_SCANCODE_S))
	{
		translation = glm::translate(translation, glm::vec3(-0.1f, 0.0f, 0.0f));
	}
	if (inputManager->OnKey(SDL_SCANCODE_A))
	{
		rotation = glm::rotate(rotation, glm::radians(1.0f), glm::vec3(0.0f, 0.1f, 0.0f));
	}
	if (inputManager->OnKey(SDL_SCANCODE_D))
	{
		rotation = glm::rotate(rotation, glm::radians(-1.0f), glm::vec3(0.0f, 0.1f, 0.0f));
	}
	cube1->model *= scale * rotation * translation;

	//2
	if (inputManager->OnKey(SDL_SCANCODE_UP))
	{
		translation2 = glm::translate(translation, glm::vec3(0.1f, 0.0f, 0.0f));
	}
	if (inputManager->OnKey(SDL_SCANCODE_DOWN))
	{
		translation2 = glm::translate(translation, glm::vec3(-0.1f, 0.0f, 0.0f));
	}
	if (inputManager->OnKey(SDL_SCANCODE_LEFT))
	{
		rotation2 = glm::rotate(rotation, glm::radians(1.0f), glm::vec3(0.0f, 0.1f, 0.0f));
	}
	if (inputManager->OnKey(SDL_SCANCODE_RIGHT))
	{
		rotation2 = glm::rotate(rotation, glm::radians(-1.0f), glm::vec3(0.0f, 0.1f, 0.0f));
	}
	cube2->model *= scale2 * rotation2 * translation2;

	//if ( inputManager->OnKeyDown(SDL_SCANCODE_S) )
	//{
	//	scale = glm::scale(scale, glm::vec3(1.1f, 1.1f, 1.1f));
	//}
	//if ( inputManager->OnKeyDown(SDL_SCANCODE_D) )
	//{
	//	scale = glm::scale(scale, glm::vec3(0.9f, 0.9f, 0.9f));
	//}

	if ( inputManager->OnMouseDown(SDL_BUTTON_LEFT) )
	{
		printf( "Pressed left mouse button at x:%i y:%i \n", inputManager->GetMousePosition().x, inputManager->GetMousePosition().y);
	}
	if ( inputManager->OnMouseUp(SDL_BUTTON_LEFT) )
	{
		printf( "Released left mouse button \n" );
	}

	if ( inputManager->OnKeyDown("shoot") )
	{
		printf( "Bam! Key: %i \n", inputManager->GetBind("shoot") );
	}
}

void Engine::UpdateLoop()
{

	SDL_Window* window;
	SDL_GLContext glcontext;
	SetupWindow(window, glcontext);
	InitGlew();

	ImGui_ImplSdl_Init(window);

	bool show_test_window = true;
	bool show_another_window = false;


	cube1 = new Renderer::RenderData();
	cube2 = new Renderer::RenderData();
	Renderer::Camera * camera = new Renderer::Camera();

	float fov = 90.0f;
	float aspectRatio = 1280.0f / 720.0f;
	float zNear = 1.0f;
	float zFar = 100.0f;
	camera->eye = glm::vec3(1.0, 1.0, 1.0);
	camera->center = glm::vec3(0.0, 0.0, 0.0);
	Renderer::GetCamera(camera, Renderer::Projection::PERSPECTIVE, fov, aspectRatio, zNear, zFar);
	Renderer::GetRenderData(cube1);
	Renderer::GetRenderData(cube2);


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
		PollEvent();
		//inputManager->MidiListener();

		while (deltaTimeGame > gameUpdateInterval)
		{
			Update();



			deltaTimeGame -= gameUpdateInterval;

			if (deltaTimeGame < gameUpdateInterval)
			{
				UpdateGame();
				prevTicks = currentTicks;
				prevTicks -= deltaTimeGame;
			}
		}

		ImGui_ImplSdl_NewFrame(window);


		//moved clear for weikies hack
		glClearColor( 0.2, 0.2, 0.2, 1.0 );
		glClear(GL_COLOR_BUFFER_BIT); //GL_DEPTH_BUFFER_BIT
		Renderer::Render(SDL_GetTicks(), camera, cube1);
		Renderer::Render(SDL_GetTicks(), camera, cube2);

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

	CloseWindow(window, glcontext, cube1, camera);
	CloseWindow(window, glcontext, cube2, camera);
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


#define GLM_FORCE_RADIANS

#include "Engine.h"
#include "renderer.h"
#include "Input.hpp"
#include <SDL2/SDL.h>
#include <SDL2_net/SDL_net.h>
#include "luaSystem.h"
#include "Modules/mInput.h"
#include "cTransform.h"
#include "content.h"
#include "standardIncludes.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "glew.h"
#include "SDL2/SDL_opengl.h"
#include <iostream>
#include <cstring>
#include "Utilities/helperFunctions.h"
#include "Clock.hpp"

Engine::Engine() :
	//renderer(NULL),
	inputManager(NULL)
{
}

Engine::~Engine()
{
	//TODO: Clean up all entities and their components
	//delete renderer;
	delete inputManager;
}

void Engine::Init()
{
    //renderer = new Renderer::RenderSystem();
    
    inputManager = new Input();
    //inputManager->BindKey("shoot", SDL_SCANCODE_SPACE);
    
    //TODO: I don't really want to bind this here, but I also don't want to pass inputManager all over the place
    //Does it have to be a singular instance contained in Engine?
    mInput::SetInput(inputManager);
    
    Entity* box = new Entity();
    CTransform* boxT= new CTransform();
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
            //TODO: don't exit instantly, rather disrupt the game loop and exit through a controlled flow
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
        Terminal.Error("Could not open config.lua - The program will not run correctly");
		return;
	}

	lua_pcall(L, 0, 0, 0);

	lua_getglobal(L, "CONTENT_PATH");
	if (lua_type(L, -1) != LUA_TSTRING)
	{
        Terminal.Warning("Problem in config.lua - CONTENT_PATH is not set to a valid string");
	}
	size_t len_cp;
	const char* contentPath = lua_tolstring(L, -1, &len_cp);

	Content::SetPath(contentPath, len_cp);

	lua_close(L);
}

void Engine::CloseWindow(SDL_Window* window, SDL_GLContext glcontext, Renderer::RenderData* data )
{
	Renderer::DeleteData(data);
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
	LuaSystem.Update(dt);

	//TODO this should be refactored out at some point
	//It is neccesary now to poll network events from the socket
	Terminal.Update();
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


    
	Renderer::RenderData* data = new Renderer::RenderData();
	float fov = 90.0f;
	float aspectRatio = 1280.0f / 720.0f;
	float zNear = 0.2f;
	float zFar = 10000.0f;
	data->camera = new CCamera( Projection::PERSPECTIVE, fov, aspectRatio, zNear, zFar );
	Renderer::GetRenderData(data, myMesh);

	const float millisecondModifier = 1000.0f;
	const float gameFPS = 60.0f;
	const float gameUpdateInterval = 1 / gameFPS * millisecondModifier;
    uint32 currentTicks = SDL_GetTicks();
	uint32 prevTicks = currentTicks;
	bool running = true;

	LuaSystem.Main();
    
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
			
			data->transform.SetPosition( glm::vec3(-500.0f, 0.0f, 0.0f ) );
			//data->transform.SetRotation( glm::vec3( 45.0f, 45.0f, 0.0f ) );
			data->transform.Rotate( glm::vec3( 45.0f, -45.0f, 0.0f ) );
			
			//data->transform.Pitch(1.0f);
			//data->transform.Roll(1.0f);
			//data->transform.SetPitch(45.0f);
			//data->transform.SetYaw(45.0f);
			//data->transform.SetRoll(0.0f);
			
			deltaTimeGame -= gameUpdateInterval;

			if (deltaTimeGame < gameUpdateInterval)
			{
                /*
                if ( inputManager->OnKeyDown(SDL_SCANCODE_UP) )
                {
                    translation = glm::translate(translation, glm::vec3(0.1f, 0.0f, 0.0f));
                }
                
                if ( inputManager->OnKeyDown(SDL_SCANCODE_DOWN) )
                {
                    translation = glm::translate(translation, glm::vec3(-0.1f, 0.0f, 0.0f));
                }
                
                if ( inputManager->OnKeyDown(SDL_SCANCODE_LEFT) )
                {
                    rotation = glm::rotate(rotation, glm::radians(1.0f), glm::vec3(0.0f, 0.1f, 0.0f));
                }
                
                if ( inputManager->OnKeyDown(SDL_SCANCODE_RIGHT) )
                {
                    rotation = glm::rotate(rotation, glm::radians(-1.0f), glm::vec3(0.0f, 0.1f, 0.0f));
                }
                
                if ( inputManager->OnKeyDown(SDL_SCANCODE_S) )
                {
                    scale = glm::scale(scale, glm::vec3(1.1f, 1.1f, 1.1f));
                }
                if ( inputManager->OnKeyDown(SDL_SCANCODE_D) )
                {
                    scale = glm::scale(scale, glm::vec3(0.9f, 0.9f, 0.9f));
                }
 
				if ( inputManager->OnKeyDown(SDL_SCANCODE_1) )
				{
					printf( "Pressed key %i \n", SDL_SCANCODE_1 );
				}
				if ( inputManager->OnKeyUp(SDL_SCANCODE_1) )
				{
					printf( "Released key %i \n", SDL_SCANCODE_1 );
				}
				if ( inputManager->OnKey(SDL_SCANCODE_2) )
				{
					printf( "Holding key %i \n", SDL_SCANCODE_2 );
				}

				if ( inputManager->OnMouseDown(SDL_BUTTON_LEFT) )
				{
					printf( "Pressed left mouse button at x:%i y:%i \n", inputManager->GetMousePosition().x, inputManager->GetMousePosition().y);
				}
				if ( inputManager->OnMouseUp(SDL_BUTTON_LEFT) )
				{
					printf( "Released left mouse button \n" );
				}
  */
				prevTicks = currentTicks;
				prevTicks -= deltaTimeGame;
			}
		}

		ImGui_ImplSdl_NewFrame(window);

		Renderer::Render(SDL_GetTicks(), camera, data);

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

	CloseWindow(window, glcontext, data);
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

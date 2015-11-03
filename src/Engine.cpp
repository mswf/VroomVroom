#define GLM_FORCE_RADIANS

#include "Engine.h"
#include "Systems/renderer.h"
#include "Input.hpp"
#include <SDL2/SDL.h>
#include <SDL2_net/SDL_net.h>
#include "Systems/luaSystem.h"
#include "Modules/mInput.h"
#include "Components/cTransform.h"
#include "Components/cMaterial.h"
#include "Components/cCamera.h"
#include "content.h"
#include "Utilities/standardIncludes.h"
#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_sdl.h"
#include "glew.h"
#include "SDL2/SDL_opengl.h"
#include <iostream>
#include <cstring>
#include "Utilities/helperFunctions.h"

Engine::Engine() :
	inputManager(NULL)
{
}

Engine::~Engine()
{
	//TODO: Clean up all entities and their components
	delete inputManager;
}

void Engine::Init()
{
    inputManager = new Input();
    //inputManager->BindKey("shoot", SDL_SCANCODE_SPACE);
    
    //TODO: I don't really want to bind this here, but I also don't want to pass inputManager all over the place
    //Does it have to be a singular instance contained in Engine?
    mInput::SetInput(inputManager);
    
    OpenConfig();
    LuaSystem.Init();
}

void Engine::PollEvent()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
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

void Engine::CloseWindow(SDL_Window* window, SDL_GLContext glcontext )
{
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
	bool show_another_window = true;


	
	CMesh* mesh = new CMesh();
	CMaterial* mat = new CMaterial( new Shader() );
	CCamera* cam = new CCamera( Projection::PERSPECTIVE, 90.0f, 1280.0f / 720.0f, 0.2f, 1000.0f );
	cam->SetEyeVector( glm::vec3(5.0f, 5.0f, 5.0f) );
	
	
	Entity* box = new Entity();
	CTransform* t = new CTransform();
	Entity::AddComponent(box, t );
	Entity::AddComponent(box, mesh);
	Entity::AddComponent(box, mat);
	Renderer::GenerateTriangle(box);
	//Renderer::GenerateCube(box);
	
	Entity* camera = new Entity();
	Entity::AddComponent(camera, new CTransform() );
	Entity::AddComponent(camera, cam);
	
	

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
			
			deltaTimeGame -= gameUpdateInterval;

			if (deltaTimeGame < gameUpdateInterval)
			{
				//t->Roll(10.0f);
				t->Rotate( glm::vec3(10.0f, 0.0f, 10.0f) );
		
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

		ShowSimpleWindowOne(show_test_window, show_another_window);

		if (show_another_window)
		{
			ShowSimpleWindowTwo();
		}
		if (show_test_window)
		{
			ShowSimpleWindowThree();
		}
		
		glClearColor( 0.2, 0.2, 0.2, 1.0 );
		glClear(GL_COLOR_BUFFER_BIT);
		
		Renderer::Render( SDL_GetTicks(), camera, box );
		ImGui::Render();
		SDL_GL_SwapWindow(window);

		//rendering
		//float normalizedInterpolationValue = deltaTimeGame / gameUpdateInterval
		//	//Do something with locking
		//	render.draw(normalizedInterpolationValue)
	}
	Renderer::ClearBuffers( mesh );
	CloseWindow(window, glcontext);
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

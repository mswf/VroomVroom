#include "Engine.h"
#include <SDL2/SDL.h>
#include <SDL2_net/SDL_net.h>
#include <lua.hpp>
#include "content.h"
#include "standardIncludes.h"
#include "renderer.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "glew.h"
#include "SDL2/SDL_opengl.h"
#include <iostream>
#include <cstring>
#include "Utilities/helperFunctions.h"

Engine::Engine()
{
}


Engine::~Engine()
{
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
		}
	}
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

void Engine::runMainLua()
{
	lua_State* L = luaL_newstate();
	string path;
	Content::CreateFilePath("main.lua", &path);

	luaL_openlibs(L);

	//tell lua where to find all the files relative to the content root folder
	{
		lua_getglobal(L, "package");
		lua_getfield(L, -1, "path");
		const char* cur_path = lua_tostring(L, -1);
		string new_path = string(cur_path);
		new_path += ";";
		new_path += Content::GetPath();
		new_path += "/?.lua";

		lua_pop(L, 1);
		lua_pushstring(L, new_path.c_str());
		lua_setfield(L, -2, "path");
		lua_pop(L, 1);
	}

	if (luaL_loadfile(L, path.c_str()))
	{
		//TODO error handling
		std::cout << "Could not open main.lua - The program will not run correctly" << std::endl;
		return;
	}

	if (lua_pcall(L, 0, 0, 0))
	{
		std::cout << "lua error: " << lua_tostring(L, -1) << std::endl;
	}
    
    luaState = L;

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

void Engine::InitGlew()
{
	glewExperimental = true;

	GLenum glewError = glewInit();
	if (glewError != GLEW_OK)
	{
		printf("Error initializing GLEW! %p\n", glewGetErrorString(glewError));
	}
	std::cout << "GL version " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLEW version " << glewGetString(GLEW_VERSION_MAJOR) << "." << glewGetString(GLEW_VERSION_MINOR) << std::endl;
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

void Engine::ShowSimpleWindowTwo(bool& show_another_window)
{
	if (show_another_window)
	{
		ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_FirstUseEver);
		ImGui::Begin("Another Window", &show_another_window);
		ImGui::Text("Hello");
		ImGui::End();
	}
}

void Engine::ShowSimpleWindowThree(bool& show_test_window)
{
	if (show_test_window)
	{
		ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
		ImGui::ShowTestWindow(&show_test_window);
	}
}


void Engine::Update()
{
    
    //LUA
    int dt = 16;
    //TODO use actual deltatime :)
    
    lua_getglobal(luaState, "Game");            //get the global 'Game' table
    lua_getfield(luaState, -1, "update");       //find the field with the name 'update'
    lua_pushnumber(luaState, dt);               //push dt to the stack
    if(lua_pcall(luaState, 1, 0, 0) != 0){      //call a function with 1 argument
        std::cout << "lua error: " << lua_tostring(luaState, -1) << std::endl;
    }
    //END LUA
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
    
    Renderer::RenderData * data = new Renderer::RenderData();
    Renderer::Camera * camera = new Renderer::Camera();
    float fov = 90.0f;
    float aspectRatio = 1280.0f / 720.0f;
    float zNear = 1.0f;
    float zFar = 100.0f;
    camera->eye = glm::vec3(1.0, 1.0, 1.0);
    camera->center = glm::vec3(0.0, 0.0, 0.0);
    Renderer::GetCamera(camera, Renderer::Projection::PERSPECTIVE, fov, aspectRatio, zNear, zFar);
    Renderer::GetRenderData(data);
    
	const uint16 millisecondModifier = 1000;
	const float gameFPS = 60;
	const float gameUpdateInterval = 1 / gameFPS * millisecondModifier;
	//
	uint32 currentTicks = SDL_GetTicks();
	uint32 prevTicks = currentTicks;
	bool running = true;
    
    //LUA
    lua_getglobal(luaState, "Game");            //get the global 'Game' table
    lua_getfield(luaState, -1, "main");       //find the field with the name 'main'
    if(lua_pcall(luaState, 0, 0, 0) != 0){      //call a function with 0 arguments
        std::cout << "lua error: " << lua_tostring(luaState, -1) << std::endl;
    }
    //END LUA

    
	while (running)
	{
		//multithreaded rendering goes here if we decide to do it
		/*if (threadedDrawingBusy)
		continue*/

		//game
		currentTicks = SDL_GetTicks();
		uint32 deltaTimeGame = currentTicks - prevTicks;
		//pollInput();
		PollEvent();

		unsigned short safeguard = 0;
		while (deltaTimeGame > gameUpdateInterval && safeguard < 10)
		{
            //TODO: Fix the game interval in for reducing framerate
            //SDL_Delay(gameUpdateInterval);
			Update();

			deltaTimeGame -= gameUpdateInterval;

			if (deltaTimeGame < gameUpdateInterval)
			{
				prevTicks = currentTicks;
				prevTicks -= deltaTimeGame;
			}

			++safeguard;
		}
        
        ImGui_ImplSdl_NewFrame(window);
        
        Renderer::RenderObject(SDL_GetTicks(), data, camera);
        
        ShowSimpleWindowOne(show_test_window, show_another_window);
        
        ShowSimpleWindowTwo(show_another_window);
        
        ShowSimpleWindowThree(show_test_window);
        
        ImGui::Render();
        SDL_GL_SwapWindow(window);
		//rendering
		//float normalizedInterpolationValue = deltaTimeGame / gameUpdateInterval
		//	//Do something with locking
		//	render.draw(normalizedInterpolationValue)
	}
    
    CloseWindow(window, glcontext, data, camera);
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


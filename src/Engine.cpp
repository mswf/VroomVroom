#define GLM_FORCE_RADIANS

#include <cstring>
#include <iostream>

#include "content.h"
#include "Engine.h"
#include "glew.h"
#include "Input.hpp"

#include "Components/cCamera.h"
#include "Components/cTransform.h"

#include "DataStructure/material.h"

#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_sdl.h"

#include "Modules/mInput.h"

#include <SDL2/SDL.h>
#include "SDL2/SDL_opengl.h"
#include <SDL2_net/SDL_net.h>

#include "Systems/renderer.h"
#include "Systems/luaSystem.h"
#include "Systems/uiSystem.h"

#include "Utilities/helperFunctions.h"
#include "Utilities/standardIncludes.h"
//defines runCommand
#include "Utilities/command.h"

Engine::Engine() :
	inputManager(NULL),
	listener(NULL),
	fileWatcher(NULL),
	resourceManager(NULL)
{
}

Engine::~Engine()
{
	//TODO: Clean up all entities and their components
	delete inputManager;
	delete resourceManager;
	delete listener;
}

void Engine::Init()
{
	inputManager = new Input();
	resourceManager = new ResourceManager();
	listener = new UpdateListener();
	fileWatcher = new FW::FileWatcher();

	//TODO: I don't really want to bind this here, but I also don't want to pass inputManager all over the place
	//Does it have to be a singular instance contained in Engine?
	mInput::SetInput(inputManager);

	OpenConfig();
	LuaSystem.Init();

	string watching ( Content::GetPath() );
	fileWatcher->addWatch( watching, listener, true );

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
	//inputManager->MidiListener();
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

    LuaSystem.Call(L, 0, 0);

	lua_getglobal(L, "CONTENT_PATH");
	if (lua_type(L, -1) != LUA_TSTRING)
	{
		Terminal.Warning("Problem in config.lua - CONTENT_PATH is not set to a valid string");
	}
	size_t len_cp;
	const char* contentPath = lua_tolstring(L, -1, &len_cp);

	Content::SetPath(contentPath, len_cp);
    
    lua_getglobal(L, "ATOM_PATH");
    if (lua_type(L, -1) == LUA_TSTRING)
    {
        LuaSystem.SetAtomPath(string(lua_tostring(L, -1)));
    }
    
    lua_getglobal(L, "MECHANIC_CMD");
    if (lua_type(L, -1) == LUA_TSTRING && Terminal.IsConnected() == false)
    {
        string cmd = lua_tostring(L, -1);
        Terminal.Log(cmd);
        RunCommand(cmd);
        Terminal.ReattemptConnection(1000);
    }


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
	InitGlew();
}

// TODO: Move to renderer
void Engine::InitGlew()
{
	glewExperimental = true;
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK)
	{
		std::string err( (const char*)glewGetErrorString(glewError) );
		Terminal.Warning( std::string("Error initializing GLEW! " + err ) );
	}

#ifdef DEBUG
	std::string glVersion( "GL version " + std::string( (const char*)glGetString(GL_VERSION) ) );
	std::string glslVersion( "GLSL version " + std::string( (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION) ) );
	std::string majorGlew( std::string( (const char*)glewGetString(GLEW_VERSION_MAJOR) ) );
	std::string minorGlew( std::string( (const char*)glewGetString(GLEW_VERSION_MINOR) ) );
	Terminal.LogOpenGL( glVersion, true );
	Terminal.LogOpenGL( glslVersion, true );
	Terminal.LogOpenGL( "GLEW version " + majorGlew + "." + minorGlew, true  );
	
	//#define LOG_EXTENSIONS
	#ifdef LOG_EXTENSIONS
	int NumberOfExtensions, i;
 	glGetIntegerv(GL_NUM_EXTENSIONS, &NumberOfExtensions);
 	for( i = 0; i < NumberOfExtensions; ++i )
	{
		std::string extensions( (const char*)glGetStringi(GL_EXTENSIONS, i) );
		Terminal.LogOpenGL( extensions );
 	}
	#endif
#endif
}

void Engine::Update(float deltaTime)
{
	LuaSystem.Update(deltaTime);

	const std::vector< std::string >* list = listener->GetEvents();
	for (std::vector<std::string>::const_iterator i = list->begin(); i != list->end(); ++i)
	{
		std::string msg = (std::string) * i;
		LuaSystem.SendReloadCallback( msg );
	}
	listener->ClearEvents();

	//TODO this should be refactored out at some point
	//It is neccesary now to poll network events from the socket
    //but it is now also necessary to reattempt connections, so probably it should not be factored out anymore
	Terminal.Update(deltaTime);
}

void Engine::UpdateLoop()
{

	SDL_Window* window;
	SDL_GLContext glcontext;
	SetupWindow(window, glcontext);
	
	ImGui_ImplSdl_Init(window);

	std::string rabbit = "/objects/Rabbit/Rabbit.obj";
	std::string rabbit_diffuse = "/objects/Rabbit/Rabbit_D.tga";
	std::string rabbit_normal = "/objects/Rabbit/Rabbit_N.tga";
	std::string snowman = "/objects/icy_snowman.obj";
	std::string snowman_diffuse = "/objects/snowman.png";
	
	string objectRabbit( Content::GetPath() + rabbit );
	string textureRabbitD( Content::GetPath() + rabbit_diffuse );
	string textureRabbitN( Content::GetPath() + rabbit_normal );
	//string objectSnowman( Content::GetPath() + snowman );
	//string textureSnowman( Content::GetPath() + snowman_diffuse );
	
	resourceManager->ImportObjFile( objectRabbit );
	unsigned int texD = resourceManager->LoadTexture( textureRabbitD.c_str() );
	unsigned int texN = resourceManager->LoadTexture( textureRabbitN.c_str() );
	
	/// TINAS PLAYGROUND!!!
	
	Shader* currentShader = new Shader();
	Material* mat = new Material( currentShader );
	mat->SetDiffuse(texD);
	mat->SetNormal(texN);
	
	CMeshRenderer* meshRenderer = new CMeshRenderer();
	//const Mesh* meshData = resourceManager->CreateCubeMesh();
	//const Mesh* meshData = resourceManager->CreateTriangleMesh();
	//const Mesh* meshData = resourceManager->CreateTetrahedronMesh();
	const Mesh* meshData = resourceManager->tempMesh;
	
	meshRenderer->SetMaterial(mat);
	meshRenderer->Buffer( meshData );
	
	Entity* box = new Entity( "MyLittleBox" );
	CTransform* t = Entity::GetComponent<CTransform>(box);
	Entity::AddComponent(box, meshRenderer);

	Entity* root = new Entity("Root");
	root->AddChild(box);

	Entity* camera = new Entity( "Main Camera" );
	CCamera* cam = new CCamera( Projection::PERSPECTIVE, 90.0f, 1280.0f / 720.0f, 0.2f, 1000.0f );
	Entity::AddComponent(camera, cam);

	/// TINAS PLAYGROUND ENDS!!!

	const float millisecondModifier = 1000.0f;
	const float gameFPS = 60.0f;
	const float gameUpdateInterval = 1 / gameFPS * millisecondModifier;
	uint32 currentTicks = SDL_GetTicks();
	uint32 prevTicks = currentTicks;
	static bool running = true;

	LuaSystem.Main();

	while (running)
	{
		//multithreaded rendering goes here if we decide to do it
		/*if (threadedDrawingBusy)
		continue*/

		//game
		currentTicks = SDL_GetTicks();
		float deltaTimeGame = currentTicks - prevTicks;

		PollEvent();
		fileWatcher->update();


		while (deltaTimeGame > gameUpdateInterval)
		{
			deltaTimeGame -= gameUpdateInterval;

			Update(gameUpdateInterval);
			root->Update();
			
			if (deltaTimeGame < gameUpdateInterval)
			{
				if ( inputManager->OnKey(SDLK_ESCAPE) )
				{
					running = false;
				}

				t->Yaw(1.0f);
				//t->Rotate( glm::vec3(1.0f, 1.0f, 1.0f) );
				
				if ( inputManager->OnKeyDown(SDLK_d) )
				{
					meshRenderer->drawType = GL_LINES;
				}
				
				if ( inputManager->OnKeyUp(SDLK_d) )
				{
					meshRenderer->drawType = GL_TRIANGLES;
				}
				
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

		glClearColor( 0.2f, 0.2f, 0.2f, 1.0f );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		Renderer::Render( SDL_GetTicks(), camera, box );
		UiSystem.Render();
		SDL_GL_SwapWindow(window);

		//rendering
		//float normalizedInterpolationValue = deltaTimeGame / gameUpdateInterval
		//	//Do something with locking
		//	render.draw(normalizedInterpolationValue)
	}
	
	//TODO(Valentinas): UnBuffer data from GPU
	CloseWindow(window, glcontext);
}

void Engine::InitSDL()
{
	if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0)
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

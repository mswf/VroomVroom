#define GLM_FORCE_RADIANS

#include <iostream>

#include "content.h"
#include "Engine.h"
#include "glew.h"

#include "Components/entity.h"
#include "Components/cTransform.h"
#include "Components/cMeshRenderer.h"
#include "Components/cDebugRenderer.h"

#include "DataStructure/resource_manager.h"
#include "DataStructure/mesh_generator.h"
#include "DataStructure/shader.h"

#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_sdl.h"

#include "Modules/mInput.h"
#include "Modules/mCamera.h"
#include "Modules/mSystem.h"

#include <SDL2/SDL.h>
#include "SDL2/SDL_opengl.h"
#include <SDL2_net/SDL_net.h>

#include "Systems/renderer.h"
#include "Systems/luaSystem.h"
#include "Systems/uiSystem.h"

#include "Utilities/helperFunctions.h"
#include "Utilities/random.h"
#include "Utilities/standardIncludes.h"

#include "Utilities/command.h"

FMOD::System* Engine::systemLowLevel = nullptr;
SDL_Window* Engine::window = nullptr;

Engine::Engine() :
	systemStudio(nullptr),
	fileWatcher(nullptr),
	glcontext(nullptr),
	running(false)
{
}

Engine::~Engine()
{
	delete fileWatcher;
	//delete systemStudio;
	//delete systemLowLevel;
}

void Engine::Init()
{
	SetupWindow(window, glcontext);
	renderer.SetWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	ImGui_ImplSdl_Init(window);
	OpenConfig();
	InitFMOD();

	Random::SetRandomSeed(HelperFunctions::GetTime().tm_sec);

	mCamera::SetRenderer(&renderer);
	mInput::SetInput(&inputManager);
	mSystem::SetRenderer(&renderer);
	LuaSystem.Init();

	fileWatcher = new FW::FileWatcher();
	fileWatcher->addWatch(Content::GetPath(), &listener, true);

	Assets.Initialize();
	renderer.Initialize();
}

void Engine::InitSDL()
{
	if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0)
	{
		printf("Error: %s\n", SDL_GetError());
		assert(false);
	}
	printf("Initialized: SDL_TIMER, SDL_VIDEO, SDL_EVENTS\n");
}

void Engine::InitSDLNet()
{
	if (SDLNet_Init() != 0)
	{
		printf("Error: %s\n", SDLNet_GetError());
		assert(false);
	}
}

void Engine::InitFMOD()
{
	unsigned int version;
	void* extradriverdata = 0;
	FMOD::Sound      *sound1;
	FMOD::ChannelGroup * group = 0;
	FMOD::Channel    *channel = 0;
	FMOD::DSP		 *dspecho;
	FMOD_RESULT result;
	FMOD::Studio::System::create(&systemStudio);
	systemStudio->getLowLevelSystem(&systemLowLevel);
	systemLowLevel->getVersion(&version);
	if (version < FMOD_VERSION)
	{
		Terminal.Error("Error version is not the FMOD version");
	}

	result = systemStudio->initialize(32, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, extradriverdata);
	if (result == FMOD_ERR_INITIALIZATION)
	{
		Terminal.Error("Error initializing FMOD Studio");
	}

	std::string smasterBank(Content::GetPath() + "/sounds/media/" + "Master Bank.bank");
	std::string smasterBankString(Content::GetPath() + "/sounds/media/" + "Master Bank.strings.bank");
	std::string svehicleBank(Content::GetPath() + "/sounds/media/" + "Vehicles.bank");


	FMOD::Studio::Bank* masterBank = nullptr;
	systemStudio->loadBankFile(smasterBank.c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &masterBank);

	FMOD::Studio::Bank* stringsBank = nullptr;
	systemStudio->loadBankFile(smasterBankString.c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &stringsBank);

	FMOD::Studio::Bank* vehiclesBank = nullptr;
	systemStudio->loadBankFile(svehicleBank.c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &vehiclesBank);

	FMOD::Studio::EventDescription* eventDescription = nullptr;
	systemStudio->getEvent("event:/Vehicles/Basic Engine", &eventDescription);

	FMOD::Studio::EventInstance* eventInstance = nullptr;
	eventDescription->createInstance(&eventInstance);

	FMOD::Studio::ParameterInstance* rpm = nullptr;
	eventInstance->getParameter("RPM", &rpm);

	rpm->setValue(1150);

	//result = eventInstance->start();

	// Position the listener at the origin
	FMOD_3D_ATTRIBUTES attributes = { { 0 } };
	attributes.forward.z = 1.0f;
	attributes.up.y = 1.0f;
	systemStudio->setListenerAttributes(0, &attributes);

	// Position the event 2 units in front of the listener
	attributes.position.z = 2.0f;
	eventInstance->set3DAttributes(&attributes);

	result = systemLowLevel->createDSPByType(FMOD_DSP_TYPE_ECHO, &dspecho);
	if (result != FMOD_OK)
	{
		std::cout << "Error: FMOD did not create digital signal processor type echo" << std::endl;
	}
	dspecho->setParameterFloat(FMOD_DSP_ECHO_DELAY, 150.0f);
	dspecho->setParameterFloat(FMOD_DSP_ECHO_WETLEVEL, 150.0f);
	result = systemLowLevel->getMasterChannelGroup(&group);
	//group->addDSP(FMOD_CHANNELCONTROL_DSP_HEAD, dspecho);
	if (result != FMOD_OK)
	{
		std::cout << "Error: FMOD did not add effect to channel" << std::endl;
	}


	//result = systemLowLevel->createSound( (Content::GetPath() + "/sounds/piano2.wav").c_str(), FMOD_DEFAULT, 0, &sound1);
	result = systemLowLevel->createStream((Content::GetPath() + "/sounds/piano2.wav").c_str(), FMOD_DEFAULT, 0, &sound1);

	if (result == FMOD_ERR_FILE_NOTFOUND)
	{
		std::cout << "Error: FMOD did not find the file" << std::endl;
	}
	sound1->setMode(FMOD_LOOP_OFF);
	bool play = false;
	if (play)
	{
		systemLowLevel->playSound(sound1, 0, false, &channel);
	}
}

void Engine::InitGlew()
{
	glewExperimental = true;
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK)
	{
		std::string err((const char*)glewGetErrorString(glewError));
		Terminal.Warning(std::string("Error initializing GLEW! " + err));
	}
	//Calling to clear Invalid enum error, glew generates it on initialization
	glGetError();

#ifdef DEBUG

	std::string glVersion("GL version " + std::string((const char*)glGetString(GL_VERSION)));
	std::string glslVersion("GLSL version " + std::string((const char*)glGetString(GL_SHADING_LANGUAGE_VERSION)));
	std::string majorGlew(std::string((const char*)glewGetString(GLEW_VERSION_MAJOR)));
	std::string minorGlew(std::string((const char*)glewGetString(GLEW_VERSION_MINOR)));
	Terminal.LogOpenGL(glVersion, true);
	Terminal.LogOpenGL(glslVersion, true);
	Terminal.LogOpenGL("GLEW version " + majorGlew + "." + minorGlew, true);

//#define LOG_EXTENSIONS
#ifdef LOG_EXTENSIONS
	int NumberOfExtensions, i;
	glGetIntegerv(GL_NUM_EXTENSIONS, &NumberOfExtensions);
	for (i = 0; i < NumberOfExtensions; ++i)
	{
		std::string extensions((const char*)glGetStringi(GL_EXTENSIONS, i));
		Terminal.LogOpenGL(extensions);
	}

#endif
#endif
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
	window = SDL_CreateWindow("VroomVroom", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_SHOWN); //SDL_WINDOW_ALLOW_HIGHDPI
	glcontext = SDL_GL_CreateContext(window);
	InitGlew();

	SDL_SetHint("SDL_VIDEO_MINIMIZE_ON_FOCUS_LOSS", "0");
}

uint32 Engine::GetTicks()
{
	return SDL_GetTicks();
}

void Engine::WindowEvent( SDL_WindowEvent window )
{
	switch ( window.event )
	{
		case SDL_WINDOWEVENT_RESIZED:	//This event is always preceded by SIZE_CHANGED, but only occurs if not changed through API calls (thus, by a user)

			break;
		case SDL_WINDOWEVENT_SIZE_CHANGED:
			renderer.SetWindowSize(window.data1, window.data2);

			int dimensions[2];
			dimensions[0] = window.data1;
			dimensions[1] = window.data2;
			LuaSystem.EventCallback("onWindowResized", 2, dimensions);
			break;
		case SDL_WINDOWEVENT_ENTER:		//Gained mouse focus
			LuaSystem.EventCallback("onMouseEntered");
			break;
		case SDL_WINDOWEVENT_LEAVE:		//Lost mouse focus
			LuaSystem.EventCallback("onMouseLeft");
			break;
		case SDL_WINDOWEVENT_FOCUS_GAINED:	//Gained keyboard focus
			LuaSystem.EventCallback("onFocusGained");
			break;
		case SDL_WINDOWEVENT_FOCUS_LOST:		//Lost keyboard focus
			LuaSystem.EventCallback("onFocusLost");
			break;
		case SDL_WINDOWEVENT_MINIMIZED:
			LuaSystem.EventCallback("onWindowMinimized");
			break;
		case SDL_WINDOWEVENT_MAXIMIZED:
			LuaSystem.EventCallback("onWindowMaximized");
			break;
		case SDL_WINDOWEVENT_RESTORED:
			LuaSystem.EventCallback("onWindowRestored");
			break;
		case SDL_WINDOWEVENT_SHOWN:
			LuaSystem.EventCallback("onWindowShown");
			break;
		case SDL_WINDOWEVENT_HIDDEN:
			LuaSystem.EventCallback("onWindowHidden");
			break;
	}
}

void Engine::PollEvent()
{
	inputManager.resetScrollWheel();

	SDL_Event event;
	while ( SDL_PollEvent( &event ) )
	{
		ImGui_ImplSdl_ProcessEvent( &event );
		WindowEvent( event.window );
		if ( event.type == SDL_QUIT )
		{
			running = false;
		}
		if (event.type == SDL_DROPFILE)
		{
			string path = static_cast<std::string>(event.drop.file);
			string* paths[1] = {&path};
			LuaSystem.EventCallback("onDropFile", 1, paths);
		}
		else
		{
			inputManager.Update(&event);
		}

	}
	//inputManager->MidiListener();
	inputManager.StateReset();
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

void Engine::FilewatcherUpdate()
{
	const std::vector< std::string >* list = listener.GetEvents();
	for (std::vector<std::string>::const_iterator i = list->begin(); i != list->end(); ++i)
	{
		std::string msg = static_cast<std::string>(*i);
		std::cout << msg << std::endl;
		LuaSystem.SendReloadCallback( msg );
	}
	listener.ClearEvents();
}

void Engine::UpdateLoop()
{


	/// TINAS PLAYGROUND!!!
	
	/*
	
	Assets.ImportMesh("objects/Teapot/teapot.obj");
	Assets.SetMeshScale("objects/Teapot/teapot.obj", 0.01f);
	Entity* teapot = new Entity( "Teapot" );

	*/
	/*
	float lineLength = 1.0f;
	int lineAmount = 10;
	for (int i = 0; i < lineAmount + 1; ++i)
	{
		float p = (i * 0.1f);
		// Along x axis
		debugRenderer->AddLine(  Line( glm::vec3( 0.0, 0.0, p ), glm::vec3( lineLength, 0.0, p ), random_colour() ) );
		debugRenderer->AddLine(  Line( glm::vec3( 0.0, p, 0.0 ), glm::vec3( lineLength, p, 0.0 ), random_colour() ) );
		// Along y axis
		debugRenderer->AddLine(  Line( glm::vec3( p, 0.0, 0.0 ), glm::vec3( p, lineLength, 0.0 ), random_colour() ) );
		debugRenderer->AddLine(  Line( glm::vec3( p, 0.0, 0.0 ), glm::vec3( p, 0.0, lineLength ), random_colour() ) );
		// Along z axis
		debugRenderer->AddLine(  Line( glm::vec3( 0.0, 0.0, p ), glm::vec3( 0.0, lineLength, p ), random_colour() ) );
		debugRenderer->AddLine(  Line( glm::vec3( 0.0, p, 0.0 ), glm::vec3( 0.0, p, lineLength ), random_colour() ) );
	}
	*/
	
	/// TINAS PLAYGROUND ENDS!!!



	const float millisecondModifier = 1000.0f;
	const float gameFPS = 60.0f;
	const float updateInterval = 1 / gameFPS * millisecondModifier;
	uint32 currentTicks = SDL_GetTicks();
	uint32 prevTicks = currentTicks;
	running = true;
	LuaSystem.Main();
	
	while (running)
	{
		currentTicks = GetTicks();
		float deltaTimeGame = currentTicks - prevTicks;

		// This part makes it get called 60 times per second
		while (deltaTimeGame > updateInterval)
		{
			PollEvent();
			fileWatcher->update();
			deltaTimeGame -= updateInterval;

			systemStudio->update();

			LuaSystem.Update(updateInterval / 1000);
			FilewatcherUpdate();
			Terminal.Update(updateInterval / 1000);


			if (deltaTimeGame < updateInterval)
			{
				if ( inputManager.OnKey(SDLK_ESCAPE) )
				{
					running = false;
				}
				if ( inputManager.OnKeyDown(SDLK_F1) )
				{
					renderer.CaptureScreen();
				}
				
				prevTicks = currentTicks;
				prevTicks -= deltaTimeGame;
			}
		}

		CleanUpEntities();

		ImGui_ImplSdl_NewFrame(window);

		renderer.SetTime( GetTicks() );
		renderer.Render();

		UiSystem.Render();
		
		SDL_GL_SwapWindow(window);

	}
	CloseWindow(window, glcontext);
}

glm::vec3 Engine::CastRay( glm::vec3 origin, glm::vec3 direction, glm::vec3 plane, float distance )
{
	float t = 0.0f;
	glm::vec3 coord(0);
	float hit = glm::dot(direction, plane);
	if ( hit != 0 )
	{
		t = -( glm::dot(plane, origin) + distance ) / hit;
		coord = origin + glm::normalize(direction) * t;
	}
	return coord;
}
void Engine::CleanUpEntities()
{
	if ( Entity::shouldClean )
	{
		// Causes memory leaks
		std::vector<Entity*> list = Entity::root->GetChildren();
		std::vector<Entity*>::iterator it = list.begin();
		std::vector<Entity*>::iterator end = list.end();
		for ( ; it != end; ++it )
		{
			if ( (*it)->IsSetToDestroy() )
			{
				Terminal.Log( (*it)->GetName() + " is destroyed." );
				(*it)->ClearComponents();
				delete (*it);
				(*it) = nullptr;
			}
		}
		Entity::shouldClean = false;
	}
}

glm::vec2 Engine::GetWindowSize()
{
	int w, h;
	SDL_GetWindowSize(window, &w, &h);

	return glm::vec2(w, h);
}

void Engine::SetWindowSize(glm::vec2 dimensions)
{
	SDL_SetWindowSize(window, dimensions.x, dimensions.y);
}

glm::vec2 Engine::GetWindowPosition()
{
	int x, y;
	SDL_GetWindowPosition(window, &x, &y);

	return glm::vec2(x, y);
}

void Engine::SetWindowPosition(glm::vec2 position)
{
	SDL_SetWindowPosition(window, position.x, position.y);
}

void Engine::SetWindowFullscreenMode(int fullscreenMode)
{
	if (fullscreenMode == 2)
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
	else if (fullscreenMode == 1)
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
	else
		SDL_SetWindowFullscreen(window, 0);
}

int Engine::GetWindowFullscreenMode()
{
	//TODO: implement this
	return 0;
}

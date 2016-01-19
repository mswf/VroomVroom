#define GLM_FORCE_RADIANS

#include <iostream>

#include "content.h"
#include "Engine.h"
#include "glew.h"

#include "Components/cCamera.h"
#include "Components/cCollider.h"
#include "Components/cTransform.h"
#include "Components/cMeshRenderer.h"
#include "Components/cDebugRenderer.h"

#include "DataStructure/material.h"
#include "DataStructure/mesh_generator.h"

#include "DataStructure/mesh.h"
#include "DataStructure/texture.h"
#include "DataStructure/shader.h"

#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_sdl.h"

#include "Modules/mInput.h"
#include "Modules/mCamera.h"

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

#include "Components/entity.h"
#include "DataStructure/resource_manager.h"

#include "Colliders/boxCollider.h"
#include "Colliders/sphereCollider.h"

FMOD::System* Engine::systemLowLevel = nullptr;
SDL_Window* Engine::window = nullptr;

Engine::Engine() :
	systemStudio(nullptr),
	fileWatcher(nullptr),
	skybox_map(0),
	takeScreen(false),
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
	LuaSystem.Init();

	fileWatcher = new FW::FileWatcher();
	fileWatcher->addWatch(Content::GetPath(), &listener, true);

	ImportAssets();
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
	FMOD_RESULT result = FMOD::Studio::System::create(&systemStudio);
	systemStudio->getLowLevelSystem(&systemLowLevel);
	result = systemLowLevel->getVersion(&version);
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
	result = systemStudio->loadBankFile(smasterBank.c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &masterBank);

	FMOD::Studio::Bank* stringsBank = nullptr;
	result = systemStudio->loadBankFile(smasterBankString.c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &stringsBank);

	FMOD::Studio::Bank* vehiclesBank = nullptr;
	result = systemStudio->loadBankFile(svehicleBank.c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &vehiclesBank);

	FMOD::Studio::EventDescription* eventDescription = nullptr;
	result = systemStudio->getEvent("event:/Vehicles/Basic Engine", &eventDescription);

	FMOD::Studio::EventInstance* eventInstance = nullptr;
	result = eventDescription->createInstance(&eventInstance);

	FMOD::Studio::ParameterInstance* rpm = nullptr;
	result = eventInstance->getParameter("RPM", &rpm);

	result = rpm->setValue(1150);

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
	result = dspecho->setParameterFloat(FMOD_DSP_ECHO_DELAY, 150.0f);
	result = dspecho->setParameterFloat(FMOD_DSP_ECHO_WETLEVEL, 150.0f);
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
	result = sound1->setMode(FMOD_LOOP_OFF);
	bool play = false;
	if (play)
	{
		result = systemLowLevel->playSound(sound1, 0, false, &channel);
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
			LuaSystem.EventCallback("onMouseEntered", 0, nullptr);
			break;
		case SDL_WINDOWEVENT_LEAVE:		//Lost mouse focus
			LuaSystem.EventCallback("onMouseLeft", 0, nullptr);
			break;
		case SDL_WINDOWEVENT_FOCUS_GAINED:	//Gained keyboard focus
			LuaSystem.EventCallback("onFocusGained", 0, nullptr);
			break;
		case SDL_WINDOWEVENT_FOCUS_LOST:		//Lost keyboard focus
			LuaSystem.EventCallback("onFocusLost", 0, nullptr);
			break;
		case SDL_WINDOWEVENT_MINIMIZED:
			LuaSystem.EventCallback("onWindowMinimized", 0, nullptr);
			break;
		case SDL_WINDOWEVENT_MAXIMIZED:
			LuaSystem.EventCallback("onWindowMaximized", 0, nullptr);
			break;
		case SDL_WINDOWEVENT_RESTORED:
			LuaSystem.EventCallback("onWindowRestored", 0, nullptr);
			break;
		case SDL_WINDOWEVENT_SHOWN:
			LuaSystem.EventCallback("onWindowShown", 0, nullptr);
			break;
		case SDL_WINDOWEVENT_HIDDEN:
			LuaSystem.EventCallback("onWindowHidden", 0, nullptr);
			break;
	}
}

void Engine::PollEvent()
{
	SDL_Event event;
	while ( SDL_PollEvent( &event ) )
	{
		ImGui_ImplSdl_ProcessEvent( &event );
		WindowEvent( event.window );
		if ( event.type == SDL_QUIT )
		{
			//exit(EXIT_SUCCESS);
			running = false;
			//TODO: don't exit instantly, rather disrupt the game loop and exit through a controlled flow
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

void Engine::ImportAssets()
{
	auto printErr = []( std::vector<std::string>& errors )
	{
		std::vector<std::string>::const_iterator iter = errors.begin();
		std::vector<std::string>::const_iterator end = errors.end();
		for ( ; iter != end; ++iter)
		{
			Terminal.Warning( (*iter) );
		}
		errors.clear();
		Terminal.Log("Import failed", true);
	};
	ResourceManager& rm = Assets;
	rm.Initialize();
	std::vector< std::string > cube_map, errors;
	std::vector< std::pair< std::string, GLSLShaderType > > shaders;



	// Cube map
	//rm.ImportImage("images/Meadow/negx.jpg", false);
	//rm.ImportImage("images/Meadow/negy.jpg", false);
	//rm.ImportImage("images/Meadow/negz.jpg", false);
	//rm.ImportImage("images/Meadow/posx.jpg", false);
	//rm.ImportImage("images/Meadow/posy.jpg", false);
	//rm.ImportImage("images/Meadow/posz.jpg", false);

	cube_map.push_back( "images/Dusk/negx_custom.png" );
	cube_map.push_back( "images/Dusk/negy_custom.png" );
	cube_map.push_back( "images/Dusk/negz_custom.png" );
	cube_map.push_back( "images/Dusk/posx_custom.png" );
	cube_map.push_back( "images/Dusk/posy_custom.png" );
	cube_map.push_back( "images/Dusk/posz_custom.png" );



	rm.ImportShader( "shaders/line_vert.glsl", GLSLShaderType::VERTEX );
	rm.ImportShader( "shaders/line_frag.glsl", GLSLShaderType::FRAGMENT );
	const char* sh_objs2[] = { "shaders/line_vert.glsl", "shaders/line_frag.glsl", nullptr };
	Assets.CreateShaderProgram("__Debug_program", sh_objs2, 2);

	rm.ImportShader( "shaders/skybox_vert.glsl", GLSLShaderType::VERTEX );
	rm.ImportShader( "shaders/skybox_frag.glsl", GLSLShaderType::FRAGMENT );
	const char* sh_objs[] = { "shaders/skybox_vert.glsl", "shaders/skybox_frag.glsl", nullptr };
	Assets.CreateShaderProgram("__Skybox_program", sh_objs, 2);
	rm.ImportShader( "shaders/quad_vert.glsl", GLSLShaderType::VERTEX );
	rm.ImportShader( "shaders/quad_frag.glsl", GLSLShaderType::FRAGMENT );
	const char* sh_objs3[] = { "shaders/quad_vert.glsl", "shaders/quad_frag.glsl", nullptr };
	Assets.CreateShaderProgram("quad", sh_objs3, 2);

	bool successfulImport = rm.ImportImage( cube_map, errors, false );
	if (!successfulImport)
	{
		printErr(errors);
	}

	if (true)
		//if (false)
	{
		int width, height;
		width = height = rm.GetImageData("images/Dusk/negx_custom.png")->width;
		std::vector< std::pair< uint8*, uint32 > > textures;
		textures.push_back( std::make_pair( rm.GetImageData("images/Dusk/negx_custom.png")->pixelData, GL_TEXTURE_CUBE_MAP_NEGATIVE_X ) );
		textures.push_back( std::make_pair( rm.GetImageData("images/Dusk/negy_custom.png")->pixelData, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y ) );
		textures.push_back( std::make_pair( rm.GetImageData("images/Dusk/negz_custom.png")->pixelData, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z ) );
		textures.push_back( std::make_pair( rm.GetImageData("images/Dusk/posx_custom.png")->pixelData, GL_TEXTURE_CUBE_MAP_POSITIVE_X ) );
		textures.push_back( std::make_pair( rm.GetImageData("images/Dusk/posy_custom.png")->pixelData, GL_TEXTURE_CUBE_MAP_POSITIVE_Y ) );
		textures.push_back( std::make_pair( rm.GetImageData("images/Dusk/posz_custom.png")->pixelData, GL_TEXTURE_CUBE_MAP_POSITIVE_Z ) );

		skybox_map = rm.CreateCubeMap(&textures, width, height);
	}

	EnvironmentCube();
	Quad();
	renderer.skyboxMap = skybox_map;
}

void Engine::WeikieTestCode()
{
	Entity* a =  new Entity( "testB" );
	Entity* b =  new Entity( "testB" );

	float c = 0;
	float d = 0.5f;

	a->transform->SetPosition(c, c, c);
	b->transform->SetPosition(d, d, d);

	CCollider *col1 = new SphereCollider();
	CCollider* col2 = new BoxCollider();
	CCollider* col3 = new BoxCollider();
	CCollider *col4 = new SphereCollider();

	float radius = 1;
	float boxSize = 2;


	//col1->SetRadius(radius);
	//col2->SetRadius(radius);
	//
	//col1->SetCollisionBox(boxSize, boxSize, boxSize);
	//col2->SetCollisionBox(boxSize, boxSize, boxSize);


	Entity::AddComponent(a, col1);
	Entity::AddComponent(b, col2);

	bool asd = col1->Collide(*col2);
	bool asd2 = col2->Collide(*col1);
	bool asd3 = col1->Collide(*col4);
	bool asd4 = col2->Collide(*col3);

	printf( "%i \n ", asd);
	printf( "%i \n ", asd2);
}

void Engine::UpdateLoop()
{
#ifdef WEIKIE
	WeikieTestCode();
#endif

	/// TINAS PLAYGROUND!!!

	/*
	auto random_vec3 = []( int min, int max ) -> glm::vec3
	{
		float modif = 0.5f;
		return glm::vec3( Random::Next(min, max) * modif, Random::Next(min, max) * modif, Random::Next(min, max) * modif );
	};

	Assets.ImportMesh("objects/Teapot/teapot.obj");
	Assets.SetMeshScale("objects/Teapot/teapot.obj", 0.01f);
	Entity* a = new Entity( "testA" );
	Entity* ac = new Entity( "testAChild" );
	a->AddChild(ac);
	Entity* ac2 = new Entity( "testA2Child" );
	ac->AddChild(ac2);

	Entity::Destroy( a );

	//Entity* b =  new Entity( "testB" );
	//Entity* bc =  new Entity( "testBChild", b );
	Entity* list[3] = { a, ac, ac2 };

	for (int i = 0; i < 3; ++i )
	{
		CMeshRenderer* rend = new CMeshRenderer();
		Entity::AddComponent( list[i], rend);
		rend->SetModel( "objects/Teapot/teapot.obj" );
		list[i]->transform->SetPosition( random_vec3( -2, 2 ) );
	}

	Entity::GetComponentInChildren<CMeshRenderer>(a);

		CDebugRenderer* drend = new CDebugRenderer();
		Entity::AddComponent( ac, drend);
		Entity::AddComponent( ac, drend);
		Entity::RemoveComponent( ac, drend);
	*/


	/*

		Entity* debugObject = new Entity( "Debugger" );
		CDebugRenderer* debugRenderer = new CDebugRenderer();
		Entity::AddComponent(debugObject, debugRenderer);
		debugObject->transform->SetPosition(glm::vec3(-1,-1,-1));
		debugRenderer->SetDrawPoints(true);
		debugRenderer->SetPointSize(10.0f);

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

	/*
	Entity* camera = new Entity( "Main Camera" );
	//	CCamera* cam = new CCamera( Projection::PERSPECTIVE, 1280.0f / 720.0f );
	Entity::AddComponent(camera, cam);
	camera->transform->SetPosition( glm::vec3( 4, 4, -4 ) );
	renderer->SetCamera( cam );
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
					takeScreen = true;
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

		TakeScreenShot();
		SDL_GL_SwapWindow(window);

	}
	CloseWindow(window, glcontext);
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
				Terminal.Log( (*it)->name + " is destroyed." );
				(*it)->ClearComponents();
				delete (*it);
				(*it) = nullptr;
			}
		}
		Entity::shouldClean = false;
	}
}

void Engine::TakeScreenShot()
{
	if ( takeScreen )
	{
		takeScreen = false;
		renderer.ScreenGrab();
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
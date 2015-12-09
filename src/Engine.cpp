#define GLM_FORCE_RADIANS

#include <cstring>
#include <ctime>
#include <iostream>

#include "content.h"
#include "Engine.h"
#include "glew.h"
#include "Input.hpp"

#include "Components/cCamera.h"
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
//defines runCommand
#include "Utilities/command.h"

Engine::Engine() :
	inputManager(NULL),
	listener(NULL),
	renderer(NULL),
	fileWatcher(NULL)
{
}

Engine::~Engine()
{
	//TODO: Clean up all entities and their components
	delete inputManager;
	delete renderer;
	delete listener;
	delete fileWatcher;
}

void Engine::Init()
{
	SetSeed();

	inputManager = new Input();
	renderer = new Renderer::RenderSystem();
	listener = new UpdateListener();
	fileWatcher = new FW::FileWatcher();

	//TODO: I don't really want to bind this here, but I also don't want to pass inputManager all over the place
	//Does it have to be a singular instance contained in Engine?
	mInput::SetInput(inputManager);
	mCamera::SetRenderer(renderer);

	OpenConfig();
	LuaSystem.Init();

	string watching ( Content::GetPath() );
	fileWatcher->addWatch( watching, listener, true );

}

void Engine::SetSeed()
{
	time_t timer;
	struct tm y2k = {0};
	double seconds;
	y2k.tm_hour = 0;
	y2k.tm_min = 0;
	y2k.tm_sec = 0;
	y2k.tm_year = 100;
	y2k.tm_mon = 0;
	y2k.tm_mday = 1;
	time(&timer);  /* get current time; same as: timer = time(NULL)  */
	seconds = difftime(timer, mktime(&y2k));
	Random::SetRandomSeed(seconds);
}

uint32 Engine::GetTicks()
{
	return SDL_GetTicks();
}

void Engine::WindowEvent( SDL_WindowEvent window )
{
	switch ( window.event )
	{
		case SDL_WINDOWEVENT_FOCUS_GAINED:
		{
			//std::cout << "Window focus gained, keyboard " << std::endl;
			//LuaSystem
			break;
		}
		case SDL_WINDOWEVENT_FOCUS_LOST:
		{
			//std::cout << "Window focus lost, keyboard " << std::endl;
			//LuaSystem.
			break;
		}
		case SDL_WINDOWEVENT_ENTER:
		{
			//std::cout << "Window entered, mouse " << std::endl;
			//LuaSystem.
			break;
		}
		case SDL_WINDOWEVENT_LEAVE:
		{
			//std::cout << "Window left, mouse " << std::endl;
			//LuaSystem.
			break;
		}
		case SDL_WINDOWEVENT_CLOSE:
		{
			//std::cout << "Window close, exit button " << std::endl;
			//LuaSystem.
			break;
		}
		case SDL_WINDOWEVENT_HIDDEN:
		{
			//std::cout << "Window hidden? " << std::endl;
			//LuaSystem.
			break;
		}
		case SDL_WINDOWEVENT_MAXIMIZED:
		{
			//std::cout << "Window MAXIMIZED! " << std::endl;
			//LuaSystem.
			break;
		}
		case SDL_WINDOWEVENT_MINIMIZED:
		{
			//std::cout << "Window minimized.. " << std::endl;
			//LuaSystem.
			break;
		}
		case SDL_WINDOWEVENT_RESIZED:
		case SDL_WINDOWEVENT_SIZE_CHANGED:
		{
			//std::cout << "Window size changed to -> " << window.data1 << "x" << window.data2 << std::endl;
			//LuaSystem.
			break;
		}
		default:
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
	window = SDL_CreateWindow("VroomVroom", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_SHOWN ); //SDL_WINDOW_ALLOW_HIGHDPI
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
	//Calling to clear Invalid enum error, glew generates it on initialization
	glGetError();

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
	for ( i = 0; i < NumberOfExtensions; ++i )
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
		std::string msg = (std::string) (*i);
		std::cout << msg << std::endl;
		LuaSystem.SendReloadCallback( msg );
	}
	listener->ClearEvents();

	//TODO this should be refactored out at some point
	//It is neccesary now to poll network events from the socket
	//but it is now also necessary to reattempt connections, so probably it should not be factored out anymore
	Terminal.Update(deltaTime);
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
	ResourceManager& rm = ResourceManager::getInstance();
	rm.Initialize();
	std::vector< std::string > meshes, images, cube_map, errors;
	std::vector< std::pair< std::string, GLSLShaderType > > shaders;
	meshes.push_back( "objects/Rabbit/Rabbit.obj" );
	meshes.push_back( "objects/icy_snowman.obj" );
	images.push_back( "objects/snowman.png" );
	images.push_back( "objects/object_group_test/checker_1.png" );
	images.push_back( "objects/object_group_test/checker_2.png" );

	// Cube map
	/*
	cube_map.push_back( "/images/LancellottiChapel/negx.jpg" );
	cube_map.push_back( "/images/LancellottiChapel/negy.jpg" );
	cube_map.push_back( "/images/LancellottiChapel/negz.jpg" );
	cube_map.push_back( "/images/LancellottiChapel/posx.jpg" );
	cube_map.push_back( "/images/LancellottiChapel/posy.jpg" );
	cube_map.push_back( "/images/LancellottiChapel/posz.jpg" );
	*/

	shaders.push_back( std::pair<std::string, GLSLShaderType >( "shaders/line_vert.glsl", GLSLShaderType::VERTEX) );
	shaders.push_back( std::pair<std::string, GLSLShaderType >( "shaders/line_frag.glsl", GLSLShaderType::FRAGMENT) );

	shaders.push_back( std::pair<std::string, GLSLShaderType >( "shaders/skybox_vert.glsl", GLSLShaderType::VERTEX) );
	shaders.push_back( std::pair<std::string, GLSLShaderType >( "shaders/skybox_frag.glsl", GLSLShaderType::FRAGMENT) );

	bool successfulImport = rm.ImportMesh( meshes, errors );
	if (!successfulImport)
	{
		printErr(errors);
	}
	successfulImport = rm.ImportImage( images, errors );
	if (!successfulImport)
	{
		printErr(errors);
	}
	successfulImport = rm.ImportImage( cube_map, errors, false );
	if (!successfulImport)
	{
		printErr(errors);
	}
	successfulImport = rm.ImportShader( shaders, errors );
	if (!successfulImport)
	{
		printErr(errors);
	}
	/*
	int width, height;
	width = height = rm.GetImageData("/images/LancellottiChapel/negx.jpg")->width;
	std::vector< std::pair< unsigned char*, unsigned int > > textures;
	textures.push_back( std::pair<unsigned char*, unsigned int>( rm.GetImageData("/images/LancellottiChapel/negx.jpg")->pixelData, GL_TEXTURE_CUBE_MAP_NEGATIVE_X ) );
	textures.push_back( std::pair<unsigned char*, unsigned int>( rm.GetImageData("/images/LancellottiChapel/negy.jpg")->pixelData, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y ) );
	textures.push_back( std::pair<unsigned char*, unsigned int>( rm.GetImageData("/images/LancellottiChapel/negz.jpg")->pixelData, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z ) );
	textures.push_back( std::pair<unsigned char*, unsigned int>( rm.GetImageData("/images/LancellottiChapel/posx.jpg")->pixelData, GL_TEXTURE_CUBE_MAP_POSITIVE_X ) );
	textures.push_back( std::pair<unsigned char*, unsigned int>( rm.GetImageData("/images/LancellottiChapel/posy.jpg")->pixelData, GL_TEXTURE_CUBE_MAP_POSITIVE_Y ) );
	textures.push_back( std::pair<unsigned char*, unsigned int>( rm.GetImageData("/images/LancellottiChapel/posz.jpg")->pixelData, GL_TEXTURE_CUBE_MAP_POSITIVE_Z ) );
	skybox_map = rm.CreateCubeMap(&textures, width, height);
	*/
}

void Engine::UpdateLoop()
{

	SDL_Window* window;
	SDL_GLContext glcontext;
	SetupWindow(window, glcontext);

	ImGui_ImplSdl_Init(window);

	ImportAssets();
	//InitFMOD();
	
	const char* sh_objs[] = { "shaders/skybox_vert.glsl", "shaders/skybox_frag.glsl", NULL };
	const char* sh_objs2[] = { "shaders/line_vert.glsl", "shaders/line_frag.glsl", NULL };
	ResourceManager::getInstance().CreateShaderProgram("__Skybox_program", sh_objs, 2);
	ResourceManager::getInstance().CreateShaderProgram("__Debug_program", sh_objs2, 2);
	EnvironmentCube();
	
	renderer->SetWindowSize(1280, 720);
	renderer->Initialize();

	/// TINAS PLAYGROUND!!!

	Material* mt;

	mt = ResourceManager::getInstance().GetMaterialByName("Default");
	mt->SetDiffuseTexture("objects/snowman.png");

/*
	auto random_colour = []() -> glm::vec3
	{
		return glm::vec3( Random::Next(100) * 0.01f, Random::Next(100) * 0.01f, Random::Next(100) * 0.01f );
	};
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
	CCamera* cam = new CCamera( Projection::ORTHOGRAPHIC, 1280.0f / 720.0f, -5.0f, 50000.0f );
//	CCamera* cam = new CCamera( Projection::PERSPECTIVE, 1280.0f / 720.0f );
	Entity::AddComponent(camera, cam);
	camera->transform->SetPosition( glm::vec3( 4, 4, -4 ) );
	renderer->SetCamera( cam );
	 */
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
		currentTicks = GetTicks();
		float deltaTimeGame = currentTicks - prevTicks;

		// This part makes it get called 60 times per second
		while (deltaTimeGame > gameUpdateInterval)
		{
			PollEvent();
			fileWatcher->update();
			deltaTimeGame -= gameUpdateInterval;

			//systemStudio->update();
			
			Update(gameUpdateInterval / 1000);

			if (deltaTimeGame < gameUpdateInterval)
			{
				if ( inputManager->OnKey(SDLK_ESCAPE) )
				{
					running = false;
				}
/*
				if ( inputManager->OnKey(SDLK_RIGHT) )
				{
					camera->transform->Translate( glm::vec3( 0.1, 0.0, 0.0 ) );
				}
				if ( inputManager->OnKey(SDLK_LEFT) )
				{
					camera->transform->Translate( glm::vec3( -0.1, 0.0, 0.0 ) );
				}

				if ( inputManager->OnKey(SDLK_UP) )
				{
					camera->transform->Translate( glm::vec3( 0.0, 0.0, 0.1 ) );
				}
				if ( inputManager->OnKey(SDLK_DOWN) )
				{
					camera->transform->Translate( glm::vec3( 0.0, 0.0, -0.1 ) );
				}

				if ( inputManager->OnKeyDown(SDLK_r) )
				{
					ResourceManager::getInstance().ReImportImage("objects/snowman.png");
					ResourceManager::getInstance().ReImportShader("shaders/line_vert.glsl", GLSLShaderType::VERTEX );
				}
*/
				//box->transform->Yaw(1.0f);
				//box2->transform->Roll(1.0f);

				prevTicks = currentTicks;
				prevTicks -= deltaTimeGame;
			}
		}

		ImGui_ImplSdl_NewFrame(window);

		glViewport( 0, 0, (int)ImGui::GetIO().DisplaySize.x * ImGui::GetIO().DisplayFramebufferScale.x, (int)ImGui::GetIO().DisplaySize.y * ImGui::GetIO().DisplayFramebufferScale.y );
		glClearColor( 0.91f, 0.91f, 0.91f, 1.0f );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		renderer->SetTime( GetTicks() );
		renderer->Render();


		UiSystem.Render();

		SDL_GL_SwapWindow(window);

		//rendering
		//float normalizedInterpolationValue = deltaTimeGame / gameUpdateInterval
		//	//Do something with locking
		//	render.draw(normalizedInterpolationValue)
	}
	CloseWindow(window, glcontext);
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
		Terminal.Error( "Error version is not the FMOD version" );
	}
 
	result = systemStudio->initialize( 32, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, extradriverdata );
	if (result == FMOD_ERR_INITIALIZATION)
	{
		Terminal.Error( "Error initializing FMOD Studio" );
	}

	std::string smasterBank(Content::GetPath() + "/sounds/media/" + "Master Bank.bank");
	std::string smasterBankString(Content::GetPath() + "/sounds/media/" + "Master Bank.strings.bank");
	std::string svehicleBank(Content::GetPath() + "/sounds/media/" + "Vehicles.bank");
	
	
	FMOD::Studio::Bank* masterBank = NULL;
	result = systemStudio->loadBankFile( smasterBank.c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &masterBank);
	
	FMOD::Studio::Bank* stringsBank = NULL;
	result = systemStudio->loadBankFile( smasterBankString.c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &stringsBank);
	
	FMOD::Studio::Bank* vehiclesBank = NULL;
	result = systemStudio->loadBankFile( svehicleBank.c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &vehiclesBank);
	
	FMOD::Studio::EventDescription* eventDescription = NULL;
	result = systemStudio->getEvent("event:/Vehicles/Basic Engine", &eventDescription);
	
	FMOD::Studio::EventInstance* eventInstance = NULL;
	result = eventDescription->createInstance(&eventInstance);
	
	FMOD::Studio::ParameterInstance* rpm = NULL;
	result = eventInstance->getParameter("RPM", &rpm);
	
	result = rpm->setValue(650);
	
	result = eventInstance->start();
	
	// Position the listener at the origin
	FMOD_3D_ATTRIBUTES attributes = { { 0 } };
	attributes.forward.z = 1.0f;
	attributes.up.y = 1.0f;
	systemStudio->setListenerAttributes(0, &attributes);
	
	// Position the event 2 units in front of the listener
	attributes.position.z = 2.0f;
	eventInstance->set3DAttributes(&attributes);

	result = systemLowLevel->createDSPByType( FMOD_DSP_TYPE_ECHO, &dspecho );
	if (result != FMOD_OK)
	{
		std::cout <<  "Error: FMOD did not create digital signal processor type echo" << std::endl;
	}
	result = dspecho->setParameterFloat(FMOD_DSP_ECHO_DELAY, 150.0f);
	result = dspecho->setParameterFloat(FMOD_DSP_ECHO_WETLEVEL, 150.0f);
	result = systemLowLevel->getMasterChannelGroup(&group);
	group->addDSP(FMOD_CHANNELCONTROL_DSP_HEAD, dspecho);
	if (result != FMOD_OK)
	{
		std::cout <<  "Error: FMOD did not add effect to channel" << std::endl;
	}
	
 
	//result = systemLowLevel->createSound( (Content::GetPath() + "/sounds/piano2.wav").c_str(), FMOD_DEFAULT, 0, &sound1);
	result = systemLowLevel->createStream( (Content::GetPath() + "/sounds/piano2.wav").c_str(), FMOD_DEFAULT, 0, &sound1);
	
	if (result == FMOD_ERR_FILE_NOTFOUND)
	{
		std::cout <<  "Error: FMOD did not find the file" << std::endl;
	}
	result = sound1->setMode(FMOD_LOOP_OFF);
	result = systemLowLevel->playSound(sound1, 0, false, &channel);
	
}

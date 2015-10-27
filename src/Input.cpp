#include "Input.hpp"

Input::Input()
{
    //MidiTest();
    for (int i = 0; i < SDL_NUM_SCANCODES; ++i)
    {
        keyState[i].triggerEvent = KeyEvent::NOTHING;
        keyState[i].usedInCurrentFrame = false;
    }
    
    for (int i = 0; i < 6; ++i)
    {
        mouseState[i].state = MouseEvent::NOTHING;
        mouseState[i].usedInCurrentFrame = false;
    }
    
    //bindings = new std::map< std::string, int >();
}

Input::~Input()
{
    //delete bindings;
    //delete midiin;
    //delete midiout;
}

const glm::ivec2 Input::GetMousePosition()
{
    return glm::ivec2(mousePosition.x, mousePosition.y);
}

bool Input::OnMouseDown(uint8_t button)
{
    return mouseState[button].state == MouseEvent::DOWN;
}

bool Input::OnMouseUp(uint8_t button)
{
    return mouseState[button].state == MouseEvent::RELEASE;
}

bool Input::OnKeyDown(SDL_Keycode key)
{
    SDL_Scancode scanKey = SDL_GetScancodeFromKey(key);
    return keyState[scanKey].triggerEvent == KeyEvent::DOWN;
}
/*
bool Input::OnKeyDown( std::string name )
{
    SDL_Scancode key = (SDL_Scancode)(*bindings)[name];
    return OnKeyDown(key);
}
*/
bool Input::OnKeyUp(SDL_Keycode key)
{
    SDL_Scancode scanKey = SDL_GetScancodeFromKey(key);
    return keyState[scanKey].triggerEvent == KeyEvent::RELEASE;
}
/*
bool Input::OnKeyUp( std::string name )
{
    SDL_Scancode key = (SDL_Scancode)(*bindings)[name];
    return OnKeyUp(key);
}
*/
bool Input::OnKey(SDL_Keycode key)
{
    SDL_Scancode scanKey = SDL_GetScancodeFromKey(key);
    return keyState[scanKey].triggerEvent == KeyEvent::HOLD;
}
/*
bool Input::OnKey( std::string name )
{
    SDL_Scancode key = (SDL_Scancode)(*bindings)[name];
    return OnKey(key);
}
*/
void Input::StateReset()
{
    for (int i = 0; i < SDL_NUM_SCANCODES; ++i)
    {
        if ( keyState[i].usedInCurrentFrame )
        {
            keyState[i].usedInCurrentFrame = false;
        }
        else
        {
            keyState[i].triggerEvent = KeyEvent::NOTHING;
        }
    }
    
    for (int i = 0; i < 6; ++i)
    {
        if ( mouseState[i].usedInCurrentFrame )
        {
            mouseState[i].usedInCurrentFrame = false;
        }
        else
        {
            mouseState[i].state = MouseEvent::NOTHING;
        }

    }
}
/*
void Input::BindKey( std::string name, int key )
{
    if ( name.empty() && key == SDL_SCANCODE_UNKNOWN)
    {
        return;
    }
    
    bindings->insert( std::pair<std::string, int>( name, key ) );
}

void Input::UnbindKey( std::string name )
{
    if (BindExists(name))
    {
        bindings->erase(name);
    }
}

bool Input::BindExists( std::string key )
{
    return ( bindings->find(key) != bindings->end() );
}

bool Input::IsBound( int key )
{
    if ( bindings->empty() )
    {
        return false;
    }

    for ( auto &it : *bindings )
    {
        if (it.second == key)
        {
            return true;
        }
    }
    return false;
}

int Input::GetBind( std::string name )
{
    // If #key is not binded to anything...
    if ( bindings->find( name ) == bindings->end() )
    {
        return SDL_SCANCODE_UNKNOWN;
    }
    return ( *bindings )[name];
}
 */

// TODO: Implement Modifier support, Motion getter, Wheel getter
// TODO: Support Keycode
void Input::Update( SDL_Event* event )
{
    //SDL_Keycode keyVCode = event->key.keysym.sym;
    SDL_Scancode keyCode = event->key.keysym.scancode;
    uint8_t mouseButton = event->button.button;
    
    mousePosition.x = event->button.x;
    mousePosition.y = event->button.y;
    
    
    switch (event->type)
    {
        case SDL_KEYDOWN:
        {
            if (event->key.repeat == 0)
            {
                keyState[keyCode].triggerEvent = KeyEvent::DOWN;
            }
            else
            {
                keyState[keyCode].triggerEvent = KeyEvent::HOLD;
            }
            keyState[keyCode].usedInCurrentFrame = true;
            break;
        }
        case SDL_KEYUP:
        {
            keyState[keyCode].triggerEvent = KeyEvent::RELEASE;
            keyState[keyCode].usedInCurrentFrame = true;
            break;
        }
        case SDL_MOUSEBUTTONDOWN:
        {
            mouseState[mouseButton].state = MouseEvent::DOWN;
            mouseState[mouseButton].usedInCurrentFrame = true;
            break;
        }
        case SDL_MOUSEBUTTONUP:
        {
            mouseState[mouseButton].state = MouseEvent::RELEASE;
            mouseState[mouseButton].usedInCurrentFrame = true;
            break;
        }
        case SDL_MOUSEMOTION:
        {
            mouseMotion.position = mousePosition;
            mouseMotion.relativeMotion = glm::ivec2( event->motion.xrel, event->motion.yrel );
            break;
        }
        case SDL_MOUSEWHEEL:
        {
            mouseWheel.horizontalScroll = event->wheel.x;
            mouseWheel.verticalScroll = event->wheel.y;
            break;
        }
        default:
            break;
    }
}

/*
 void Input::MidiTest()
 {
 midiin = new RtMidiIn();
 unsigned int nPortsIn = midiin->getPortCount();
 std::cout << "\nThere are " << nPortsIn << " MIDI input sources available." << std::endl;
 std::string portNameIn;
 for ( unsigned int i=0; i<nPortsIn; i++ )
 {
 portNameIn = midiin->getPortName(i);
 std::cout << "  Input Port #" << i+1 << ": " << portNameIn << std::endl;
 }
 
 midiout = new RtMidiOut();
 unsigned int nPortsOut = midiout->getPortCount();
 std::cout << "\nThere are " << nPortsOut << " MIDI output sources available." << std::endl;
 std::string portNameOut;
 for ( unsigned int i=0; i<nPortsOut; i++ )
 {
 portNameOut = midiin->getPortName(i);
 std::cout << "  Input Port #" << i+1 << ": " << portNameOut << std::endl;
 }
 
 midiin->openPort( 0 );
 }
 */

/*
 ** Experamental code for Midi controller
 void Input::MidiListener()
 {
 std::vector<unsigned char> message;
 
 double stamp = midiin->getMessage( &message );
 int nBytes = message.size();
 for (int i = 0; i < nBytes; i++ )
 {
 std::cout << "Byte " << i << " = " << (int)message[i] << ", ";
 }
 if ( nBytes > 0 )
 {
 std::cout << "stamp = " << stamp << std::endl;
 }
 
 }
 */

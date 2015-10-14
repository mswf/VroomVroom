#ifndef Input_h
#define Input_h

#include <map>
#include <string>
#include <SDL2/SDL_events.h>
#include "glm/vec2.hpp"
//#include "RtMidi.h"

class Input
{
    enum class KeyEvent
    {
        NOTHING = 0,
        DOWN,
        HOLD,
        RELEASE
    };
    
    enum class MouseEvent
    {
        NOTHING = 0,
        DOWN,
        RELEASE
    };
    
    struct KeyState
    {
        KeyEvent triggerEvent;
        bool usedInCurrentFrame;
    };
    
    struct MouseButtonState
    {
        MouseEvent state;
        bool usedInCurrentFrame;
    };

    
    struct MouseMotionState
    {
        glm::ivec2 position;
        glm::ivec2 relativeMotion;
    };
    
    struct MouseWheelState
    {
        int32_t horizontalScroll;
        int32_t verticalScroll;
    };
    
    
    public:
    
        Input();
        ~Input();
    
        void BindKey( std::string name, int key );
        void UnbindKey( std::string name );
        int GetBind( std::string name );
        bool BindExists( std::string name );
        bool IsBound( int key );
        bool OnKey( SDL_Scancode key );
        bool OnKey( std::string name );
        bool OnKeyDown( SDL_Scancode key );
        bool OnKeyDown( std::string name );
        bool OnKeyUp( SDL_Scancode key );
        bool OnKeyUp( std::string name );
        bool OnMouseDown( uint8_t button );
        bool OnMouseUp( uint8_t button );
        const glm::ivec2 GetMousePosition();
        void Update( SDL_Event* event );
        void StateReset();
        void MidiTest();
        void MidiListener();
    
    private:
    
        std::map< std::string, int > * bindings;
        KeyState keyState[SDL_NUM_SCANCODES];
        MouseButtonState mouseState[6];
        MouseMotionState mouseMotion;
        MouseWheelState mouseWheel;
        glm::ivec2 mousePosition;
        // Experamental code for midi controller
        // It works, btw
        //RtMidiIn*   midiin;
        //RtMidiOut*  midiout;
    
};


#endif /* Input_h */

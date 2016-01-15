#ifndef Input_h
#define Input_h

#include <map>
#include <string>
#include <SDL2/SDL_events.h>
#include "../glm/vec2.hpp"
#include "RtMidi.h"

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

		bool OnKey( SDL_Keycode key ) const;
		bool OnKeyDown( SDL_Keycode key ) const;
		bool OnKeyUp( SDL_Keycode key ) const;
		bool OnMouseDown( uint8_t button ) const;
		bool OnMouseUp( uint8_t button ) const;
		const glm::ivec2& GetMousePosition() const;
		const glm::ivec2 GetMouseWheelScroll() const;
		const glm::ivec2 GetMouseRelativeMotion() const;
		void Update( SDL_Event* event );
		void StateReset();
		void MidiTest();
		void MidiListener();

	private:

		KeyState keyState[SDL_NUM_SCANCODES];
		MouseButtonState mouseState[6];
		MouseMotionState mouseMotion;
		MouseWheelState mouseWheel;
		glm::ivec2 mousePosition;

		// Experamental code for midi controller
		RtMidiIn*   midiin;
		RtMidiOut*  midiout;
		std::vector<unsigned char> midiMessage;

};


#endif /* Input_h */

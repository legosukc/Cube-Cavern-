#ifndef _SDLCLASSES_INPUTHANDLER
#define _SDLCLASSES_INPUTHANDLER

#include <SDL2/SDL.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_stdinc.h>

#include "./SDLClasses/Window.hpp"

#include "./LuauClasses/BindableEvent.hpp"

static SDLClasses::Window* ActiveWindow;

namespace SDLClasses::InputHandler {

	SDL_FORCE_INLINE void SetTargetWindow(SDLClasses::Window* Window) {
		ActiveWindow = Window;
	}

	SDL_FORCE_INLINE bool IsKeyPressed(SDL_Scancode ScancodeKey) {
		return SDL_GetKeyboardState(NULL)[ScancodeKey] && ActiveWindow->HasKeyboardFocus;
	}

	SDL_FORCE_INLINE void AddInputDownConnection(void(*FunctionPtr)(void* Connector, SDL_Scancode Scancode), void* Connector, bool DisconnectAfterInvoke = false) {
		ActiveWindow->KeyPressedEvent.Connect(FunctionPtr, Connector, DisconnectAfterInvoke);
	}

	SDL_FORCE_INLINE void AddMouseButtonDownConnection(void(*FunctionPtr)(void* Connector, SDL_MouseButtonEvent* Scancode), void* Connector, bool DisconnectAfterInvoke = false) {
		ActiveWindow->MouseButtonPressedEvent.Connect(FunctionPtr, Connector, DisconnectAfterInvoke);
	}

	struct MouseInput {
		int x, y;
		bool LMBPressed, RMBPressed;
	};

	enum MouseButtonEnums {
		LeftMouse = 1,
		RightMouse = 2,
	};

	SDLClasses::InputHandler::MouseInput PollMouse() {

		if (not ActiveWindow->HasMouseFocus) {
			return SDLClasses::InputHandler::MouseInput{ 0, 0, false, false };
		}
		
		SDLClasses::InputHandler::MouseInput PolledMouseInput;
		const Uint32 MouseButtonMask = SDL_GetMouseState(&PolledMouseInput.x, &PolledMouseInput.y);

		PolledMouseInput.x -= ActiveWindow->Width / 2;
		PolledMouseInput.y -= ActiveWindow->Height / 2;
		
		PolledMouseInput.LMBPressed = SDL_BUTTON(MouseButtonMask) & SDL_BUTTON_LEFT;
		PolledMouseInput.RMBPressed = SDL_BUTTON(MouseButtonMask) & SDL_BUTTON_RIGHT;

		return PolledMouseInput;
	}

	void SetMousePosition(int x, int y) {

		if (ActiveWindow->HasMouseFocus and ActiveWindow->HasKeyboardFocus) {
			SDL_WarpMouseInWindow(ActiveWindow->SDLWindow, x + (ActiveWindow->Width / 2), y + (ActiveWindow->Height / 2));
		}
	}
	SDL_FORCE_INLINE void CenterMouse() {
		SDLClasses::InputHandler::SetMousePosition(0, 0);
	}

	SDL_FORCE_INLINE void Update() {

	}
}

#endif
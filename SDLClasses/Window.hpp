#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_error.h>

#include <glad/glad.h>

#include "FunctionHeaders/Error.hpp"

#include "LuauClasses/BindableEvent.hpp"

namespace SDLClasses {

	class Window {

		Uint32 LastTick;

	public:

		Window(const char* WindowTitle, int X, int Y, int Width, int Height, Uint32 Flags, float zNear = 0.01f, float zFar = 100.f) {

			this->SDLWindow = SDL_CreateWindow(WindowTitle, X, Y, Width, Height, SDL_WINDOW_OPENGL | Flags);
			if (this->SDLWindow == NULL) {
				FatalFunctionFailedError("SDL_CreateWindow", SDL_GetError());
			}

			this->GLContext = SDL_GL_CreateContext(this->SDLWindow);
			if (this->GLContext == NULL) {
				FatalFunctionFailedError("SDL_GL_CreateContext", SDL_GetError());
			}

			if (const int ErrorCode = SDL_GL_MakeCurrent(this->SDLWindow, this->GLContext); ErrorCode != 0) {
				
				// add ErrorCode to tis
				FatalFunctionFailedError("SDL_GL_MakeCurrent", SDL_GetError());
				/*
				std::cerr << ":ERROR: SDL_GL_MakeCurrent failure\nSDL Error: " << SDL_GetError()
					<< "\nError Code: " << ErrorCode
					<< "\nError recorded at " << __LINE__ << " in " << __FILE__ << std::endl;

				exit(EXIT_FAILURE);*/
			}
			
			this->Minimized = false;
			this->Visible = (Flags & SDL_WINDOW_HIDDEN) == 0;

			this->Width = Width;
			this->Height = Height;

			this->zNear = zNear;
			this->zFar = zFar;

			this->LastTick = SDL_GetTicks();
		}

		~Window() {
			SDL_DestroyWindow(this->SDLWindow);
			SDL_GL_DeleteContext(this->GLContext);
		}

		SDL_Window* SDLWindow;
		SDL_GLContext GLContext;
		
		bool Minimized, Visible, HasKeyboardFocus, HasMouseFocus;
		bool IsWindowAlive = true;
		int Width, Height;

		float zNear, zFar;

		double Delta60 = 1.0;
		Uint32 FPS = 60;

		LuauClasses::BindableEvent<SDLClasses::Window*> WindowResizedEvent;
		LuauClasses::BindableEvent<SDL_Scancode> KeyPressedEvent;

		LuauClasses::BindableEvent<SDL_MouseButtonEvent*> MouseButtonPressedEvent;

		void PollEvents() {

			SDL_Event Event;
			while (SDL_PollEvent(&Event)) {

				this->HasMouseFocus = SDL_GetMouseFocus() == this->SDLWindow;
				this->HasKeyboardFocus = SDL_GetKeyboardFocus() == this->SDLWindow;

				switch (Event.type) {

					case SDL_QUIT:
						this->IsWindowAlive = false;
						break;
					case SDL_WINDOWEVENT:

						switch (Event.window.type) {

							case SDL_WINDOWEVENT_MINIMIZED:
								this->Minimized = true;
								break;
							case SDL_WINDOWEVENT_MAXIMIZED:
								this->Minimized = false;
								break;

							case SDL_WINDOWEVENT_SHOWN:
								this->Minimized = false;
								break;
							/*
							case SDL_WINDOWEVENT_ENTER:
								this->HasMouseFocus = true;
								break;
							case SDL_WINDOWEVENT_LEAVE:
								this->HasMouseFocus = false;
								break;

							case SDL_WINDOWEVENT_FOCUS_GAINED:
								this->HasKeyboardFocus = true;
								break;
							case SDL_WINDOWEVENT_FOCUS_LOST:
								this->HasKeyboardFocus = false;
								break;*/

							case SDL_WINDOWEVENT_EXPOSED:


							case SDL_WINDOWEVENT_RESIZED:
								this->Width = Event.window.data1;
								this->Height = Event.window.data2;

								this->WindowResizedEvent.Invoke(this);
								break;

							case SDL_WINDOWEVENT_SIZE_CHANGED:
								SDL_GetWindowSize(this->SDLWindow, &this->Width, &this->Height);

								this->WindowResizedEvent.Invoke(this);
								break;
						}
						break;

					case SDL_KEYDOWN:
						this->KeyPressedEvent.Invoke(Event.key.keysym.scancode);
						break;

					case SDL_MOUSEBUTTONDOWN:
						this->MouseButtonPressedEvent.Invoke(&Event.button);
						break;
				}
			}
		}

		void RenderUpdateWindow() {

			const Uint32 Ticks = SDL_GetTicks();
			const Uint32 TickDelta = Ticks - this->LastTick;

			const Uint32 TargetMilliseconds = 1000u / this->FPS; //* (!this->HasMouseFocus * 4);

			if (TickDelta < TargetMilliseconds) {
				SDL_Delay(TargetMilliseconds - TickDelta);
			}

			this->Delta60 = std::max((double)TickDelta / (1000.0 / (double)this->FPS), 1.0);
			//this->Delta60 = SDL_max((double)TickDelta / (double)TargetMilliseconds, 1.0); //SDL_max((double)TickDelta / (1000.0 / (double)this->FPS), 0.0625);
			this->LastTick = Ticks;

			SDL_GL_SwapWindow(this->SDLWindow);
		}
	};
}
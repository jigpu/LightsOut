/**
 * Copyright © 2009, Localhost Labs, Jason Gerecke
 * 
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 * 
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 * 
 *   1. The origin of this software must not be misrepresented; you must not
 *      claim that you wrote the original software. If you use this software
 *      in a product, an acknowledgment in the product documentation would be
 *      appreciated but is not required.
 *   
 *   2. Altered source versions must be plainly marked as such, and must not be
 *      misrepresented as being the original software.
 *   
 *   3. This notice may not be removed or altered from any source
 *      distribution.
 */


#ifndef PC

#include <SDL/SDL.h>
#include <SDL/SDL_keysym.h>
#include <wiiuse/wpad.h>
#include "Wiimote.hpp"


Wiimote::Wiimote() {
	WPAD_Init();
}


void Renderer::eventOccured(SDL_Event* event) {
	switch (event->type) {
		case SDL_QUIT: {
			stop();
			break;
		}
	}
}


void Wiimote::run() {
	u32 laststate = 0;
	SDLKey key;
	while (runThread) {
		yield(25);
		
		WPAD_ScanPads();
		u32 state = WPAD_ButtonsDown(0);
		
		u32 press   = state & ~laststate;
		u32 release = ~state & laststate;
		
		laststate = state;
		
		SDL_Event event;
		
		if (press & WPAD_BUTTON_UP) {
			event.key = { SDL_KEYDOWN, SDL_PRESSED, SDLK_UP };
			SDL_PushEvent(event);
		}
		if (press & WPAD_BUTTON_DOWN) {
			event.key = { SDL_KEYDOWN, SDL_PRESSED, SDLK_DOWN };
			SDL_PushEvent(event);
		}
		if (press & WPAD_BUTTON_LEFT) {
			event.key = { SDL_KEYDOWN, SDL_PRESSED, SDLK_LEFT };
			SDL_PushEvent(event);
		}
		if (press & WPAD_BUTTON_RIGHT) {
			event.key = { SDL_KEYDOWN, SDL_PRESSED, SDLK_RIGHT };
			SDL_PushEvent(event);
		}
		if (press & WPAD_BUTTON_A) {
			event.key = { SDL_KEYDOWN, SDL_PRESSED, SDLK_a };
			SDL_PushEvent(event);
		}
		if (press & WPAD_BUTTON_B) {
			event.key = { SDL_KEYDOWN, SDL_PRESSED, SDLK_b };
			SDL_PushEvent(event);
		}
		if (press & WPAD_BUTTON_1) {
			event.key = { SDL_KEYDOWN, SDL_PRESSED, SDLK_1 };
			SDL_PushEvent(event);
		}
		if (press & WPAD_BUTTON_2) {
			event.key = { SDL_KEYDOWN, SDL_PRESSED, SDLK_2 };
			SDL_PushEvent(event);
		}
		if (press & WPAD_BUTTON_PLUS) {
			event.key = { SDL_KEYDOWN, SDL_PRESSED, SDLK_PLUS };
			SDL_PushEvent(event);
		}
		if (press & WPAD_BUTTON_MINUS) {
			event.key = { SDL_KEYDOWN, SDL_PRESSED, SDLK_MINUS };
			SDL_PushEvent(event);
		}
		if (press & WPAD_BUTTON_HOME) {
			event.key = { SDL_KEYDOWN, SDL_PRESSED, SDLK_HOME };
			SDL_PushEvent(event);
		}
		
		
		if (release & WPAD_BUTTON_UP) {
			event.key = { SDL_KEYUP, SDL_RELEASED, SDLK_UP };
			SDL_PushEvent(event);
		}
		if (release & WPAD_BUTTON_DOWN) {
			event.key = { SDL_KEYUP, SDL_RELEASED, SDLK_DOWN };
			SDL_PushEvent(event);
		}
		if (release & WPAD_BUTTON_LEFT) {
			event.key = { SDL_KEYUP, SDL_RELEASED, SDLK_LEFT };
			SDL_PushEvent(event);
		}
		if (release & WPAD_BUTTON_RIGHT) {
			event.key = { SDL_KEYUP, SDL_RELEASED, SDLK_RIGHT };
			SDL_PushEvent(event);
		}
		if (release & WPAD_BUTTON_A) {
			event.key = { SDL_KEYUP, SDL_RELEASED, SDLK_a };
			SDL_PushEvent(event);
		}
		if (release & WPAD_BUTTON_B) {
			event.key = { SDL_KEYUP, SDL_RELEASED, SDLK_b };
			SDL_PushEvent(event);
		}
		if (release & WPAD_BUTTON_1) {
			event.key = { SDL_KEYUP, SDL_RELEASED, SDLK_1 };
			SDL_PushEvent(event);
		}
		if (release & WPAD_BUTTON_2) {
			event.key = { SDL_KEYUP, SDL_RELEASED, SDLK_2 };
			SDL_PushEvent(event);
		}
		if (release & WPAD_BUTTON_PLUS) {
			event.key = { SDL_KEYUP, SDL_RELEASED, SDLK_PLUS };
			SDL_PushEvent(event);
		}
		if (release & WPAD_BUTTON_MINUS) {
			event.key = { SDL_KEYUP, SDL_RELEASED, SDLK_MINUS };
			SDL_PushEvent(event);
		}
		if (release & WPAD_BUTTON_HOME) {
			event.key = { SDL_KEYUP, SDL_RELEASED, SDLK_HOME };
			SDL_PushEvent(event);
		}
	}
}


#endif


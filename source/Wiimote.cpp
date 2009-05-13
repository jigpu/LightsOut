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


#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_keysym.h>
#include <wiiuse/wpad.h>
#include "EventPublisher.hpp"
#include "Wiimote.hpp"


Wiimote::Wiimote() {
	WPAD_Init();
}


void Wiimote::eventOccured(const SDL_Event* const event) {
	switch (event->type) {
		case SDL_QUIT: {
			stop();
			break;
		}
	}
}


void Wiimote::run() {
	EventPublisher::getInstance().addEventObserver(this);
	
	u32 lastDown = 0;
	while (runThread) {
		WPAD_ScanPads();
		u32 down    = WPAD_ButtonsHeld(0);
		u32 press   = ~lastDown & down;
		u32 release = lastDown & ~down;
		
		SDL_Event event;
		event.key.type   = SDL_KEYDOWN;
		event.key.state  = SDL_PRESSED;
		event.key.keysym.mod = KMOD_NONE;
		event.key.keysym.unicode = 0;
		event.key.keysym.scancode = 0;
		
		if (press != 0) {
			if (press & WPAD_BUTTON_UP) {
				event.key.keysym.sym = SDLK_UP;
				SDL_PushEvent(&event);
			}
			if (press & WPAD_BUTTON_DOWN) {
				event.key.keysym.sym = SDLK_DOWN;
				SDL_PushEvent(&event);
			}
			if (press & WPAD_BUTTON_LEFT) {
				event.key.keysym.sym = SDLK_LEFT;
				SDL_PushEvent(&event);
			}
			if (press & WPAD_BUTTON_RIGHT) {
				event.key.keysym.sym = SDLK_RIGHT;
				SDL_PushEvent(&event);
			}
			if (press & WPAD_BUTTON_A) {
				event.key.keysym.sym = SDLK_a;
				SDL_PushEvent(&event);
			}
			if (press & WPAD_BUTTON_B) {
				event.key.keysym.sym = SDLK_b;
				SDL_PushEvent(&event);
			}
			if (press & WPAD_BUTTON_1) {
				event.key.keysym.sym = SDLK_1;
				SDL_PushEvent(&event);
			}
			if (press & WPAD_BUTTON_2) {
				event.key.keysym.sym = SDLK_2;
				SDL_PushEvent(&event);
			}
			if (press & WPAD_BUTTON_PLUS) {
				event.key.keysym.sym = SDLK_PLUS;
				SDL_PushEvent(&event);
			}
			if (press & WPAD_BUTTON_MINUS) {
				event.key.keysym.sym = SDLK_MINUS;
				SDL_PushEvent(&event);
			}
			if (press & WPAD_BUTTON_HOME) {
				event.key.keysym.sym = SDLK_HOME;
				SDL_PushEvent(&event);
			}
		}

		
		event.key.type   = SDL_KEYUP;
		event.key.state  = SDL_RELEASED;
		if (release != 0) {
			if (release & WPAD_BUTTON_UP) {
				event.key.keysym.sym = SDLK_UP;
				SDL_PushEvent(&event);
			}
			if (release & WPAD_BUTTON_DOWN) {
				event.key.keysym.sym = SDLK_DOWN;
				SDL_PushEvent(&event);
			}
			if (release & WPAD_BUTTON_LEFT) {
				event.key.keysym.sym = SDLK_LEFT;
				SDL_PushEvent(&event);
			}
			if (release & WPAD_BUTTON_RIGHT) {
				event.key.keysym.sym = SDLK_RIGHT;
				SDL_PushEvent(&event);
			}
			if (release & WPAD_BUTTON_A) {
				event.key.keysym.sym = SDLK_a;
				SDL_PushEvent(&event);
			}
			if (release & WPAD_BUTTON_B) {
				event.key.keysym.sym = SDLK_b;
				SDL_PushEvent(&event);
			}
			if (release & WPAD_BUTTON_1) {
				event.key.keysym.sym = SDLK_1;
				SDL_PushEvent(&event);
			}
			if (release & WPAD_BUTTON_2) {
				event.key.keysym.sym = SDLK_2;
				SDL_PushEvent(&event);
			}
			if (release & WPAD_BUTTON_PLUS) {
				event.key.keysym.sym = SDLK_PLUS;
				SDL_PushEvent(&event);
			}
			if (release & WPAD_BUTTON_MINUS) {
				event.key.keysym.sym = SDLK_MINUS;
				SDL_PushEvent(&event);
			}
			if (release & WPAD_BUTTON_HOME) {
				event.key.keysym.sym = SDLK_HOME;
				SDL_PushEvent(&event);
			}
		}
		
		lastDown = down;
		yield(25);
	}
	
	EventPublisher::getInstance().removeEventObserver(this);
}


#endif


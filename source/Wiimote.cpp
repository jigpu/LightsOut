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


void Wiimote::run() {
	u32 laststate = 0;
	SDLKey key;
	while (true) {
		yield(25);
		
		WPAD_ScanPads();
		u32 state = WPAD_ButtonsDown(0);
		
		u32 press   = state & ~laststate;
		u32 release = ~state & laststate;
		
		laststate = state;
		
		if (press & WPAD_BUTTON_UP) {
			key = SDLK_UP;
			notifyObservers(SDL_KEYDOWN, &key);
		}
		if (press & WPAD_BUTTON_DOWN) {
			key = SDLK_DOWN;
			notifyObservers(SDL_KEYDOWN, &key);
		}
		if (press & WPAD_BUTTON_LEFT) {
			key = SDLK_LEFT;
			notifyObservers(SDL_KEYDOWN, &key);
		}
		if (press & WPAD_BUTTON_RIGHT) {
			key = SDLK_RIGHT;
			notifyObservers(SDL_KEYDOWN, &key);
		}
		if (press & WPAD_BUTTON_A) {
			key = SDLK_a;
			notifyObservers(SDL_KEYDOWN, &key);
		}
		if (press & WPAD_BUTTON_B) {
			key = SDLK_b;
			notifyObservers(SDL_KEYDOWN, &key);
		}
		if (press & WPAD_BUTTON_1) {
			key = SDLK_1;
			notifyObservers(SDL_KEYDOWN, &key);
		}
		if (press & WPAD_BUTTON_2) {
			key = SDLK_2;
			notifyObservers(SDL_KEYDOWN, &key);
		}
		if (press & WPAD_BUTTON_PLUS) {
			key = SDLK_PLUS;
			notifyObservers(SDL_KEYDOWN, &key);
		}
		if (press & WPAD_BUTTON_MINUS) {
			key = SDLK_MINUS;
			notifyObservers(SDL_KEYDOWN, &key);
		}
		if (press & WPAD_BUTTON_HOME) {
			key = SDLK_HOME;
			notifyObservers(SDL_KEYDOWN, &key);
		}
		
		
		if (release & WPAD_BUTTON_UP) {
			key = SDLK_UP;
			notifyObservers(SDL_KEYUP, &key);
		}
		if (release & WPAD_BUTTON_DOWN) {
			key = SDLK_DOWN;
			notifyObservers(SDL_KEYUP, &key);
		}
		if (release & WPAD_BUTTON_LEFT) {
			key = SDLK_LEFT;
			notifyObservers(SDL_KEYUP, &key);
		}
		if (release & WPAD_BUTTON_RIGHT) {
			key = SDLK_RIGHT;
			notifyObservers(SDL_KEYUP, &key);
		}
		if (release & WPAD_BUTTON_A) {
			key = SDLK_a;
			notifyObservers(SDL_KEYUP, &key);
		}
		if (release & WPAD_BUTTON_B) {
			key = SDLK_b;
			notifyObservers(SDL_KEYUP, &key);
		}
		if (release & WPAD_BUTTON_1) {
			key = SDLK_1;
			notifyObservers(SDL_KEYUP, &key);
		}
		if (release & WPAD_BUTTON_2) {
			key = SDLK_2;
			notifyObservers(SDL_KEYUP, &key);
		}
		if (release & WPAD_BUTTON_PLUS) {
			key = SDLK_PLUS;
			notifyObservers(SDL_KEYUP, &key);
		}
		if (release & WPAD_BUTTON_MINUS) {
			key = SDLK_MINUS;
			notifyObservers(SDL_KEYUP, &key);
		}
		if (release & WPAD_BUTTON_HOME) {
			key = SDLK_HOME;
			notifyObservers(SDL_KEYUP, &key);
		}
	}
}


#endif


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


#include <SDL/SDL.h>
#include <wiiuse/wpad.h>
#include "Wiimote.hpp"


Wiimote::Wiimote() {
	WPAD_Init();
}


void Wiimote::run() {
	u32 laststate = 0;
	while (true) {
		yield(25);
		
		WPAD_ScanPads();
		u32 state = WPAD_ButtonsDown(0);
		
		u32 press   = state & ~laststate;
		u32 release = ~state & laststate;
		
		laststate = state;
		
		if (press & WPAD_BUTTON_UP)
			notifyObservers(SDL_KEYDOWN, SDLK_UP);
		if (press & WPAD_BUTTON_DOWN)
			notifyObservers(SDL_KEYDOWN, SDLK_DOWN);
		if (press & WPAD_BUTTON_LEFT)
			notifyObservers(SDL_KEYDOWN, &SDLK_LEFT);
		if (press & WPAD_BUTTON_RIGHT)
			notifyObservers(SDL_KEYDOWN, &SDLK_RIGHT);
		if (press & WPAD_BUTTON_A)
			notifyObservers(SDL_KEYDOWN, new SDLK_a);
		if (press & WPAD_BUTTON_B)
			notifyObservers(SDL_KEYDOWN, new SDLK_b);
		if (press & WPAD_BUTTON_1)
			notifyObservers(SDL_KEYDOWN, new SDLK_1);
		if (press & WPAD_BUTTON_2)
			notifyObservers(SDL_KEYDOWN, new SDLK_2);
		if (press & WPAD_BUTTON_PLUS)
			notifyObservers(SDL_KEYDOWN, new SDLK_PLUS);
		if (press & WPAD_BUTTON_MINUS)
			notifyObservers(SDL_KEYDOWN, new SDLK_MINUS);
		if (press & WPAD_BUTTON_HOME)
			notifyObservers(SDL_KEYDOWN, new SDLK_HOME);
		
		
		if (release & WPAD_BUTTON_UP)
			notifyObservers(SDL_KEYUP, new SDLK_UP);
		if (release & WPAD_BUTTON_DOWN)
			notifyObservers(SDL_KEYUP, new SDLK_DOWN);
		if (release & WPAD_BUTTON_LEFT)
			notifyObservers(SDL_KEYUP, new SDLK_LEFT);
		if (release & WPAD_BUTTON_RIGHT)
			notifyObservers(SDL_KEYUP, new SDLK_RIGHT);
		if (release & WPAD_BUTTON_A)
			notifyObservers(SDL_KEYUP, new SDLK_a);
		if (release & WPAD_BUTTON_B)
			notifyObservers(SDL_KEYUP, new SDLK_b);
		if (release & WPAD_BUTTON_1)
			notifyObservers(SDL_KEYUP, new SDLK_1);
		if (release & WPAD_BUTTON_2)
			notifyObservers(SDL_KEYUP, new SDLK_2);
		if (release & WPAD_BUTTON_PLUS)
			notifyObservers(SDL_KEYUP, new SDLK_PLUS);
		if (release & WPAD_BUTTON_MINUS)
			notifyObservers(SDL_KEYUP, new SDLK_MINUS);
		if (release & WPAD_BUTTON_HOME)
			notifyObservers(SDL_KEYUP, new SDLK_HOME);
	}
}


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
			observer->controllerAction(SDL_KEYDOWN, SDLK_UP);
		if (press & WPAD_BUTTON_DOWN)
			observer->controllerAction(SDL_KEYDOWN, SDLK_DOWN);
		if (press & WPAD_BUTTON_LEFT)
			observer->controllerAction(SDL_KEYDOWN, &SDLK_LEFT);
		if (press & WPAD_BUTTON_RIGHT)
			observer->controllerAction(SDL_KEYDOWN, &SDLK_RIGHT);
		if (press & WPAD_BUTTON_A)
			observer->controllerAction(SDL_KEYDOWN, new SDLK_a);
		if (press & WPAD_BUTTON_B)
			observer->controllerAction(SDL_KEYDOWN, new SDLK_b);
		if (press & WPAD_BUTTON_1)
			observer->controllerAction(SDL_KEYDOWN, new SDLK_1);
		if (press & WPAD_BUTTON_2)
			observer->controllerAction(SDL_KEYDOWN, new SDLK_2);
		if (press & WPAD_BUTTON_PLUS)
			observer->controllerAction(SDL_KEYDOWN, new SDLK_PLUS);
		if (press & WPAD_BUTTON_MINUS)
			observer->controllerAction(SDL_KEYDOWN, new SDLK_MINUS);
		if (press & WPAD_BUTTON_HOME)
			observer->controllerAction(SDL_KEYDOWN, new SDLK_HOME);
		
		
		if (release & WPAD_BUTTON_UP)
			observer->controllerAction(SDL_KEYUP, new SDLK_UP);
		if (release & WPAD_BUTTON_DOWN)
			observer->controllerAction(SDL_KEYUP, new SDLK_DOWN);
		if (release & WPAD_BUTTON_LEFT)
			observer->controllerAction(SDL_KEYUP, new SDLK_LEFT);
		if (release & WPAD_BUTTON_RIGHT)
			observer->controllerAction(SDL_KEYUP, new SDLK_RIGHT);
		if (release & WPAD_BUTTON_A)
			observer->controllerAction(SDL_KEYUP, new SDLK_a);
		if (release & WPAD_BUTTON_B)
			observer->controllerAction(SDL_KEYUP, new SDLK_b);
		if (release & WPAD_BUTTON_1)
			observer->controllerAction(SDL_KEYUP, new SDLK_1);
		if (release & WPAD_BUTTON_2)
			observer->controllerAction(SDL_KEYUP, new SDLK_2);
		if (release & WPAD_BUTTON_PLUS)
			observer->controllerAction(SDL_KEYUP, new SDLK_PLUS);
		if (release & WPAD_BUTTON_MINUS)
			observer->controllerAction(SDL_KEYUP, new SDLK_MINUS);
		if (release & WPAD_BUTTON_HOME)
			observer->controllerAction(SDL_KEYUP, new SDLK_HOME);
	}
}


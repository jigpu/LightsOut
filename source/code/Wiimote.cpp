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
	//WPAD_Init();
	SDL_JoystickEventState(SDL_ENABLE);
}


void Wiimote::eventOccured(const SDL_Event* const event) {
	switch (event->type) {
		case SDL_USEREVENT:
			//std::clog << SDL_GetTicks() << " (" << this << "): Renderer gracefully stopping." << std::endl;
			stop();
			break;
		
		case SDL_QUIT:
			//std::clog << SDL_GetTicks() << " (" << this << "): Renderer quitting NOW." << std::endl;
			kill();
			break;
	}
}


void Wiimote::run() {
	EventPublisher::getInstance().addEventObserver(this);
	
	//while (runThread) {
		for(int i=0; i<SDL_NumJoysticks(); i++) {
			if (!SDL_JoystickOpened(i)) {
				//std::clog << SDL_GetTicks() << " (" << this << "): Opening new joystick '" << SDL_JoystickName(i) << "'";
				SDL_JoystickOpen(i);
			}
		}
		
		yield(1000);
	//}
	
	EventPublisher::getInstance().removeEventObserver(this);
}


#endif


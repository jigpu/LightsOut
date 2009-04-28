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


#include "Light.hpp"


Light::Light(int states) {
	this->states = states;
	state = 0;
	rect.x = 0;
	rect.y = 0;
}


int Light::getState() {
	return state;
}


void Light::nextState() {
	state++;
	if (state >= states)
		state = 0;
}


int Light::paint(SDL_Surface* surface) {
	rect.w = surface->w;
	rect.h = surface->h;
	
	switch (state) {
		case 0:	SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, COLOR_0)); break;
		case 1:	SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, COLOR_1)); break;
		case 2:	SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, COLOR_2)); break;
		case 3:	SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, COLOR_3)); break;
		default: SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, COLOR_UNK)); break;
	}
	
	return 0;
}


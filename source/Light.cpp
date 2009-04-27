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


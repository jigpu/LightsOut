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


#include <iostream>
#include <SDL/SDL_image.h>
#include <SDL/SDL_rotozoom.h>
#include "Light.hpp"


SDL_Surface* Light::glassTexture = NULL;


Light::Light(int states) {
	std::clog << SDL_GetTicks() << " (" << this << "): new Light." << std::endl;
	
	if (glassTexture == NULL) {
		glassTexture = IMG_Load("glass.png");
		if (glassTexture == NULL) {
			std::cerr << "Error loading glass.png: " << SDL_GetError() << std::endl;
			throw 1;
		}
	}
	
	this->states = states;
	state = 0;
	
	paintMutex = SDL_CreateMutex();
	
	dirty = true;
	surface = NULL;
}


Light::~Light() {
	std::clog << SDL_GetTicks() << " (" << this << "): delete light." << std::endl;
	
	SDL_DestroyMutex(paintMutex);
	SDL_FreeSurface(surface);
	//Do not free glassTexture on destruction since it is static
}


bool Light::isLightOn() {
	return state != 0;
}


void Light::nextState() {
	SDL_mutexP(paintMutex);
	state++;
	if (state >= states)
		state = 0;
	
	dirty = true;
	SDL_mutexV(paintMutex);
}


bool Light::paint(SDL_Surface& surface, int width, int height) {
	SDL_mutexP(paintMutex);
	
	if (dirty ||
	    this->surface == NULL ||
	    this->surface->w != width ||
	    this->surface->h != height) {
		SDL_FreeSurface(this->surface);
		this->surface = SDL_CreateRGBSurface(SDL_HWSURFACE,width,height,16,0,0,0,0);
		dirty = true;
	}	
	
	//Draw light onto surface
	///////////////////////////////////////////////////
	if (dirty) {		
		switch (state) {
			case 0:  SDL_FillRect(this->surface, NULL, SDL_MapRGB(this->surface->format, COLOR_0));   break;
			case 1:  SDL_FillRect(this->surface, NULL, SDL_MapRGB(this->surface->format, COLOR_1));   break;
			case 2:  SDL_FillRect(this->surface, NULL, SDL_MapRGB(this->surface->format, COLOR_2));   break;
			case 3:  SDL_FillRect(this->surface, NULL, SDL_MapRGB(this->surface->format, COLOR_3));   break;
			case 4:  SDL_FillRect(this->surface, NULL, SDL_MapRGB(this->surface->format, COLOR_4));   break;
			case 5:  SDL_FillRect(this->surface, NULL, SDL_MapRGB(this->surface->format, COLOR_5));   break;
			case 6:  SDL_FillRect(this->surface, NULL, SDL_MapRGB(this->surface->format, COLOR_6));   break;
			default: SDL_FillRect(this->surface, NULL, SDL_MapRGB(this->surface->format, COLOR_UNK)); break;
		}
		
		double widthPercent  = ((double)(this->surface->w))/((double)(glassTexture->w));
		double heightPercent = ((double)(this->surface->h))/((double)(glassTexture->h));
		SDL_Surface* zoom = rotozoomSurfaceXY(glassTexture, 0.0, widthPercent, heightPercent, 1);
		SDL_SetAlpha(zoom, SDL_SRCALPHA, 0);
		SDL_BlitSurface(zoom, NULL, this->surface, NULL);
		SDL_FreeSurface(zoom);
	}
	
	//Set surface and return
	///////////////////////////////////////////////////
	surface = *(this->surface);
	
	if (dirty) {
		dirty = false;
		SDL_mutexV(paintMutex);
		
		return true;
	}
	else {
		dirty = false;
		SDL_mutexV(paintMutex);
		
		return false;
	}
}


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


Light::Light(unsigned int states) {
	//std::clog << SDL_GetTicks() << " (" << this << "): new Light." << std::endl;
	
	if (glassTexture == NULL) {
		//glassTexture = IMG_Load("glass.png");
		FILE* file = fopen("glass.png", "r");
		glassTexture = IMG_Load_RW(SDL_RWFromFP(file, 0), 1);
		fclose(file);
		if (glassTexture == NULL) {
			std::cerr << "Error loading glass.png: " << SDL_GetError() << std::endl;
			throw 1;
		}
	}
	
	this->states = states;
	state = 0;
	presses = 0;
	
	paintMutex = SDL_CreateMutex();
	isDirty = true;
	parentRenderable = NULL;
	surfaceCache = NULL;
	
	uid = (rand()%256 << 16) | (rand()%256 << 8) | (rand()%256);
}


Light::~Light() {
	//std::clog << SDL_GetTicks() << " (" << this << "): delete Light." << std::endl;
	
	SDL_DestroyMutex(paintMutex);
	SDL_FreeSurface(surfaceCache);
	//Do not free glassTexture on destruction since it is static
}


bool Light::isLightOn() const {
	return state != 0;
}


void Light::nextState() {
	SDL_mutexP(paintMutex);
	state++;
	if (state >= states)
		state = 0;
	
	markDirty();
	SDL_mutexV(paintMutex);
}


bool Light::paint(SDL_Surface& surface, unsigned int width, unsigned int height, unsigned int type) const {
	SDL_mutexP(paintMutex);
	
	SDL_Surface* target = NULL;
	
	if (type == PAINT_NORMAL) {
		if (isDirty ||
		    this->surfaceCache == NULL ||
		    this->surfaceCache->w != width ||
		    this->surfaceCache->h != height) {
			SDL_FreeSurface(this->surfaceCache);
			this->surfaceCache = SDL_CreateRGBSurface(SDL_HWSURFACE,width,height,32,0,0,0,0);
			isDirty = true; //Don't markDirty() since this is a local phenomenon
		}
		target = this->surfaceCache;
	}
	else if (type == PAINT_UID) {
		target = SDL_CreateRGBSurface(SDL_SWSURFACE,width,height,32,0,0,0,0);
		uid = SDL_MapRGB(target->format, (Uint8)((uid & 0x00FF0000) >> 16), (Uint8)((uid & 0x0000FF00) >> 8), (Uint8)(uid & 0x000000FF));
		SDL_FillRect(target, NULL, uid);
	}
	
	//Draw light onto surface
	///////////////////////////////////////////////////
	if (isDirty && type == PAINT_NORMAL) {
		switch (state) {
			case 0:  SDL_FillRect(target, NULL, SDL_MapRGB(target->format, COLOR_0));   break;
			case 1:  SDL_FillRect(target, NULL, SDL_MapRGB(target->format, COLOR_1));   break;
			case 2:  SDL_FillRect(target, NULL, SDL_MapRGB(target->format, COLOR_2));   break;
			case 3:  SDL_FillRect(target, NULL, SDL_MapRGB(target->format, COLOR_3));   break;
			case 4:  SDL_FillRect(target, NULL, SDL_MapRGB(target->format, COLOR_4));   break;
			case 5:  SDL_FillRect(target, NULL, SDL_MapRGB(target->format, COLOR_5));   break;
			case 6:  SDL_FillRect(target, NULL, SDL_MapRGB(target->format, COLOR_6));   break;
			default: SDL_FillRect(target, NULL, SDL_MapRGB(target->format, COLOR_UNK)); break;
		}
		
		double widthPercent  = ((double)(target->w))/((double)(glassTexture->w));
		double heightPercent = ((double)(target->h))/((double)(glassTexture->h));
		SDL_Surface* zoom = rotozoomSurfaceXY(glassTexture, 0.0, widthPercent, heightPercent, 1);
		SDL_SetAlpha(zoom, SDL_SRCALPHA, 0);
		SDL_BlitSurface(zoom, NULL, target, NULL);
		SDL_FreeSurface(zoom);
	}
	
	//Set surface and return
	///////////////////////////////////////////////////
	surface = *target;
	
	bool dirt = false;
	switch (type) {
		case PAINT_NORMAL:
			dirt = isDirty;
			isDirty = false;
			break;
		
		case PAINT_UID:
			dirt = true;
			break;
	}
	
	SDL_mutexV(paintMutex);
	return dirt;
}


void Light::press() {
	SDL_mutexP(paintMutex);
	presses++;
	if (presses >= states)
		presses = 0;
	
	markDirty();
	SDL_mutexV(paintMutex);
}


bool Light::shouldPress() const {
	return presses != 0;
}


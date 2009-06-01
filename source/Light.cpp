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
	
	dirty = true;
	uid_dirty = true;
	surface = NULL;
	uid_surface = NULL;
	
	uid = (rand()%256 << 16) | (rand()%256 << 8) | (rand()%256);
}


Light::~Light() {
	//std::clog << SDL_GetTicks() << " (" << this << "): delete Light." << std::endl;
	
	SDL_DestroyMutex(paintMutex);
	SDL_FreeSurface(surface);
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
	
	dirty = true;
	SDL_mutexV(paintMutex);
}


bool Light::paint(SDL_Surface& surface, unsigned int width, unsigned int height, unsigned int type) const {
	SDL_mutexP(paintMutex);
	
	if (type == PAINT_NORMAL && (dirty ||
	    this->surface == NULL ||
	    this->surface->w != width ||
	    this->surface->h != height)) {
		SDL_FreeSurface(this->surface);
		this->surface = SDL_CreateRGBSurface(SDL_HWSURFACE,width,height,32,0,0,0,0);
		dirty = true;
	}
	
	if (type == PAINT_UID && (uid_dirty ||
	    this->uid_surface == NULL ||
	    this->uid_surface->w != width ||
	    this->uid_surface->h != height)) {
		SDL_FreeSurface(this->uid_surface);
		this->uid_surface = SDL_CreateRGBSurface(SDL_SWSURFACE,width,height,32,0,0,0,0);
		std::clog << "UID_b: " << (int)uid << std::endl;
		uid = SDL_MapRGB(this->uid_surface->format, (Uint8)((uid & 0x00FF0000) >> 16), (Uint8)((uid & 0x0000FF00) >> 8), (Uint8)(uid & 0x000000FF));
		std::clog << "UID_a: " << (int)uid << std::endl;
		SDL_FillRect(this->uid_surface, NULL, uid);
		Uint32 col = 0;
		memcpy ( &col , this->uid_surface->pixels , this->uid_surface->format->BytesPerPixel ) ;
		std::clog << "UID_o: " << (int)col << std::endl;
		uid_dirty = true;
	}
	
	SDL_Surface* target = NULL;
	switch (type) {
		case PAINT_NORMAL: target = this->surface; break;
		case PAINT_UID:    target = this->uid_surface; break;
	}
	
	//Draw light onto surface
	///////////////////////////////////////////////////
	if (dirty && type == PAINT_NORMAL) {
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
		
		double widthPercent  = ((double)(this->surface->w))/((double)(glassTexture->w));
		double heightPercent = ((double)(this->surface->h))/((double)(glassTexture->h));
		SDL_Surface* zoom = rotozoomSurfaceXY(glassTexture, 0.0, widthPercent, heightPercent, 1);
		SDL_SetAlpha(zoom, SDL_SRCALPHA, 0);
		SDL_BlitSurface(zoom, NULL, this->surface, NULL);
		SDL_FreeSurface(zoom);
	}
	
	//Set surface and return
	///////////////////////////////////////////////////
	surface = *target;
	
	bool dirt = false;
	switch (type) {
		case PAINT_NORMAL:
			dirt = dirty;
			dirty = false;
			break;
		
		case PAINT_UID:
			dirt = uid_dirty;
			uid_dirty = false;
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
	
	dirty = true;
	SDL_mutexV(paintMutex);
}


bool Light::shouldPress() const {
	return presses != 0;
}


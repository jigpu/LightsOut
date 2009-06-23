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
#include "EventPublisher.hpp"
#include "Cursor.hpp"


#if SDL_BYTEORDER == SDL_BIG_ENDIAN
#define rmask 0xff000000
#define gmask 0x00ff0000
#define bmask 0x0000ff00
#define amask 0x000000ff
#else
#define rmask 0x000000ff
#define gmask 0x0000ff00
#define bmask 0x00ff0000
#define amask 0xff000000
#endif


SDL_Surface* Cursor::pointerTexture = NULL;


Cursor::Cursor() {
	if (pointerTexture == NULL) {
		FILE* file = fopen("generic_point.png", "r");
		pointerTexture = IMG_Load_RW(SDL_RWFromFP(file, 0), 1);
		fclose(file);
		if (pointerTexture == NULL) {
			std::cerr << "Error loading generic_point.png: " << SDL_GetError() << std::endl;
			throw 1;
		}
		SDL_SetAlpha(pointerTexture, 0, 0);
	}
	
	paintMutex = SDL_CreateMutex();
	isDirty = true;
	parentRenderable = NULL;
	this->x = 0;
	this->y = 0;
	
	EventPublisher::getInstance().addEventObserver(this);
}


Cursor::~Cursor() {
	EventPublisher::getInstance().removeEventObserver(this);
	SDL_DestroyMutex(paintMutex);
	SDL_FreeSurface(pointerTexture);
}


void Cursor::eventOccured(const SDL_Event* const event) {
	switch (event->type) {
		case SDL_MOUSEMOTION: {
			//std::clog << SDL_GetTicks() << " (" << this << "): Cursor detected mouse movement." << std::endl;
			SDL_mutexP(paintMutex);
			this->x = event->motion.x;
			this->y = event->motion.y;
			markDirty();
			SDL_mutexV(paintMutex);
			/*
			SDL_Surface uid_buffer;
			bool dirty = child->paint(uid_buffer, surface->w, surface->h, PAINT_UID);
			if (dirty)
				std::clog << "uid_buffer was dirty!" << std::endl;
			
			bool lock = SDL_MUSTLOCK((&uid_buffer));
			if(lock) {
				if (SDL_LockSurface(&uid_buffer) != 0)
					std::cerr << SDL_GetTicks() << " (" << this << "): Unable to lock uid_buffer!" << std::endl;
			}
			
			Uint8* position = (Uint8*)uid_buffer.pixels;
			position += uid_buffer.pitch * event->motion.y;
			position += uid_buffer.format->BytesPerPixel * event->motion.x;
			memcpy(&mouseoverUID, position, uid_buffer.format->BytesPerPixel);
			
			if(lock) { SDL_UnlockSurface(&uid_buffer); }
			//std::clog << SDL_GetTicks() << " (" << this << "): Mouse over UID " << mouseoverUID << std::endl;
			break;
			*/
		}
	}
}


bool Cursor::paint(SDL_Surface& surface, unsigned int width, unsigned int height, unsigned int type) const {
	if (type != PAINT_NORMAL)
		return false;
	
	SDL_mutexP(paintMutex);
	
	//std::clog << SDL_GetTicks() << " (" << this << "): Cursor being painted." << std::endl;
	SDL_Surface* target = SDL_CreateRGBSurface(SDL_HWSURFACE, width, height, 32, rmask, gmask, bmask, amask);
	
	SDL_Rect dest;
	dest.x = this->x - 48;
	dest.y = this->y - 48;
	
	SDL_BlitSurface(pointerTexture, NULL, target, &dest);
	
	surface = *target;
	bool wasDirty = isDirty;
	isDirty = false;
	
	SDL_mutexV(paintMutex);
	return wasDirty;
}


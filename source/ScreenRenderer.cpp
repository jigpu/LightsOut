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
#include "EventPublisher.hpp"
#include "ScreenRenderer.hpp"


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


ScreenRenderer::ScreenRenderer(SDL_Surface* screenSurface) {
	this->screenSurface = screenSurface;
	paintMutex = SDL_CreateMutex();
	isDirty = true;
	parentRenderable = NULL;
	surfaceCache = NULL;
}


void ScreenRenderer::addChild(Renderable* const child) {
	SDL_mutexP(paintMutex);
	children.push_back(child);
	markDirty();
	SDL_mutexV(paintMutex);
}


void ScreenRenderer::eventOccured(const SDL_Event* const event) {
	switch (event->type) {
		case SDL_USEREVENT:
			//std::clog << SDL_GetTicks() << " (" << this << "): ScreenRenderer gracefully stopping." << std::endl;
			stop();
			break;
		
		case SDL_QUIT:
			//std::clog << SDL_GetTicks() << " (" << this << "): ScreenRenderer quitting NOW." << std::endl;
			kill();
			break;
	}
}


bool ScreenRenderer::paint(SDL_Surface& surface, unsigned int width, unsigned int height, unsigned int type) const {
	SDL_mutexP(paintMutex);
	
	if (isDirty ||
	    surfaceCache == NULL ||
	    surfaceCache->w != width ||
	    surfaceCache->h != height) {
		//std::clog << SDL_GetTicks() << " (" << this << "): ScreenRenderer painting due to dirt." << std::endl;
		isDirty = true; //Don't markDirty() since this is a local phenomenon
		SDL_FreeSurface(surfaceCache);
		surfaceCache = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height,32,0,0,0,0);
		//SDL_SetAlpha(surfaceCache, SDL_SRCALPHA, 128);
		
		std::list<Renderable*>::const_reverse_iterator riter;
		riter = children.rbegin();
		while (riter != children.rend()) {
			//std::clog << SDL_GetTicks() << " (" << this << "): ScreenRenderer painting a child." << std::endl;
			SDL_Surface layer;
			(*riter)->paint(layer, width, height, type);
			SDL_SetAlpha(&layer, SDL_SRCALPHA, 128);
			SDL_BlitSurface(&layer, NULL, surfaceCache, NULL);
			riter++;
		}
	}
	//std::clog << SDL_GetTicks() << " (" << this << "): ScreenRenderer returning painted surface." << std::endl;
	surface = *surfaceCache;
	bool wasDirty = isDirty;
	isDirty = false;
	
	SDL_mutexV(paintMutex);
	return wasDirty;
}


void ScreenRenderer::removeChild(Renderable* const child) {
	SDL_mutexP(paintMutex);
	children.remove(child);
	markDirty();
	SDL_mutexV(paintMutex);
}


void ScreenRenderer::run() {
	unsigned int currTime    = 0;
	unsigned int prevTime    = 0;
	unsigned int timeElapsed = 0;
	
	EventPublisher::getInstance().addEventObserver(this);
	
	//Frame-rate independent code based on
	//http://hdrlab.org.nz/frame-rate-independent-animation-using-sdl-and-opengl-with-frame-rate-limiting/
	while (runThread) {
		currTime = SDL_GetTicks();
		timeElapsed = currTime - prevTime;
		while (timeElapsed < MIN_FRAMETIME_MSECS && runThread) {
			yield(MIN_FRAMETIME_MSECS - timeElapsed);
			currTime = SDL_GetTicks();
			timeElapsed = currTime - prevTime;
		}
		prevTime = currTime;
		
		//std::clog << SDL_GetTicks() << " (" << this << "): ScreenRenderer checking if dirty paint needed." << std::endl;
		if (isDirty) {
			SDL_Surface buffer;
			bool dirtyPaint = paint(buffer, screenSurface->w, screenSurface->h);
			SDL_BlitSurface(&buffer, NULL, screenSurface, NULL);
			SDL_Flip(screenSurface);
			isDirty = false;
		}
	}
	
	EventPublisher::getInstance().removeEventObserver(this);
}


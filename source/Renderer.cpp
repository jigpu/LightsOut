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
#include "Renderable.hpp"
#include "Renderer.hpp"


Renderer::Renderer(SDL_Surface* surface, const Renderable* child) {
	this->surface = surface;
	this->child = child;
}


void Renderer::eventOccured(const SDL_Event* const event) {
	switch (event->type) {
		case SDL_MOUSEMOTION:
			//Locate and send the moused-over object an event
			mouseOver(event->motion.x, event->motion.y);
			break;
		
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


Uint32 Renderer::getPixel(SDL_Surface* pSurface, unsigned int x, unsigned int y) {
	//Code copied from http://www.gamedev.net/reference/programming/features/sdl2/page5.asp
	
	SDL_Color color ;
	Uint32 col = 0 ;
	
	char* pPosition = ( char* ) pSurface->pixels ;
	pPosition += ( pSurface->pitch * y ) ;
	pPosition += ( pSurface->format->BytesPerPixel * x ) ;
	
	memcpy ( &col , pPosition , pSurface->format->BytesPerPixel ) ;
	return col;
	//SDL_GetRGB ( col , pSurface->format , &color.r , &color.g , &color.b ) ;
	//return ( color ) ;
}


void Renderer::mouseOver(unsigned int x, unsigned int y) {
	//SDL_Surface* color_buffer = SDL_CreateRGBSurface(SDL_SWSURFACE,surface->w,surface->h,32,0,0,0,0);
	SDL_Surface color_buffer;
	child->paint(color_buffer, surface->w, surface->h, PAINT_UID);
	
	bool lock = SDL_MUSTLOCK((&color_buffer));
	if(lock) {
		if (SDL_LockSurface(&color_buffer) != 0)
			std::cerr << SDL_GetTicks() << " (" << this << "): Unable to lock color buffer!" << std::endl;
	}
	
	//SDL_Color color = getPixel(&color_buffer, x, y);
	//Uint32 uid = SDL_MapRGB(color_buffer.format, color.r, color.g, color.b);
	Uint32 uid = getPixel(&color_buffer, x, y);
	
	std::clog << SDL_GetTicks() << " (" << this << "): Sending UID: " << (int)uid << std::endl;
	SDL_Event mouseover;
	mouseover.user.type = SDL_USEREVENT + 1;
	mouseover.user.code = (int)uid;
	SDL_PushEvent(&mouseover);
	
	if(lock) { SDL_UnlockSurface(&color_buffer); }
}


void Renderer::run() {
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
		
		SDL_Surface buffer;
		bool dirtyPaint = child->paint(buffer, surface->w, surface->h);
		if (dirtyPaint) {
			//std::clog << "Blitting buffer to surface and flipping." << std::endl;
			SDL_BlitSurface(&buffer, NULL, surface, NULL);
			SDL_Flip(surface);
		}
	}
	
	EventPublisher::getInstance().removeEventObserver(this);
}


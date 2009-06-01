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


Uint32 Renderer::mouseoverUID = 0;


Renderer::Renderer(SDL_Surface* surface, const Renderable* child) {
	this->surface = surface;
	this->child = child;
}


void Renderer::eventOccured(const SDL_Event* const event) {
	switch (event->type) {
		case SDL_MOUSEMOTION: {
			SDL_Surface color_buffer;
			child->paint(color_buffer, surface->w, surface->h, PAINT_UID);
			
			bool lock = SDL_MUSTLOCK((&color_buffer));
			if(lock) {
				if (SDL_LockSurface(&color_buffer) != 0)
					std::cerr << SDL_GetTicks() << " (" << this << "): Unable to lock color buffer!" << std::endl;
			}
			
			Uint8* position = (Uint8*)color_buffer.pixels;
			position += color_buffer.pitch * event->motion.y;
			position += color_buffer.format->BytesPerPixel * event->motion.x;
			memcpy(&mouseoverUID, position, color_buffer.format->BytesPerPixel);
			
			if(lock) { SDL_UnlockSurface(&color_buffer); }
			//std::clog << SDL_GetTicks() << " (" << this << "): Mouse over UID " << mouseoverUID << std::endl;
			break;
		}
		
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


Uint32 Renderer::getMouseoverUID() {
	return mouseoverUID;
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


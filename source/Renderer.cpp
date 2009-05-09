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
#include "Renderer.hpp"


Renderer::Renderer(SDL_Surface* surface, Renderable* child) {
	this->surface = surface;
	this->child = child;
}


void Renderer::eventOccured(SDL_Event* event) {
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


void Renderer::run() {
	int currTime    = 0;
	int prevTime    = 0;
	int timeElapsed = 0;
	
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


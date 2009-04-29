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


#ifndef __Renderer_hpp__
#define __Renderer_hpp__


#include <SDL/SDL.h>
#include <vector>
#include "EventObserver.hpp"
#include "Renderable.hpp"
#include "Thread.hpp"


#define MIN_FRAMETIME_MSECS 32


/**
 * The Renderer is in charge of the SDL GUI. While it does not
 * initialize SDL, it should be started shortly afterward. It
 * periodically calls the paint method of its child Renderable
 * to ensure that the display is up to date.
 */
class Renderer : public Thread, public EventObserver {

protected:
	SDL_Surface* surface;
	
	Renderable* child;
	
public:
	Renderer(SDL_Surface* surface, Renderable* child);
	
	void eventOccured(SDL_Event* event);
	
	void run();
	
};


#endif


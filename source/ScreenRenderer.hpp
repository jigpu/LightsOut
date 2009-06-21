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


#ifndef __ScreenRenderer_hpp__
#define __ScreenRenderer_hpp__


#include <list>
#include <SDL/SDL.h>
#include "EventObserver.hpp"
#include "CachedRenderable.hpp"
#include "Thread.hpp"


#define MIN_FRAMETIME_MSECS 25


/**
 * The ScreenRenderer is a CachedRenderable that represents the
 * screen space available to the application. It contains a reference
 * to SDL's screen surface and periodically renders all children to
 * it.
 *
 * Top-level Renderable objects that need to appear on the screen
 * should add themselves as children.
 */
class ScreenRenderer : public CachedRenderable, public Thread, public EventObserver {

protected:	
	/**
	 * List of all child objects that are to be rendered.
	 * Children are rendered from last to first, to make having
	 * a Cursor child easier :)
	 */
	std::list<Renderable*> children;
	
	/**
	 * The surface of the screen as returned by SDL.
	 */
	SDL_Surface* screenSurface;
	
public:
	ScreenRenderer(SDL_Surface* screenSurface);
	
	void addChild(Renderable* const child);
	
	void eventOccured(const SDL_Event* const event);
	
	bool paint(SDL_Surface& surface,
	           unsigned int width,
	           unsigned int height,
	           unsigned int type = PAINT_NORMAL) const;
	
	void removeChild(Renderable* const child);
	
	void run();
	
};


#endif


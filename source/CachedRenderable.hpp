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


#ifndef __CachedRenderable_hpp__
#define __CachedRenderable_hpp__


#include <SDL/SDL.h>
#include "Renderable.hpp"


/**
 * A CachedRenderable is a Renderable which holds onto a surface
 * containing the last clean render of itself. If the object is not
 * dirty then rendering speed is greatly increased (only one blit
 * need occur, and no further painting of children is necessary).
 * 
 * CachedRenderable is an ideal superclass for objects which do not
 * get dirty often (whether by themselves or from their children).
 * Objects which are very often dirty may actually have slower render
 * times.
 */
class CachedRenderable : public Renderable {

protected:
	mutable SDL_Surface* surfaceCache;
	
};


#endif


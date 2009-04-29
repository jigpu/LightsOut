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


#ifndef __Renderable_hpp__
#define __Renderable_hpp__


#include <SDL/SDL.h>


/**
 * Renderable objects have the ability to paint themselves onto
 * an SDL surface. Many Renderable objects are "complex" objects
 * which are composed of other Renderables. In this case, a new
 * surface for each component should be created, and the result
 * blitted into the given surface.
 */
class Renderable {

protected:
	bool dirty;
	
	SDL_Surface* surface;
	
public:
	virtual int paint(SDL_Surface* surface) = 0;
	
};


#endif


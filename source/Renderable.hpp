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
	/**
	 * This boolean describes the state of the object itself.
	 * When a modification occurs, this should be made true so
	 * that the paint method knows that it needs to repaint the
	 * surface.
	 *
	 * This boolean should not depend on the dirtyness of
	 * sub-objects, though obviously something with dirty
	 * sub-objects will need to be re-painted.
	 */
	bool dirty;
	
	/**
	 * When painting a dirty object, paint operations should be
	 * sent to this object. This surface can then be blitted onto
	 * the surface that is given in the paint method.
	 *
	 * If an object and all sub-objects are clean, this surface
	 * can be blitted onto the surface given in the paint method
	 * without needing to perform any other drawing operations.
	 */
	SDL_Surface* surface;
	
public:
	/**
	 * The paint method will be called any time that the screen
	 * needs to be updated. The given surface should be painted
	 * to any time this method is called. Given this fact, it
	 * will be faster to use the surface above as a "clean"
	 * reference and blit it onto the given surface if this
	 * object and all sub-objects are clean.
	 */
	virtual int paint(SDL_Surface* surface) = 0;
	
};


#endif


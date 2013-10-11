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
	mutable bool dirty;
	
	/**
	 * When painting a dirty object, paint operations should be
	 * sent to this object. This surface can be passed back to
	 * the caller of the paint method if not dirty, or re-created
	 * and painted again before passing back.
	 */
	mutable SDL_Surface* surface;
	
public:
	/**
	 * The paint method will be called any time that the screen
	 * needs to be updated. The caller should pass in a NULL
	 * surface, allowing the child to create and pass back its
	 * own surface.
	 *
	 * Paint operations that were "dirty" should return true
	 * to let the caller know that it may want to update its
	 * own surface as well.
	 */
	virtual bool paint(SDL_Surface& surface, unsigned int width, unsigned int height) const = 0;
	
	virtual ~Renderable() { };
};


#endif


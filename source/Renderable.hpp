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


#define PAINT_NORMAL 0
#define PAINT_UID    1


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
	 *
	 * uid_dirty is similar, but should only be marked dirty if
	 * something causes a change in the uid_surface. This would
	 * be caused by an object being created/deleted/moved/etc.
	 */
	mutable bool dirty;
	mutable bool uid_dirty;
	
	/**
	 * When painting a dirty object, paint operations should be
	 * sent to this object. This surface can be passed back to
	 * the caller of the paint method if not dirty, or re-created
	 * and painted again before passing back.
	 *
	 * uid_surface is similar, but contains the result of calling
	 * paint with the PAINT_UID type. Just like the normal
	 * surface, this is cached to keep render times down.
	 */
	mutable SDL_Surface* surface;
	mutable SDL_Surface* uid_surface;
	
	/**
	 * A unique id that defines this object. As a color, this ID
	 * may be painted onto the screen. This makes object picking
	 * with the mouse extremely easy since the color under the
	 * mouse can be matched directly to an object.
	 *
	 * Ideally, this should not change, but if it does the
	 * uid_dirty flag MUST be set.
	 */
	mutable Uint32 uid;
	
public:
	/**
	 * Returns the UID color associated with this object. Objects
	 * may not necessarily know "where" they are, meaning their
	 * parents may have to perform the identification. Without
	 * access to the UIDs, this is impossible.
	 */
	Uint32 getUID(SDL_PixelFormat* fmt) { return SDL_MapRGB(fmt, (Uint8)(uid & 0x00FF0000 >> 16), (Uint8)(uid & 0x0000FF00 >> 8), (Uint8)(uid & 0x000000FF)); }
	
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
	virtual bool paint(SDL_Surface& surface, unsigned int width, unsigned int height, unsigned int type = PAINT_NORMAL) const = 0;
	
};


#endif


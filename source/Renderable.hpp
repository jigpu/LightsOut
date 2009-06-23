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
#include "Thread.hpp"


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
	 * This boolean is used to keep track of if the object has
	 * been modified since the last paint operation.
	 */
	mutable bool isDirty;
	
	/**
	 * Mutex to prevent the modification of internal state
	 * durring a paint operation (or vice-versa).
	 */
	SDL_mutex* paintMutex;
	
	/**
	 * A reference to the parent Renderable. Parents are notified
	 * when their children become dirty so that they may be
	 * marked dirty as well.
	 */
	Renderable* parentRenderable;
	
	/**
	 * A unique id that defines this object. This id can be
	 * interpreted as a 32-bit color and painted to the screen
	 * to ease object picking.
	 *
	 * Ideally this should not change, but if it does, the
	 * object MUST be marked dirty and the UID buffer must be
	 * redrawn.
	 */
	mutable Uint32 uid;
	
	/**
	 * Marks this object and all parents as being dirty. This
	 * should be called whenever a method changes the object's
	 * visual state.
	 *
	 * Note that this method does *NOT* lock paintMutex. It it
	 * did, a race condition would exist between the unlock call
	 * just after a dirtying operation and the call to this
	 * method.
	 */
	void markDirty() const {
		//std::clog << SDL_GetTicks() << " (" << this << "): I am being marked dirty!" << std::endl;
		isDirty = true;
		if (parentRenderable != NULL)
			parentRenderable->markDirty();
	}
	
public:
	/**
	 * Returns the UID associated with this renderable. The UID
	 * is used in object picking to determine exactly what the
	 * cursor is currently over.
	 */
	Uint32 getUID() const {
		return uid;
	}
	
	/**
	 * The paint method may be called at any time to obtain the
	 * most current rendering. The caller should pass in an empty
	 * surface object, NOT anything from SDL_CreateRGBSurface (or
	 * relatives). The callee is responsible for creating the
	 * actual surface and returning a reference to it via the
	 * surface argument.
	 * 
	 * If the paint was performed while the object was dirty,
	 * 'true' should be returned. Otherwise, return 'false'.
	 */
	virtual bool paint(SDL_Surface& surface,
	                   unsigned int width,
	                   unsigned int height,
	                   unsigned int type = PAINT_NORMAL) const = 0;
	
	/**
	 * Sets the parent renderable of the object. Since a newly-
	 * created renderable has no notion of what object is it
	 * "contained" in, this method should be called to ensure
	 * that changes in dirtyness can be propogated up the tree.
	 */
	void setParent(Renderable* parent) {
		SDL_mutexP(paintMutex);
		this->parentRenderable = parent;
		SDL_mutexV(paintMutex);
	}
	
};


#endif


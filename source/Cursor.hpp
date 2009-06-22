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


#ifndef __Cursor_hpp__
#define __Cursor_hpp__


#include "Renderable.hpp"
#include "EventObserver.hpp"


/**
 * A Cursor is a Renderable which is responsible for not only
 * creating a mouse cursor at the correct spot on screen; but
 * also for notifying other classes of movements, clicks, etc.
 */
class Cursor : public Renderable, public EventObserver {

protected:
	/**
	 * A static surface containing the pointer texture that
	 * represents the mouse.
	 */
	static SDL_Surface* pointerTexture;
	
	/**
	 * The most up-to-date coordinates of the cursor.
	 */
	int x, y;
	
public:
	Cursor();
	
	~Cursor();
	
	void eventOccured(const SDL_Event* const event);
	
	bool paint(SDL_Surface& surface,
	           unsigned int width,
	           unsigned int height,
	           unsigned int type = PAINT_NORMAL) const;
};


#endif


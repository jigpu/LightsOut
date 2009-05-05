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


#ifndef __ControllerObserver_hpp__
#define __ControllerObserver_hpp__


#include <SDL/SDL.h>


/**
 * An EventObserver is a class which is interested in recieving
 * SDL_Events. Threads in an application making use of the
 * EventPublisher should extend this class and register themselves to
 * recieve the SDL_QUIT event.
 */
class EventObserver {

public:
	/**
	 * Event observers must implement this method in order to
	 * recieve events. Events generated by SDL as well as events
	 * placed onto the queue by other classes will be pushed out
	 * to all observers from the EventPublisher by calling this
	 * method.
	 */
	virtual void eventOccured(SDL_Event* event) = 0;
	
};


#endif


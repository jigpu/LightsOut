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


#ifndef __Controller_hpp__
#define __Controller_hpp__


#include <list>
#include <SDL/SDL.h>
#include "ControllerObserver.hpp"
#include "Thread.hpp"


/**
 * Controllers are responsible for checking the input devices for a
 * change in status, and notifying all observers when necessary.
 * This abstraction is necessary desspite SDL for two reasons:
 * 
 *  * SDL does not support the Wiimote
 *  * SDL events disappear off the queue after read (...I think...)
 */
class Controller : public Thread {

protected:
	std::list<ControllerObserver*> observers;
	
	void notifyObservers(int type, SDLKey* value);
	
public:
	void addObserver(ControllerObserver* observer);
	
	void removeObserver(ControllerObserver* observer);
	
	virtual void run() = 0;
	
};


#endif


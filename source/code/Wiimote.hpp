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


#ifndef __Wiimote_hpp__
#define __Wiimote_hpp__


#include "EventObserver.hpp"
#include "Thread.hpp"


/**
 * A Wiimote is an abstraction of the Wii remote. Since SDL provides
 * no support for it, the run method polls the Wiimote itself,
 * pushing SDL_Events when necessary.
 */
class Wiimote : public Thread, public EventObserver {

public:
	Wiimote();
	
	void eventOccured(const SDL_Event* const event);
	
	/**
	 * The Wiimote thread periodically polls the state of the
	 * Wiimote and pushes events onto the SDL event queue when
	 * anything interesting happens.
	 */
	void run();
	
};


#endif


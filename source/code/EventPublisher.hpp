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


#ifndef __EventPublisher_hpp__
#define __EventPublisher_hpp__


#include <list>
#include <SDL/SDL.h>
#include "EventObserver.hpp"
#include "Thread.hpp"


/**
 * The EventPublisher is in charge of dispatching SDL_Events all
 * interested parties. This class makes the SDL event queue thread
 * friendly, since SDL_PollEvent removes events from the queue
 * (meaning that only one caller will ever get the message).
 *
 * EventPublisher is a singleton, since multiple publishers would end
 * up competing over the single event queue (which is precicely what
 * we wanted to avoid!).
 *
 * Although EventPublisher is a thread, it does not make sense to
 * have it watch itself for an SDL_QUIT event. Instead it checks each
 * event just after dispatching to see if it is one.
 *
 * This class replaces the old Controller class since it was
 * essentially just a specialized event publisher.
 */
class EventPublisher : public Thread {

protected:	
	/**
	 * This mutex protects the list of observers from concurrent
	 * access (which can easily cause a crash).
	 */
	SDL_mutex* listMutex;
	
	
	/**
	 * The list of registered observers.
	 */
	std::list<EventObserver*> observers;
	
	/**
	 * Protect the many ways to construct and destruct an object
	 * from being used by the public, since this is a singleton.
	 */
	EventPublisher();
	EventPublisher(EventPublisher const&);
	EventPublisher& operator=(EventPublisher const&);
	~EventPublisher();
	
	/**
	 * Notify all registered observers of the given SDL event.
	 */
	void notifyEventObservers(const SDL_Event* const event) const;
	
public:
	/**
	 * Register a new observer to recieve SDL events off the
	 * event queue.
	 */
	void addEventObserver(EventObserver* const observer);
	
	/**
	 * Provides access to the EventPublisher's single instance by
	 * a well-controlled single method.
	 */
	static EventPublisher& getInstance();
	
	/**
	 * Unregister an observer from the list of objects to recieve
	 * SDL events.
	 */
	void removeEventObserver(EventObserver* const observer);
	
	/**
	 * Periodically poll the SDL event queue for new events that
	 * need pushing out to observers.
	 */
	void run();
	
};


#endif


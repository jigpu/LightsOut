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


#include <iostream>
#include "EventPublisher.hpp"


EventPublisher::EventPublisher() {
	listMutex = SDL_CreateMutex();
}


EventPublisher::~EventPublisher() {
	SDL_DestroyMutex(listMutex);
}


void EventPublisher::addEventObserver(EventObserver* const observer) {
	//std::clog << SDL_GetTicks() << " (" << this << "): Adding new event observer (" << observer << ")" << std::endl;
	
	SDL_mutexP(listMutex);
	observers.push_back(observer);
	SDL_mutexV(listMutex);
}


EventPublisher& EventPublisher::getInstance() {
	static EventPublisher singleton;
	return singleton;
}


void EventPublisher::notifyEventObservers(const SDL_Event* const event) const {
	//std::clog << SDL_GetTicks() << " (" << this << "): Notifying observers of new event." << std::endl;
	
	SDL_mutexP(listMutex);
	std::list<EventObserver*>::const_iterator iter = observers.begin();
	while (iter != observers.end()) {
		(*iter)->eventOccured(event);
		iter++;
	}
	SDL_mutexV(listMutex);
}


void EventPublisher::removeEventObserver(EventObserver* const observer) {
	//std::clog << SDL_GetTicks() << " (" << this << "): Removing event observer (" << observer << ")" << std::endl;
	
	SDL_mutexP(listMutex);
	observers.remove(observer);
	SDL_mutexV(listMutex);
}


void EventPublisher::run() {
	SDL_Event event;
	
	while(runThread) {
		while(SDL_PollEvent(&event)) {
			notifyEventObservers(&event);
			
			if (event.type == SDL_USEREVENT) {
				//std::clog << SDL_GetTicks() << " (" << this << "): EventPublisher gracefully stopping." << std::endl;
				//std::clog << SDL_GetTicks() << " (" << this << "): Waiting for all observers to unregister..." << std::endl;
				while (observers.size() > 0)
					yield(100);
				return;
			}
			else if (event.type == SDL_QUIT) {
				//std::clog << SDL_GetTicks() << " (" << this << "): EventPublisher quitting NOW." << std::endl;
				return;
			}
		}
		
		yield(25);
	}
}


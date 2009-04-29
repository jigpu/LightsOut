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
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "EventPublisher.hpp"
#include "LightsOutGameManager.hpp"
#include <SDL/SDL_keysym.h>


LightsOutGameManager::LightsOutGameManager() {
	paintMutex = SDL_CreateMutex();
	
	newgame = false;
	gameover = false;
	
	surface = NULL;
	dirty = true;
	
	srand ( time(NULL) );
}


LightsOutGameManager::~LightsOutGameManager() {
	SDL_FreeSurface(surface);
	SDL_DestroyMutex(paintMutex);
}


void LightsOutGameManager::eventOccured(SDL_Event* event) {
	switch (event->type) {
		case SDL_KEYDOWN: {
			switch(event->key.keysym.sym) {
				case SDLK_ESCAPE:
				case SDLK_HOME:
					SDL_Event die;
					die.quit.type = SDL_QUIT;
					SDL_PushEvent(&die);
			}
			break;
		}
		
		case SDL_QUIT: {
			stop();
			break;
		} 
	}
}


void LightsOutGameManager::run() {
	EventPublisher::getInstance().addEventObserver(this);
	
	while (runThread) {
		SDL_mutexP(paintMutex);
		dirty = true;
		gameover = false;
		this->game = new LightsOutGame();
		SDL_mutexV(paintMutex);
		
		this->game->start();
		this->game->join();
	};
	
	std::cout << "Thanks for playing!" << std::endl;
	
	EventPublisher::getInstance().removeEventObserver(this);
}


int LightsOutGameManager::paint(SDL_Surface* surface) {
	SDL_mutexP(paintMutex);
	if (this->game == NULL) {
		std::cout << "No game yet..." << std::endl;
		return 1;
	}
	
	if (this->surface == NULL ||
	    this->surface->w != surface->w ||
	    this->surface->h != surface->h) {
		this->surface = SDL_CreateRGBSurface(surface->flags,surface->w,surface->h,16,0,0,0,0);
		dirty = true;
	}
	
	
	//Create subsurfaces and paint them
	bool dirtysub = false;
	SDL_Rect dest;
	
	dest.x = 8;
	dest.y = 8;
	dest.w  = 624;
	dest.h = 464;
	SDL_Surface* subsurface = SDL_CreateRGBSurface(surface->flags,dest.w,dest.h,16,0,0,0,0);
	if (game->paint(subsurface) == 0) dirtysub = true;
	SDL_BlitSurface(subsurface, NULL, this->surface, &dest);
	SDL_FreeSurface(subsurface);
	
	
	//Blit onto the target surface and return
	SDL_BlitSurface(this->surface, NULL, surface, NULL);
	if (dirty || dirtysub) {
		dirty = false;
		SDL_mutexV(paintMutex);
		return 0;
	}
	else {
		SDL_mutexV(paintMutex);
		return 1;
	}
}


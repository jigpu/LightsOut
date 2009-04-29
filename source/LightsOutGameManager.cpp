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
#include "Controller.hpp"
#include "LightsOutGameManager.hpp"


LightsOutGameManager::LightsOutGameManager(Controller* controller) {
	this->controller = controller;
	controller->addObserver(this);
	
	paintMutex = SDL_CreateMutex();
	
	surface = NULL;
	dirty = true;
	
	srand ( time(NULL) );
}


LightsOutGameManager::~LightsOutGameManager() {
	SDL_FreeSurface(surface);
	SDL_DestroyMutex(paintMutex);
}


void LightsOutGameManager::controllerAction(int type, SDLKey* value) {	
	switch (type) {
		case SDL_KEYDOWN: {
			switch(*value) {
				case SDLK_ESCAPE:
				case SDLK_HOME:   exit(0);
				default: break;
			}
		}
	}
}


void LightsOutGameManager::run() {
	do {
		SDL_mutexP(paintMutex);
		dirty = true;
		gameover = false;
		game = new LightsOutGame();
		controller->addObserver(game);
		SDL_mutexV(paintMutex);
		
		game->start();
		game->join();
		controller->removeObserver(game);
		
		std::cout << "\033[2J\033[1;1H" << "A winner is you!" << std::endl << "Play again? (Y/N) " << std::flush;
		
		do {
			yield(100);
		} while (!gameover);
	} while (newgame);
	
	std::cout << "Thanks for playing!" << std::endl;
}


int LightsOutGameManager::paint(SDL_Surface* surface) {
	SDL_mutexP(paintMutex);
	if (game == NULL)
		return 1;
	
	
	if (this->surface == NULL ||
	    this->surface->w != surface->w ||
	    this->surface->h != surface->h)
		this->surface = SDL_CreateRGBSurface(surface->flags,surface->w,surface->h,16,0,0,0,0);
	
	
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


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


using namespace std;


LightsOutGameManager::LightsOutGameManager(Controller* controller) {
	this->controller = controller;
	controller->addObserver(this);
	
	surface = NULL;
	srand ( time(NULL) );
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
		dirty = true;
		gameover = false;
		game = new LightsOutGame();
		controller->addObserver(game);
		
		while (!game->winningState()) {
			yield(100);
		}
		
		controller->removeObserver(game);
		
		cout << "\033[2J\033[1;1H" << "A winner is you!" << endl << "Play again? (Y/N) " << flush;
		
		do {
			yield(100);
		} while (!gameover);
	} while (newgame);
	
	cout << "Thanks for playing!" << endl;
}


int LightsOutGameManager::paint(SDL_Surface* surface) {
	if (game == NULL)
		return 1;
	
	
	if (this->surface == NULL ||
	    this->surface->w != surface->w ||
	    this->surface->h != surface->h)
		this->surface = SDL_CreateRGBSurface(surface->flags,surface->w,surface->h,16,0,0,0,0);
	
	
	//Create subsurfaces and paint them
	bool dirtysub = false;
	SDL_Rect dest;
	
	dest.x = 16;
	dest.y = 16;
	dest.w  = 448;
	dest.h = 448;
	SDL_Surface* subsurface = SDL_CreateRGBSurface(surface->flags,dest.w,dest.h,16,0,0,0,0);
	if (game->paint(subsurface) == 0) dirtysub = true;
	SDL_BlitSurface(subsurface, NULL, this->surface, &dest);
	SDL_FreeSurface(subsurface);
	
	
	//Paint this object itself if dirty, or if underlying
	//objects were dirty
	if (dirty || dirtysub) {
	}
	
	
	//Blit onto the target surface and return
	SDL_BlitSurface(this->surface, NULL, surface, NULL);
	if (dirty || dirtysub) {
		dirty = false;
		return 0;
	}
	else {
		return 1;
	}
}


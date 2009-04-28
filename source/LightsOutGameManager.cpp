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
	
	return game->paint(surface);
}


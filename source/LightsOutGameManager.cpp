#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <time.h>
//#include <wiiuse/wpad.h>
#include "Controller.hpp"
#include "LightsOutGameManager.hpp"


using namespace std;


LightsOutGameManager::LightsOutGameManager() {
	srand ( time(NULL) );
}


void LightsOutGameManager::controllerAction(int type, SDLKey* value) {
	//cout << "Recieved button " << value << endl;
	
	switch (type) {
		/*case WIIMOTE_BUTTON: {
			if (value & WPAD_BUTTON_HOME)
				exit(0);
			if (value & WPAD_BUTTON_UP)
				move(0, -1);
			if (value & WPAD_BUTTON_DOWN)
				move(0, 1);
			if (value & WPAD_BUTTON_LEFT)
				move(-1, 0);
			if (value & WPAD_BUTTON_RIGHT)
				move(1, 0);
			if (value & WPAD_BUTTON_A)
				select();
			if (value & WPAD_BUTTON_B) {
				game->getMoveHint(&x, &y);
				select();
			}
			break;
		}*/
		case SDL_KEYDOWN: {
			switch(*value) {
				case SDLK_ESCAPE: exit(0);
				case SDLK_UP:     move( 0,-1); break;
				case SDLK_DOWN:   move( 0, 1); break;
				case SDLK_LEFT:   move(-1, 0); break;
				case SDLK_RIGHT:  move( 1, 0); break;
				case SDLK_TAB:    game->getMoveHint(&x, &y);
				case SDLK_RETURN: select(); break;
			}
		}
	}
}


void LightsOutGameManager::select() {
	game->pressButton(x,y);
	move(0,0);
}


void LightsOutGameManager::move(int deltaX, int deltaY) {
	x += deltaX;
	y += deltaY;
	
	if (x < 0) x = 0;
	if (x >= game->getWidth()) x = game->getWidth()-1;
	if (y < 0) y = 0;
	if (y >= game->getHeight()) y = game->getHeight()-1;
	
	dirty = true;
}


void LightsOutGameManager::run() {
	do {
		x = 0;
		y = 0;
		dirty = false;
		gameover = false;
		game = new LightsOutGame();
		move(0,0);
		
		while (!game->winningState()) {
			yield(100);
		}
		
		cout << "\033[2J\033[1;1H" << "A winner is you!" << endl << "Play again? (Y/N) " << flush;
		
		do {
			yield(100);
		} while (!gameover);
	} while (newgame);
	
	cout << "Thanks for playing!" << endl;
}


int LightsOutGameManager::paint(SDL_Surface* surface) {
	if (!dirty)
		return 1;
	
	if (game == NULL)
		return 1;
	
	game->paint(surface);
	dirty = false;
	return 0;
}


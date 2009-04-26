#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <wiiuse/wpad.h>
#include "Controller.hpp"
#include "LightsOutGameManager.hpp"


using namespace std;


LightsOutGameManager::LightsOutGameManager() {
	srand ( time(NULL) );
}


void LightsOutGameManager::controllerAction(int type, int value) {
	//cout << "Recieved button " << value << endl;
	
	switch (type) {
		case WIIMOTE_BUTTON: {
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
		}/*
		case KEYBOARD_SINGLE: {
			if (value == 27)
				exit(0);
			else if (value == 87 || value == 119)
				move(0, -1);
			else if (value == 83 || value == 115)
				move(0, 1);
			else if (value == 65 || value == 97)
				move(-1, 0);
			else if (value == 68 || value == 100)
				move(1, 0);
			else if (value == 10)
				select();
			
			//Only when game has ended and we're looking
			//for a y/n response from the player
			if ((value == (int)'Y' || value == (int)'y') && game->winningState()) {
				newgame = true;	
				gameover = true;
			}
			if ((value == (int)'N' || value == (int)'n') && game->winningState()) {
				newgame = false;
				gameover = true;
			}
			
			break;
		}
		case KEYBOARD_DOUBLE: {
			if (value == 0x48)
				move(0, -1);
			else if (value == 0x50)
				move(0, 1);
			else if (value == 0x4B)
				move(0, -1);
			else if (value == 0x4D)
				move(0, 1);
			break;
		}*/
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
	
	cout << "\033[2J\033[1;1H"; //Clear screen
	game->paint();
	cout << endl << "Position: (" << (char)(x+65) << "," << y << ")";
	//cout << "\033[" << y+2 << ";" << x*2+3 << "f\033[7m";
	//cout << "\033[0m" << flush;
}


void LightsOutGameManager::run() {
	do {
		x = 0;
		y = 0;
		gameover = false;
		game = new LightsOutGame();
		move(0,0);
		
		while (!game->winningState()) {
			//usleep(100000);
			LWP_YieldThread();
		}
		
		cout << "\033[2J\033[1;1H" << "A winner is you!" << endl << "Play again? (Y/N) " << flush;
		
		do {
			//usleep(100000);
			LWP_YieldThread();
		} while (!gameover);
	} while (newgame);
	
	cout << "Thanks for playing!" << endl;
}


#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <wiiuse/wpad.h>
#include "LightsOutGameManager.hpp"


using namespace std;


LightsOutGameManager::LightsOutGameManager() {
	srand ( time(NULL) );
}


void LightsOutGameManager::run() {
	char input;
	bool success;
	cout << endl;
	do {
		game = new LightsOutGame();
		while (!game->winningState()) {
			game->paint();
			
			int x = -1;
			int y = -1;
			cout << endl;
			do {
				cout << "Coordinates: ";
				
				do {
					u32 pressed;
					do { WPAD_ScanPads(); pressed = WPAD_ButtonsDown(0); }
					while (pressed == 0);
					
					if ( pressed & WPAD_BUTTON_HOME )
						exit(0);
					if (pressed & WPAD_BUTTON_UP)
						y = 0;
					if (pressed & WPAD_BUTTON_DOWN)
						y = 1;
					if (pressed & WPAD_BUTTON_LEFT)
						y = 2;
					if (pressed & WPAD_BUTTON_RIGHT)
						y = 3;
					if (pressed & WPAD_BUTTON_A)
						y = 4;
					if (pressed & WPAD_BUTTON_MINUS)
						x = 0;
					if (pressed & WPAD_BUTTON_PLUS)
						x = 1;
					if (pressed & WPAD_BUTTON_1)
						x = 2;
					if (pressed & WPAD_BUTTON_2)
						x = 3;
					if (pressed & WPAD_BUTTON_B)
						x = 4;
					
				} while (x == -1 || y == -1);
				success = true;
				
			} while (!success);
			cout << endl;
			
			//game->getMoveHint(&x,&y);
			//cout << (char)('A'+x) << " " << y << endl;
			
			game->pressButton(x,y);
		}
		
		cout << "A winner is you!" << endl;
		
		do {
			cout << "Play again? (Y/N) " << endl;
			success = (cin >> input);
			if (!success) {
				cin.clear();
				cin.ignore(256,'\n');
				cout << "Invalid input." << endl;
			}
		} while (input != 'Y' && input != 'y' &&
		         input != 'N' && input != 'n');
	} while (input == 'Y' || input == 'y');
	
	cout << "Thanks for playing!" << endl;
}


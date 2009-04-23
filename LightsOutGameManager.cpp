#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "LightsOutGameManager.hpp"


using namespace std;


LightsOutGameManager::LightsOutGameManager() {
	srand ( time(NULL) );
}


void LightsOutGameManager::run() {
	char input;
	bool success;
	int x,y;
	do {
		game = new LightsOutGame();
		while (!game->winningState()) {
			game->paint();
			
			cout << endl;
			/*do {
				cout << "X Y: ";
				success = (cin >> x >> y);
				if (!success) {
					cin.clear();
					cin.ignore(256,'\n');
					cout << "Invalid input." << endl;
				}
				
			} while (!success);
			cout << endl;*/

			game->getMoveHint(&x,&y);
			cout << x << "," << y << endl;
			
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


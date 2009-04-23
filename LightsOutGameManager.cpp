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
			do {
				char* input;
				cout << "Coordinates: ";
				success = (cin >> input);
				if (!success) {
					cin.clear();
					cin.ignore(256,'\n');
					cout << "Invalid input." << endl;
				} else {
					x = -1;
					y = -1;
					
					if (input[0] >= 'A' && input[0] <= 'Z')
						x = input[0]-'A';
					else if (input[0] >= 'a' && input[0] <= 'z')
						x = input[0]-'a';
					else if (input[0] >= '0' && input[0] <= '9')
						y = input[0]-'0';
					
					if (input[1] >= 'A' && input[1] <= 'Z')
						x = input[1]-'A';
					else if (input[1] >= 'a' && input[1] <= 'z')
						x = input[1]-'a';
					else if (input[1] >= '0' && input[1] <= '9')
						y = input[1]-'0';
					
					if (x < 0 || y < 0 || x >= game->getWidth() || y >= game->getHeight()) {
						cout << "Invalid move." << endl << endl;
						success = false;
					}
				}
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


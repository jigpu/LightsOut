#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "LightsOutGameManager.hpp"


LightsOutGameManager::LightsOutGameManager() {
	srand ( time(NULL) );
}


void LightsOutGameManager::run() {
	char input;
	do {
		game = new LightsOutGame();
		while (!game->winningState()) {
			game->paint();
			printf("\nX,Y [0,%d]: ", 5);
			
			int x, y;
			//scanf("%d,%d", &x, &y);
			game->getMoveHint(&x,&y);
			printf("%d,%d\n", x, y);
			
			game->pressButton(x,y);
		}
		
		printf("A winner is you!\n");
		
		do {
			printf("Play again? (Y/N) ");
			scanf("%c", &input);
		} while (input != 'Y' && input != 'y' &&
		         input != 'N' && input != 'n');
	} while (input == 'Y' || input == 'y');
	printf("Thanks for playing!\n");
}


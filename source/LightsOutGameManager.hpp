#ifndef __LightsOutGameManager_hpp__
#define __LightsOutGameManager_hpp__


#include <SDL/SDL.h>
#include "ControllerObserver.hpp"
#include "Thread.hpp"
#include "LightsOutGame.hpp"
#include "LightsOutView.hpp"


/**
 * A simple manager for the multiple games of Lights Out. The manager
 * dictates how multiple games are played: how to choose players, when
 * to end the set of games, keeping track of scores across the set of
 * games, etc.
 */

class LightsOutGameManager : public ControllerObserver, public Thread {

protected:
	LightsOutGame* game;
	LightsOutView* view;
	SDL_Surface* screen;
	int x, y;
	bool newgame, gameover;
	
	void move(int deltaX, int deltaY);
	
	void select();

public:	
	LightsOutGameManager(SDL_Surface* screen);
	
	void controllerAction(int type, int value);
	
	void run();
	
};


#endif


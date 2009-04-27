#ifndef __LightsOutGameManager_hpp__
#define __LightsOutGameManager_hpp__


#include "ControllerObserver.hpp"
#include "Thread.hpp"
#include "LightsOutGame.hpp"
#include "LightsOutView.hpp"
#include "Renderable.hpp"


/**
 * A simple manager for the multiple games of Lights Out. The manager
 * dictates how multiple games are played: how to choose players, when
 * to end the set of games, keeping track of scores across the set of
 * games, etc.
 */

class LightsOutGameManager : public ControllerObserver, public Thread, public Renderable {

protected:
	LightsOutGame* game;
	int x, y;
	bool newgame, gameover, dirty;
	
	void move(int deltaX, int deltaY);
	
	void select();

public:	
	LightsOutGameManager();
	
	void controllerAction(int type, SDLKey* value);
	
	void run();
	
	int paint(SDL_Surface* surface);
};


#endif


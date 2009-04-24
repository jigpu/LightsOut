#ifndef __LightsOutGameManager_hpp__
#define __LightsOutGameManager_hpp__


#include "LightsOutGame.hpp"


/**
 * A simple manager for the multiple games of Lights Out. The manager
 * dictates how multiple games are played: how to choose players, when
 * to end the set of games, keeping track of scores across the set of
 * games, etc.
 */

class LightsOutGameManager {

protected:
	LightsOutGame* game;


public:	
	LightsOutGameManager();
	
	void run();
	
};


#endif


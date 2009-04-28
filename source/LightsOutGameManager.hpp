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


#ifndef __LightsOutGameManager_hpp__
#define __LightsOutGameManager_hpp__


#include "ControllerObserver.hpp"
#include "Thread.hpp"
#include "LightsOutGame.hpp"
#include "Renderable.hpp"


/**
 * A simple manager for the multiple games of Lights Out. The manager
 * dictates how multiple games are played: how to choose players, when
 * to end the set of games, keeping track of scores across the set of
 * games, etc.
 */
class LightsOutGameManager : public ControllerObserver, public Thread, public Renderable {

protected:
	Controller* controller;
	
	LightsOutGame* game;
	
	bool newgame, gameover;
	
public:
	LightsOutGameManager(Controller* controller);
	
	void controllerAction(int type, SDLKey* value);
	
	int paint(SDL_Surface* surface);
	
	void run();
	
};


#endif

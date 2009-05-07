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


#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>
#include "EventObserver.hpp"
#include "Thread.hpp"
#include "LightsOutGame.hpp"
#include "Renderable.hpp"


/**
 * A simple manager for the multiple games of Lights Out. The manager
 * dictates how multiple games are played: how to choose players,
 * when to end the set of games, keeping track of scores across the
 * set of games, etc.
 */
class LightsOutGameManager : public EventObserver, public Thread, public Renderable {

protected:
	/**
	 * A static font used for display of game statistics on the
	 * screen. DO NOT CLOSE THIS FONT! AFTER CREATION IT SHOULD
	 * REMAIN IN MEMORY FOR OTHER LIGHTSOUTGAMES.
	 */
	static TTF_Font* font;
	
	/**
	 * Mutex to prevent the modification of internal state
	 * durring a paint operation (or vice-versa).
	 */
	SDL_mutex* paintMutex;
	
	Mix_Music* music;
	
	/**
	 * The currently active game of Lights Out.
	 */
	LightsOutGame* game;
	
	/**
	 * The level of play that new games should be created at,
	 * number of games that have been completed, and time that
	 * the manager was started (for an idea of overall play time)
	 */
	int level, gamesCompleted, managerStartTime;
	
	/**
	 * newGame should be set to true when the manager should stop
	 * the game in progress and create a new game. This can be
	 * due to a reset request, a level change, etc.
	 */
	bool newGame;
	
public:
	LightsOutGameManager();
	
	~LightsOutGameManager();
	
	void eventOccured(SDL_Event* event);
	
	bool paint(SDL_Surface& surface, int width, int height);
	
	/**
	 * Runs the manager, which creates new games for the player
	 * until the program terminates.
	 */
	void run();
	
};


#endif


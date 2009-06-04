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


#ifndef __LightsOutGame_hpp__
#define __LightsOutGame_hpp__


#include <SDL/SDL_ttf.h>
#include "EventObserver.hpp"
#include "Light.hpp"
#include "RectangleMap.tpp"
#include "Renderable.hpp"
#include "Thread.hpp"
#include "Tile.tpp"


/**
 * A LightsOutGame allows for a single "play" of the game of Lights
 * Out. After creating the game, just start (and optionally join) the
 * thread. When the game ends, the thread will exit. Simply create a
 * new LightsOutGame if additional rounds of play are requested.
 */
class LightsOutGame : public EventObserver, public Renderable, public Thread {

protected:
	/**
	 * A static font used for display of game statistics on the
	 * screen. DO NOT CLOSE THIS FONT! AFTER CREATION IT SHOULD
	 * REMAIN IN MEMORY FOR OTHER LIGHTSOUTGAMES.
	 */
	static TTF_Font* font;
	
	/**
	 * A static surface containing the cursor which the user uses
	 * to select a light. DO NOT FREE THIS SURFACE! AFTER
	 * CREATION IT SHOULD REMAIN IN MEMORY FOR OTHER
	 * LIGHTSOUTGAMES.
	 */
	static SDL_Surface* cursorTexture;
	
	/**
	 * X and Y contain the location of the cursor.
	 */
	unsigned int x, y;
	
	/**
	 * The time as returned by SDL_GetTicks when this game was
	 * started. This is used for game statistics.
	 */
	unsigned int gameStartTime;
	
	/**
	 * The minimum number of moves required to solve the game,
	 * and the total number of moves played so far.
	 */
	unsigned int minMoves, moves;
	
	/**
	 * The number of states that the lights are allowed to take
	 * on.
	 */
	unsigned int states;
	
	/**
	 * A mutex to prevent the modification of internal state
	 * durring a paint operation (or vice-versa).
	 */
	SDL_mutex* paintMutex;
	
	/**
	 * The map containing the individual light objects.
	 */
	RectangleMap<Light*>* lights;
	
	/**
	 * Toggles autoplay of the current game.
	 */
	bool autoplay;
	
	/**
	 * Press the button at the given cursor location. This causes
	 * it and the four neighboring lights to advance to their
	 * next state. This method DOES NOT move the cursor.
	 *
	 * This method assumes you've already locked paintMutex.
	 */
	void pressButton(unsigned int x, unsigned int y);
	
	/**
	 * Has the light at (X,Y) go to its next available state.
	 * This operation marks the game as dirty.
	 *
	 * This method assumes you've already locked paintMutex.
	 */
	void toggleLight(unsigned int x, unsigned int y);
	
public:
	/**
	 * Create a new game of Lights Out. This game will have the
	 * traditional 5x5 board size, with 2-state lights. Board
	 * size and number of states may be changed if desired.
	 */
	LightsOutGame(unsigned int width=5, unsigned int height=5, unsigned int states=2, bool autoplay=false);
	
	~LightsOutGame();
	
	/**
	 * Cheat for a single move. Have the computer determine and
	 * perform a single move which will get you closer to a
	 * solved game.
	 */
	void cheat();
	
	/**
	 * Enables or disables demo mode, which causes the game to
	 * play itself.
	 */
	void demoMode(const bool enable);
	
	void eventOccured(const SDL_Event* const event);
	
	/**
	 * Suggests a move that will lead to the completion of the
	 * game. The values of suggestedX and suggestedY will contain
	 * the location after the call completes.
	 */
	void getMoveHint(unsigned int& suggestedX, unsigned int& suggestedY) const;
	
	/**
	 * Move the cursor a specified ammount.
	 */
	void move(int deltaX, int deltaY);
	
	/**
	 * Moves the cursor to the absolute coordinate (X,Y). This
	 * operation marks the game as dirty.
	 */
	void moveAbsolute(unsigned int x, unsigned int y);
	
	bool paint(SDL_Surface& surface, unsigned int width, unsigned int height) const;
	
	/**
	 * Runs the game, which does nothing but wait for a winning
	 * state.
	 */
	void run();
	
	/**
	 * Press the button at the current cursor location. This
	 * causes it and the four neighboring lights to advance to
	 * their next state.
	 */
	void select();
	
	/**
	 * Returns true if the board is in a winning state (that is,
	 * all lights off).
	 */
	bool winningState() const;
	
};


#endif


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


#include <iostream>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_rotozoom.h>
#include <string>
#include "EventObserver.hpp"
#include "Light.hpp"
#include "RectangleMap.tpp"
#include "Renderable.hpp"
#include "Thread.hpp"
#include "Tile.tpp"


/**
 * A LightsOutGame allows for a single "play" of the game of Lights Out.
 * After creating the game, just start (and optionally join) the thread.
 * When the game ends, the thread will exit. Simply create a new
 * LightsOutGame if additional rounds of play are requested.
 */
class LightsOutGame : public EventObserver, public Renderable, public Thread {

protected:
	static TTF_Font* font;
	
	static SDL_Surface* cursorTexture;
	
	int x, y, width, height, gameStartTime, minMoves, moves;
	
	SDL_mutex* paintMutex;
	
	RectangleMap<Light*>* lights;
	
	void toggleLight(int x, int y);
	
	void moveAbsolute(int x, int y);
	
public:
	LightsOutGame(int width=5, int height=5, int states=2);
	
	~LightsOutGame();
	
	void eventOccured(SDL_Event* event);
	
	void getMoveHint(int* suggestedX, int* suggestedY);
	
	Tile<Light*>* getTile(int x, int y);
	
	void move(int deltaX, int deltaY);
	
	int paint(SDL_Surface* surface);
	
	void pressButton(int x, int y);
	
	void run();
	
	void select();
	
	bool winningState();
	
};


#endif


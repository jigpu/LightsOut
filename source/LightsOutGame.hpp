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
#include <SDL/SDL_rotozoom.h>
#include "ControllerObserver.hpp"
#include "Light.hpp"
#include "RectangleMap.tpp"
#include "Renderable.hpp"
#include "Tile.tpp"


/**
 * A LightsOutGame is a single "play" of the game of Lights Out.
 * Additional rounds of play should create new objects.
 */
class LightsOutGame : public ControllerObserver, public Renderable {

protected:
	int width, height;
	
	int x, y;
	
	SDL_Surface* glow;
	
	RectangleMap<Light*>* lights;
	
	void toggleLight(int x, int y);
	
	void moveAbsolute(int x, int y);
	
public:
	LightsOutGame(int width=5, int height=5, int states=4);
	
	~LightsOutGame();
	
	void controllerAction(int type, SDLKey* value);
	
	void getMoveHint(int* suggestedX, int* suggestedY);
	
	Tile<Light*>* getTile(int x, int y);
	
	void move(int deltaX, int deltaY);
	
	int paint(SDL_Surface* surface);
	
	void pressButton(int x, int y);
	
	void select();
	
	bool winningState();
	
};


#endif


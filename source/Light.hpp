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


#ifndef __Light_hpp__
#define __Light_hpp__


#include "Renderable.hpp"


#define COLOR_0   64,64,64
#define COLOR_1   64,192,64
#define COLOR_2   192,64,64
#define COLOR_3   64,64,192
#define COLOR_UNK 255,255,255


/**
 * Lights are the objects that players are attempting to turn
 * off in the game of LightsOut. In the basic game, a light
 * has only two states. More advanced versions allow for additional
 * states.
 */
class Light : public Renderable {

protected:
	static SDL_Surface* glassTexture;
	
	int state, states;
	
	SDL_mutex* paintMutex;
	
public:
	Light(int states=2);
	
	~Light();
	
	int getState();
	
	int getStates();
	
	void nextState();
	
	int paint(SDL_Surface* surface);
	
};


#endif


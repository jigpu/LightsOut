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
#define COLOR_4   64,192,192
#define COLOR_5   192,64,192
#define COLOR_6   192,192,64
#define COLOR_UNK 255,255,255


/**
 * Lights are the objects that players are attempting to turn
 * off in the game of LightsOut. In the basic game, a light
 * has only two states. More advanced versions allow for additional
 * states.
 */
class Light : public Renderable {

protected:
	/**
	 * A static surface containing the glass texture that
	 * overlays all Light objects. DO NOT FREE THIS SURFACE!
	 * AFTER CREATION IT SHOULD REMAIN IN MEMORY FOR ALL FUTURE
	 * LIGHT OBJECTS TO USE.
	 */
	static SDL_Surface* glassTexture;
	
	/**
	 * The current state and total number of states that the
	 * Light may take on. State 0 has special meaning as the
	 * "off" state.
	 */
	int state, states;
	
	/**
	 * A mutex to prevent the change of internal object state
	 * while a paint is taking place (or vice-versa).
	 */
	SDL_mutex* paintMutex;
	
public:
	/**
	 * Create a new Light. By default the light will have two
	 * states (off and on) if no maximum is specified.
	 */
	Light(int states=2);
	
	~Light();
	
	/**
	 * Returns true if the light is on.
	 */
	bool isLightOn();
	
	/**
	 * Cycles the light to the next available state. Lights at
	 * the highest available state will be reset to the "off"
	 * state.
	 */
	void nextState();
	
	int paint(SDL_Surface* surface);
	
};


#endif


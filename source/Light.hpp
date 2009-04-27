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
 * has two states (on, off), though modifications of the game
 * include additional states.
 */
class Light : public Renderable {

protected:
	int state;
	int states;
	SDL_Rect rect;
	
public:
	Light(int states=2);
	int getState();
	void nextState();
	int paint(SDL_Surface* surface);

};


#endif


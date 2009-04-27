#ifndef __Light_hpp__
#define __Light_hpp__


/**
 * Lights are the objects that players are attempting to turn
 * off in the game of LightsOut. In the basic game, a light
 * has two states (on, off), though modifications of the game
 * include additional states.
 */
class Light {

protected:
	int state;
	int max_state;
	
public:
	Light(int max_state=1);
	int getState();
	void nextState();

};


#endif


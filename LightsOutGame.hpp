#ifndef __LightsOutGame_hpp__
#define __LightsOutGame_hpp__


#include "RectangleMap.tpp"


/**
 * A LightsOutGame is a single "play" of the game of Lights Out.
 * Additional rounds may be played by creating a new object.
 */

class LightsOutGame {

protected:
	int width, height;
	RectangleMap<bool>* lights;
	
	void toggleLight(int x, int y);


public:
	LightsOutGame(int width=5, int height=5);
	
	~LightsOutGame();
	
	void getMoveHint(int* suggestedX, int* suggestedY);
	
	void pressButton(int x, int y);
	
	bool winningState();
	
	void paint();

};


#endif


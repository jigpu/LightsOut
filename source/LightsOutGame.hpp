#ifndef __LightsOutGame_hpp__
#define __LightsOutGame_hpp__


#include "Light.hpp"
#include "RectangleMap.tpp"
#include "Renderable.hpp"


/**
 * A LightsOutGame is a single "play" of the game of Lights Out.
 * Additional rounds may be played by creating a new object.
 */

class LightsOutGame : public Renderable {

protected:
	int width, height;
	RectangleMap<Light*>* lights;
	
	void toggleLight(int x, int y);


public:
	LightsOutGame(int width=5, int height=5, int states=2);
	
	~LightsOutGame();
	
	int getHeight();
	
	int getWidth();
	
	Tile<Light*>* getTile(int x, int y);
	
	void getMoveHint(int* suggestedX, int* suggestedY);
	
	void pressButton(int x, int y);
	
	bool winningState();
	
	int paint(SDL_Surface* surface);

};


#endif


#ifndef __LightsOutView_hpp__
#define __LightsOutView_hpp__


#include "LightsOutGame.hpp"


class LightsOutView {

protected:
	int x, y;
	LightsOutGame* game;

public:
	LightsOutView(LightsOutGame* game);
	void paint(SDL_Surface* surface);
	void select(int x, int y);

};


#endif

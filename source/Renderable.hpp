#ifndef __Renderable_hpp__
#define __Renderable_hpp__


#include <SDL/SDL.h>


class Renderable {

public:
	virtual int paint(SDL_Surface* surface) = 0;

};


#endif


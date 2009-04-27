#ifndef __Renderer_hpp__
#define __Renderer_hpp__


#include <SDL/SDL.h>
#include <vector>
#include "Renderable.hpp"
#include "Thread.hpp"


#define MIN_FRAMETIME_MSECS 32


class Renderer : public Thread {

protected:
	SDL_Surface* surface;
	Renderable* child;

public:
	Renderer(SDL_Surface* surface, Renderable* child);
	void run();
};


#endif


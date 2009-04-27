#include <iostream>
#include "Renderer.hpp"


Renderer::Renderer(SDL_Surface* surface, Renderable* child) {
	this->surface = surface;
	this->child = child;
}


void Renderer::run() {
	bool running    = true;
	int currTime    = 0;
	int prevTime    = 0;
	int timeElapsed = 0;
	
	//Frame-rate independent code from
	//http://hdrlab.org.nz/frame-rate-independent-animation-using-sdl-and-opengl-with-frame-rate-limiting/
	while (running) {
		currTime = SDL_GetTicks();
		timeElapsed = currTime - prevTime;
		while (timeElapsed < MIN_FRAMETIME_MSECS) {
			yield(MIN_FRAMETIME_MSECS - timeElapsed);
			currTime = SDL_GetTicks();
			timeElapsed = currTime - prevTime;
		}
		prevTime = currTime;
		if (child->paint(surface) == 0)
			SDL_Flip(surface);
	}
}


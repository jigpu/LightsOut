#include <SDL/SDL.h>
#include "Controller.hpp"
#include "Keyboard.hpp"


using namespace std;


Keyboard::Keyboard() {
}


Keyboard::~Keyboard() {

}


void Keyboard::run() {
	SDL_Event event;
	bool running = true;

	while(running) {
		yield(25);
		while(SDL_PollEvent(&event)) {
			switch(event.type){
				case SDL_KEYDOWN:
				case SDL_KEYUP:
					 observer->controllerAction(event.type, &event.key.keysym.sym);
					break;
				case SDL_QUIT:
					running = false;
					break;
			}
		}
	}
}


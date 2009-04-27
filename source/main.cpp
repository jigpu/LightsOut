#include <cstdlib>    // For some useful functions such as atexit :)
#include <gccore.h>
#include <SDL/SDL.h> //Main SDL header
#include <fat.h>
//#include "Keyboard.hpp"
#include "LightsOutGameManager.hpp"
#include "Wiimote.hpp"


#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

 
SDL_Surface* screen; //This pointer will reference the backbuffer 

int initVideo(Uint32 flags = SDL_DOUBLEBUF) {
	// Load SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
		return false;
	}
	
	atexit(SDL_Quit); // Clean it up nicely :)
 
	// fullscreen can be toggled at run time :) any you might want to change the flags with params?
	//set the main screen to SCREEN_WIDTHxSCREEN_HEIGHT with a colour depth of 16:
	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 16, flags);
	if (screen == NULL) {
		fprintf(stderr, "Unable to set video mode: %s\n", SDL_GetError());
		return false;
	}
	
	SDL_ShowCursor(SDL_DISABLE);
	
	return true;
}


int main(int argc, char** argv) {
	fatInitDefault();
	initVideo();
	
	Wiimote* controller = new Wiimote();
	//Keyboard* controller = new Keyboard();
	
	LightsOutGameManager* game = new LightsOutGameManager(screen);
	controller->observer = game;
	
	controller->start();
	game->start();
	game->join();
	
	//Explicit destruction, since the Keyboard controller
	//tweaks the console and only resets it in the
	//destructor (which isn't called by default?!)
	delete(game);
	delete(controller);
	
	SDL_Quit();
	return 0;
}


/**
 * Copyright © 2009, Localhost Labs, Jason Gerecke
 * 
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 * 
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 * 
 *   1. The origin of this software must not be misrepresented; you must not
 *      claim that you wrote the original software. If you use this software
 *      in a product, an acknowledgment in the product documentation would be
 *      appreciated but is not required.
 *   
 *   2. Altered source versions must be plainly marked as such, and must not be
 *      misrepresented as being the original software.
 *   
 *   3. This notice may not be removed or altered from any source
 *      distribution.
 */


#include <cstdlib>
#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include "EventPublisher.hpp"
#include "LightsOutGameManager.hpp"
#include "Renderer.hpp"


#ifndef PC
#ifdef DEBUG
#include <debug.h>
#endif
#include <fat.h>
#include <gccore.h>
#include <unistd.h>
#include "Wiimote.hpp"
#endif


#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480


SDL_Surface* screen; //This pointer will reference the backbuffer


void initSDL(Uint32 hardware = SDL_INIT_VIDEO, Uint32 flags = SDL_DOUBLEBUF | SDL_HWSURFACE) {
	if (SDL_Init(hardware) != 0) {
		std::cerr << "Unable to initialize SDL: " << SDL_GetError() << std::endl;
		throw 1;
	}
	atexit(SDL_Quit);
	
	TTF_Init();
	atexit(TTF_Quit);
 	
	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 16, flags);
	if (screen == NULL) {
		std::cerr << "Unable to set video mode: " << SDL_GetError() << std::endl;
		throw 1;
	}
}


int main(int argc, char** argv) {
	Uint32 hardware = SDL_INIT_VIDEO;
	#ifndef PC
	hardware |= SDL_INIT_JOYSTICK; //Wiimote is a Joystick in SDL
	#endif
	initSDL(hardware);
	
	#ifndef PC
	std::cerr << std::endl;
	#ifdef DEBUG
	DEBUG_Init(GDBSTUB_DEVICE_WIFI, 8000);
	_break();
	#endif
	if (!fatInitDefault()) {
		std::cerr << "Unable to initialize FAT subsystem!" << std::endl;
		sleep(5);
		throw 1;
	}
	
	Wiimote* wiimote = new Wiimote();
	wiimote->start();
	#endif
	
	EventPublisher::getInstance().start();
	
	LightsOutGameManager* manager = new LightsOutGameManager();
	manager->start();
	
	Renderer* renderer = new Renderer(screen, manager);
	renderer->start();
	
	manager->join();
	
	//For a clean shutdown, we need to ensure a few things:
	//
	//  * The renderer stops before SDL quits
	//  * All event observers have been unregistered before the
	//    publisher itself is destroyed
	SDL_Event die;
	die.user.type = SDL_USEREVENT;
	SDL_PushEvent(&die);
	
	renderer->join();
	EventPublisher::getInstance().join();
	
	return 0;
}


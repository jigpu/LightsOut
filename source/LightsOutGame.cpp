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


#include <stdlib.h>
#include "LightsOutGame.hpp"


LightsOutGame::LightsOutGame(int width, int height, int states) {
	this->width = width;
	this->height = height;	
	x = 0;
	y = 0;
	lights = new RectangleMap<Light*>(width, height, 10, 10);
	dirty = true;
	
	for (int x=0; x<width; x++)
		for (int y=0; y<height; y++)
			lights->setTile(x, y, new Tile<Light*>(new Light(states)));
	
	//Initialize the board to some solvable state.
	for (int x=0; x<width; x++)
		for (int y=0; y<height; y++)
			for (int i=0; i<rand()%states; i++)
				pressButton(x,y);
}


LightsOutGame::~LightsOutGame() {
	for (int x=0; x<width; x++) {
		for (int y=0; y<height; y++) {
			delete lights->getTile(x, y);
		}
	}
}


void LightsOutGame::controllerAction(int type, SDLKey* value) {
	//cout << "Recieved button " << value << endl;
	
	switch (type) {
		case SDL_KEYDOWN: {
			switch(*value) {
				case SDLK_UP:     move( 0,-1); break;
				case SDLK_DOWN:   move( 0, 1); break;
				case SDLK_LEFT:   move(-1, 0); break;
				case SDLK_RIGHT:  move( 1, 0); break;
				case SDLK_TAB:
				case SDLK_b:      getMoveHint(&x, &y); move(0,0); break;
				case SDLK_RETURN:
				case SDLK_a:      select(); break;
			}
		}
	}
}


void LightsOutGame::getMoveHint(int* suggestedX, int* suggestedY) {
	for (int y=0; y<height; y++) {
		for (int x=0; x<width; x++) {
			if (lights->getTile(x,y)->object->getState() != 0) {
				if (y+1 != height) {
					//"Chase the lights"
					*suggestedX = x;
					*suggestedY = y+1;
					return;
				}
				else {
					//Fixing the top
					*suggestedY = 0;
					switch (x) {
						case 0: *suggestedX = 1; return;
						case 1: *suggestedX = 0; return;
						case 2: *suggestedX = 3; return;	
					}
				}
			}
		}
	}
	
	//Unsolvable
	//How on earth we got here after the constructor ensured
	//the board was solvable is beyond me.
	//Should throw an exception...
	exit(-1);
}


Tile<Light*>* LightsOutGame::getTile(int x, int y) {
	return lights->getTile(x, y);
}


void LightsOutGame::move(int deltaX, int deltaY) {
	int newX = x+deltaX;
	int newY = y+deltaY;
	
	if (newX < 0) newX = 0;
	if (newX >= width) newX = width-1;
	if (newY < 0) newY = 0;
	if (newY >= height) newY = height-1;
	
	moveAbsolute(newX, newY);
}


void LightsOutGame::moveAbsolute(int x, int y) {
	this->x = x;
	this->y = y;
	
	dirty = true;
}


int LightsOutGame::paint(SDL_Surface* surface) {
	if (!dirty)
		return 1;
	
	SDL_Rect dest;
	dest.w = 64;
	dest.h = 64;
	
	for (int y=0; y<height; y++) {
		dest.y = y*dest.h;
		for (int x=0; x<width; x++) {
			dest.x = x*dest.w;
			
			SDL_Surface* subsurface = SDL_CreateRGBSurface(surface->flags,dest.w,dest.h,16,0,0,0,0);
			lights->getTile(x,y)->object->paint(subsurface);
			
			SDL_BlitSurface(subsurface, NULL, surface, &dest);
			SDL_FreeSurface(subsurface);
		}
	}
	
	dirty = false;
	return 0;
}


void LightsOutGame::pressButton(int x, int y) {
	if (x >= width || x < 0 || y >= height || y < 0)
		return; //Should throw an invalid move exception...
	
	toggleLight(x,y);
	toggleLight(x-1,y);
	toggleLight(x+1,y);
	toggleLight(x,y-1);
	toggleLight(x,y+1);
}


void LightsOutGame::select() {
	pressButton(x,y);
	dirty = true;
}


void LightsOutGame::toggleLight(int x, int y) {
	if (x >= width || x < 0 || y >= height || y < 0)
		return; //Assume the caller was just lazy
	
	lights->getTile(x,y)->object->nextState();
}


bool LightsOutGame::winningState() {
	for (int y=0; y<height; y++) {
		for (int x=0; x<width; x++) {
			if (lights->getTile(x,y)->object->getState() != 0)
				return false;
			
		}
	}
	return true;
}


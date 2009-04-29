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
	
	glow = IMG_Load("glow.png");
	if (glow == NULL) {
		std::cout << "Error loading glow.png: " << SDL_GetError() << std::endl;
	}
	
	surface = NULL;
	
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
	//std::cout << "Recieved button " << value << std::endl;
	
	switch (type) {
		case SDL_KEYDOWN: {
			switch(*value) {
				case SDLK_UP:     move( 0,-1); break;
				case SDLK_DOWN:   move( 0, 1); break;
				case SDLK_LEFT:   move(-1, 0); break;
				case SDLK_RIGHT:  move( 1, 0); break;
				case SDLK_TAB:
				case SDLK_b:      getMoveHint(&x, &y); move(0,0);
				case SDLK_RETURN:
				case SDLK_a:      select(); break;
				default: break;
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
	if (this->surface == NULL ||
	    this->surface->w != surface->w ||
	    this->surface->h != surface->h)
		this->surface = SDL_CreateRGBSurface(surface->flags,surface->w,surface->h,16,0,0,0,0);
	
	
	//Create subsurfaces and paint them
	double tileWidth  = (double)surface->w/(double)width;
	double tileHeight = (double)surface->h/(double)height;
	
	SDL_Rect dest;
	bool dirtysub = false;
	for (int x=0; x<width; x++) {
		dest.x = (int)(tileWidth*x);
		dest.w = (int)(tileWidth*(x+1)) - dest.x;
		for (int y=0; y<height; y++) {
			dest.y = (int)(tileHeight*y);
			dest.h = (int)(tileHeight*(y+1)) - dest.y;
			
			SDL_Surface* subsurface = SDL_CreateRGBSurface(surface->flags,dest.w,dest.h,16,0,0,0,0);
			if (lights->getTile(x,y)->object->paint(subsurface) == 0) dirtysub = true;
			
			SDL_BlitSurface(subsurface, NULL, this->surface, &dest);
			SDL_FreeSurface(subsurface);
		}
	}
	
	
	//Paint this object itself if dirty, or if underlying
	//objects were dirty
	if (dirty || dirtysub) {
		dest.x = (int)(tileWidth*(this->x-1));
		dest.w = (int)(tileWidth*3);
		
		dest.y = (int)(tileHeight*(this->y-1));
		dest.h = (int)(tileHeight*3);
		
		SDL_Surface* zoom = rotozoomSurfaceXY(glow, 0.0, ((double)(dest.w))/((double)(glow->w)), ((double)(dest.h))/((double)(glow->h)), 1);
		SDL_SetAlpha(zoom, SDL_SRCALPHA, 0);
		SDL_BlitSurface(zoom, NULL, this->surface, &dest);
		SDL_FreeSurface(zoom);
	}
	
	
	//Blit onto the target surface and return
	SDL_BlitSurface(this->surface, NULL, surface, NULL);
	if (dirty || dirtysub) {
		dirty = false;
		return 0;
	}
	else {
		return 1;
	}
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


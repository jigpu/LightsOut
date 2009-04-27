#include <stdio.h>
#include <stdlib.h>
#include "LightsOutGame.hpp"


using namespace std;


void LightsOutGame::toggleLight(int x, int y) {
	if (x >= width || x < 0 || y >= height || y < 0)
		return; //Assume the caller was just lazy
	
	lights->getTile(x,y)->object->nextState();
}


LightsOutGame::LightsOutGame(int width, int height, int states) {
	this->width = width;
	this->height = height;
	lights = new RectangleMap<Light*>(width, height, 10, 10);
	
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


int LightsOutGame::getHeight() {
	return height;
}


int LightsOutGame::getWidth() {
	return width;
}


Tile<Light*>* LightsOutGame::getTile(int x, int y) {
	return lights->getTile(x, y);
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


void LightsOutGame::pressButton(int x, int y) {
	if (x >= width || x < 0 || y >= height || y < 0)
		return; //Should throw an invalid move exception...
	
	toggleLight(x,y);
	toggleLight(x-1,y);
	toggleLight(x+1,y);
	toggleLight(x,y-1);
	toggleLight(x,y+1);
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


int LightsOutGame::paint(SDL_Surface* surface) {
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
	
	return 0;
}

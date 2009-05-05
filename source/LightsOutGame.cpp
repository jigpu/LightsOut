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


#include <iomanip>
#include <sstream>
#include <stdlib.h>
#include "EventPublisher.hpp"
#include "LightsOutGame.hpp"


TTF_Font* LightsOutGame::font;


SDL_Surface* LightsOutGame::cursorTexture;


LightsOutGame::LightsOutGame(int width, int height, int states) {
	std::clog << "Creating new LightsOutGame." << std::endl;
	
	x = 0;
	y = 0;
	this->width = width;
	this->height = height;
	minMoves = 0;
	
	paintMutex = SDL_CreateMutex();
	
	font = TTF_OpenFont("Go Boom!.ttf", 36);
	if (font == NULL)
		std::cerr << "Error loading Go Boom!.ttf: " << SDL_GetError() << std::endl;
	
	cursorTexture = IMG_Load("cursor.png");
	if (cursorTexture == NULL)
		std::cerr << "Error loading cursor.png: " << SDL_GetError() << std::endl;
	
	lights = new RectangleMap<Light*>(width, height, 10, 10);
	
	for (int x=0; x<width; x++)
		for (int y=0; y<height; y++)
			lights->setTile(x, y, new Tile<Light*>(new Light(states)));
	
	//Initialize the board to some solvable state.
	for (int x=0; x<width; x++) {
		for (int y=0; y<height; y++) {
			for (int i=0; i<rand()%states; i++) {
				minMoves++;
				pressButton(x,y);
			}
		}
	}
		
	moves = 0; //Reset since its called in pressButton :D 
	surface = NULL;
	dirty = true;
}


LightsOutGame::~LightsOutGame() {
	std::clog << "Deleting LightsOutGame." << std::endl;
	
	SDL_DestroyMutex(paintMutex);
	SDL_FreeSurface(surface);
	//Do not free cursorTexture on destruction since its static
	//Do not close font on destruction since its static
	
	for (int x=0; x<width; x++) {
		for (int y=0; y<height; y++) {
			delete lights->getTile(x, y);
		}
	}
	
	delete lights;
}


void LightsOutGame::eventOccured(SDL_Event* event) {	
	switch (event->type) {
		case SDL_KEYDOWN: {
			switch(event->key.keysym.sym) {
				case SDLK_TAB:
				case SDLK_b:
					int newX, newY;
					getMoveHint(&newX, &newY);
					moveAbsolute(newX,newY);
				case SDLK_RETURN:
				case SDLK_a:      select();    break;
				case SDLK_UP:     move( 0,-1); break;
				case SDLK_DOWN:   move( 0, 1); break;
				case SDLK_LEFT:   move(-1, 0); break;
				case SDLK_RIGHT:  move( 1, 0); break;
			}
			break;
		}
		
		case SDL_QUIT: {
			stop();
			break;
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
	
	//There's no reasonable "hint" for a board that has already won ;)
	if (winningState()) {
		std::clog << "Game has already been won, providing bogus hint of (0,0)." << std::endl;
		*suggestedX = 0;
		*suggestedY = 0;
		return;
	}
	
	//Unsolvable
	//How on earth we got here after the constructor ensured
	//the board was solvable is beyond me.
	//Should throw an exception...
	std::cerr << "Game is not solvable, cannot give move hint." << std::endl;
	exit(-50);
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
	SDL_mutexP(paintMutex);
	this->x = x;
	this->y = y;
	
	dirty = true;
	SDL_mutexV(paintMutex);
}


int LightsOutGame::paint(SDL_Surface* surface) {
	SDL_mutexP(paintMutex);
	
	if (this->surface == NULL ||
	    this->surface->w != surface->w ||
	    this->surface->h != surface->h) {
		this->surface = SDL_CreateRGBSurface(surface->flags,surface->w,surface->h,16,0,0,0,0);
		dirty = true;
	}
	
	
	//Paint the gameboard
	SDL_Surface* board = SDL_CreateRGBSurface(surface->flags,surface->h,surface->h,16,0,0,0,0);
	double tileWidth  = (double)board->w/(double)width;
	double tileHeight = (double)board->h/(double)height;
	
	SDL_Rect dest;
	bool dirtysub = false;
	for (int x=0; x<width; x++) {
		dest.x = (int)(tileWidth*x);
		dest.w = (int)(tileWidth*(x+1)) - dest.x;
		for (int y=0; y<height; y++) {
			dest.y = (int)(tileHeight*y);
			dest.h = (int)(tileHeight*(y+1)) - dest.y;
			
			SDL_Surface* subsurface = SDL_CreateRGBSurface(surface->flags,dest.w,dest.h,16,0,0,0,0);
			if (lights->getTile(x,y)->object->paint(subsurface) == 0)
				dirtysub = true;
			
			SDL_BlitSurface(subsurface, NULL, board, &dest);
			SDL_FreeSurface(subsurface);
		}
	}
	
	
	//Paint the cursor over the gameboard regardless of dirt
	//If not painted when us and lights are clean, the lights will
	//still be blitted to board, making the cursor "disappear".
	dest.x = (int)(tileWidth*(this->x-1));
	dest.w = (int)(tileWidth*3);
	
	dest.y = (int)(tileHeight*(this->y-1));
	dest.h = (int)(tileHeight*3);
	
	SDL_Surface* zoom = rotozoomSurfaceXY(cursorTexture, 0.0, ((double)(dest.w))/((double)(cursorTexture->w)), ((double)(dest.h))/((double)(cursorTexture->h)), 1);
	SDL_SetAlpha(zoom, SDL_SRCALPHA, 0);
	SDL_BlitSurface(zoom, NULL, board, &dest);
	SDL_FreeSurface(zoom);
	
	
	if (dirty) {
		//Recreate our surface so we have a clean slate
		SDL_FreeSurface(this->surface);
		this->surface = SDL_CreateRGBSurface(surface->flags,surface->w,surface->h,16,0,0,0,0);
		
		//Paint game text
		SDL_Color clrFg = {255,255,255,0};
		
		dest.x = board->h + 8;
		dest.y = 16;
		dest.w = 0;
		dest.h = 0;
		SDL_Surface* movesLS = TTF_RenderText_Blended(font, "Moves:", clrFg);
		SDL_BlitSurface(movesLS, NULL, this->surface, &dest);
		SDL_FreeSurface(movesLS);
		
		dest.y = 48;
		std::stringstream movesString;
		movesString << moves << "/" << minMoves;
		SDL_Surface* movesS = TTF_RenderText_Blended(font, movesString.str().c_str(), clrFg );
		SDL_BlitSurface(movesS, NULL, this->surface, &dest);
		SDL_FreeSurface(movesS);
				
		dest.y = 112;
		SDL_Surface* timeLS = TTF_RenderText_Blended(font, "Time:", clrFg);
		SDL_BlitSurface(timeLS, NULL, this->surface, &dest);
		SDL_FreeSurface(timeLS);
		
		dest.y = 144;
		int elapsed = SDL_GetTicks() - gameStartTime;
		std::stringstream timeString;
		timeString << std::setw(2) << std::setfill('0') << elapsed/36000000 << ":"
		           << std::setw(2) << std::setfill('0') << elapsed/60000 % 60 << ":"
		           << std::setw(2) << std::setfill('0') << elapsed/1000 % 60;
		SDL_Surface* timeS = TTF_RenderText_Blended(font, timeString.str().c_str(), clrFg);
		SDL_BlitSurface(timeS, NULL, this->surface, &dest);
		SDL_FreeSurface(timeS);
				
		dest.y = 374;
		SDL_Surface* diffLS = TTF_RenderText_Blended(font, "Difficulty:", clrFg);
		SDL_BlitSurface(diffLS, NULL, this->surface, &dest);
		SDL_FreeSurface(diffLS);
				
		dest.y = 406;
		std::stringstream difficultyText;
		difficultyText << getTile(0,0)->object->getStates() << " States";
		SDL_Surface* diffS = TTF_RenderText_Blended(font, difficultyText.str().c_str(), clrFg);
		SDL_BlitSurface(diffS, NULL, this->surface, &dest);
		SDL_FreeSurface(diffS);
	}
	
	
	//Copy the gameboard to the game's surface
	SDL_BlitSurface(board, NULL, this->surface, NULL);
	SDL_FreeSurface(board);
	
	//Blit onto the target surface and return
	SDL_BlitSurface(this->surface, NULL, surface, NULL);
	if (dirty || dirtysub) {
		dirty = false;
		SDL_mutexV(paintMutex);
		return 0;
	}
	else {
		SDL_mutexV(paintMutex);
		return 1;
	}
}


void LightsOutGame::pressButton(int x, int y) {
	if (x >= width || x < 0 || y >= height || y < 0)
		return; //Should throw an invalid move exception...
	
	SDL_mutexP(paintMutex);
	moves++;
	SDL_mutexV(paintMutex);
	
	toggleLight(x,y);
	toggleLight(x-1,y);
	toggleLight(x+1,y);
	toggleLight(x,y-1);
	toggleLight(x,y+1);
}


void LightsOutGame::run() {
	EventPublisher::getInstance().addEventObserver(this);
	
	gameStartTime = SDL_GetTicks();
	while (runThread && !winningState()) {
		dirty = true;
		yield(250);
	};
	
	EventPublisher::getInstance().removeEventObserver(this);
}


void LightsOutGame::select() {
	pressButton(x,y);
}


void LightsOutGame::toggleLight(int x, int y) {
	if (x >= width || x < 0 || y >= height || y < 0)
		return; //Assume the caller was just lazy
	
	SDL_mutexP(paintMutex);
	lights->getTile(x,y)->object->nextState();
	SDL_mutexV(paintMutex);
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


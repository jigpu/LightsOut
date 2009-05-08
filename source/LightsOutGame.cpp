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
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <SDL/SDL_image.h>
#include <SDL/SDL_rotozoom.h>
#include "EventPublisher.hpp"
#include "LightsOutGame.hpp"


TTF_Font* LightsOutGame::font = NULL;


SDL_Surface* LightsOutGame::cursorTexture = NULL;


LightsOutGame::LightsOutGame(int width, int height, int states) {
	std::clog << SDL_GetTicks() << " (" << this << "): new LightsOutGame." << std::endl;
	
	if (font == NULL) {
		font = TTF_OpenFont("yukari.ttf", 36);
		if (font == NULL) {
			std::cerr << "Error loading yukari.ttf: " << SDL_GetError() << std::endl;
			throw 1;
		}
	}
	
	if (cursorTexture == NULL) {
		cursorTexture = IMG_Load("cursor.png");
		if (cursorTexture == NULL) {
			std::cerr << "Error loading cursor.png: " << SDL_GetError() << std::endl;
			throw 1;
		}
	}
	
	x = 0;
	y = 0;
	this->states = states;
	
	lights = new RectangleMap<Light*>(width, height);
	
	//Create all gameboard lights
	for (int x=0; x<width; x++) {
		for (int y=0; y<height; y++) {
			lights->setTile(x, y, new Tile<Light*>(new Light(states)));
		}
	}
	
	//MUST create the mutex	prior to initialization, since pressButton
	//acquires it.
	paintMutex = SDL_CreateMutex();
	
	//Initialize the board to some solvable state.
	minMoves = 0;
	for (int x=0; x<width; x++) {
		for (int y=0; y<height; y++) {
			for (int i=0; i<rand()%states; i++) {
				minMoves++;
				pressButton(x,y);
			}
		}
	}
		
	moves = 0; //Moves is modified by the initialization above, so we reset it here
	
	surface = NULL;
	dirty = true;
}


LightsOutGame::~LightsOutGame() {
	std::clog << SDL_GetTicks() << " (" << this << "): delete LightsOutGame." << std::endl;
	
	SDL_DestroyMutex(paintMutex);
	SDL_FreeSurface(surface);
	//Do not free cursorTexture on destruction since its static
	//Do not close font on destruction since its static
	
	for (int x=0; x<lights->getWidth(); x++) {
		for (int y=0; y<lights->getHeight(); y++) {
			Tile<Light*>* tile = lights->getTile(x, y);
			delete tile->object;
			delete tile;
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
					getMoveHint(newX, newY);
					moveAbsolute(newX, newY);
					//break if you don't want auto-select.
				case SDLK_RETURN:
				case SDLK_a:
					select();
					break;
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


void LightsOutGame::getMoveHint(int& suggestedX, int& suggestedY) {
	for (int y=0; y<lights->getHeight(); y++) {
		for (int x=0; x<lights->getWidth(); x++) {
			if (lights->getTile(x,y)->object->isLightOn()) {
				if (y+1 != lights->getHeight()) {
					//"Chase the lights"
					suggestedX = x;
					suggestedY = y+1;
					return;
				}
				else {
					//Fixing the top
					suggestedY = 0;
					switch (x) {
						case 0: suggestedX = 1; return;
						case 1: suggestedX = 0; return;
						case 2: suggestedX = 3; return;	
					}
				}
			}
		}
	}
	
	//There's no reasonable "hint" for a board that has already won ;)
	if (winningState()) {
		std::clog << SDL_GetTicks() << " (" << this << "): Game has already been won, providing bogus hint of (0,0)." << std::endl;
		suggestedX = 0;
		suggestedY = 0;
		return;
	}
	
	//Unsolvable
	//How on earth we got here after the constructor ensured
	//the board was solvable is beyond me.
	std::cerr << "Game is not solvable, cannot give move hint." << std::endl;
	throw 10;
}


void LightsOutGame::move(int deltaX, int deltaY) {
	SDL_mutexP(paintMutex);
	int newX = x+deltaX;
	int newY = y+deltaY;
	SDL_mutexV(paintMutex);
	
	if (newX < 0) newX = 0;
	if (newX >= lights->getWidth()) newX = lights->getWidth()-1;
	if (newY < 0) newY = 0;
	if (newY >= lights->getHeight()) newY = lights->getHeight()-1;
	
	moveAbsolute(newX, newY);
}


void LightsOutGame::moveAbsolute(int x, int y) {
	SDL_mutexP(paintMutex);
	this->x = x;
	this->y = y;
	
	dirty = true;
	SDL_mutexV(paintMutex);
}


bool LightsOutGame::paint(SDL_Surface& surface, int width, int height) {
	SDL_mutexP(paintMutex);
	
	if (dirty ||
	    this->surface == NULL ||
	    this->surface->w != width ||
	    this->surface->h != height) {
		SDL_FreeSurface(this->surface);
		this->surface = SDL_CreateRGBSurface(SDL_HWSURFACE,width,height,16,0,0,0,0);
		dirty = true;
	}
	
	SDL_Rect dest;
	bool dirtysub = false;
	
	//Create & paint gameboard subsurface
	///////////////////////////////////////////////////
	SDL_Surface* gameboard = SDL_CreateRGBSurface(SDL_HWSURFACE,this->surface->h,this->surface->h,16,0,0,0,0);
	double tileWidth  = (double)gameboard->w/(double)lights->getWidth();
	double tileHeight = (double)gameboard->h/(double)lights->getHeight();
	
	//Paint lights onto gameboard
	for (int x=0; x<lights->getWidth(); x++) {
		dest.x = (int)(tileWidth*x);
		dest.w = (int)(tileWidth*(x+1)) - dest.x;
		for (int y=0; y<lights->getHeight(); y++) {
			dest.y = (int)(tileHeight*y);
			dest.h = (int)(tileHeight*(y+1)) - dest.y;
			
			SDL_Surface subsurface;
			
			bool dirtyPaint = lights->getTile(x,y)->object->paint(subsurface, dest.w, dest.h);
			dirtysub |= dirtyPaint;
			
			//If we're dirty [moved cursor], we need to blit all
			//lights back onto the screen to "undraw" the cursor
			if (dirty || dirtyPaint)
				SDL_BlitSurface(&subsurface, NULL, gameboard, &dest);
		}
	}
	
	//Paint cursor onto gameboard, and blit gameboard onto surface
	if (dirty || dirtysub) {
		dest.x = (int)(tileWidth*(this->x-1));
		dest.y = (int)(tileHeight*(this->y-1));
		dest.w = (int)(tileWidth*3);
		dest.h = (int)(tileHeight*3);
		
		double widthPercent  = ((double)(dest.w))/((double)(cursorTexture->w));
		double heightPercent = ((double)(dest.h))/((double)(cursorTexture->h));
		SDL_Surface* zoom = rotozoomSurfaceXY(cursorTexture, 0.0, widthPercent, heightPercent, 1);
		SDL_SetAlpha(zoom, SDL_SRCALPHA, 0);
		SDL_BlitSurface(zoom, NULL, gameboard, &dest);
		SDL_FreeSurface(zoom);
		
		SDL_BlitSurface(gameboard, NULL, this->surface, NULL);
	}
	SDL_FreeSurface(gameboard);
	
	//Paint stats onto surface
	///////////////////////////////////////////////////
	if (dirty) {
		//Paint game text
		SDL_Color clrFg = {255,255,255,0};
		
		dest.x = this->surface->h + 8;
		dest.y = 16;
		dest.w = 0;
		dest.h = 0;
		SDL_Surface* movesLS = TTF_RenderText_Blended(font, "Moves:", clrFg);
		SDL_BlitSurface(movesLS, NULL, this->surface, &dest);
		SDL_FreeSurface(movesLS);
		
		dest.y += 32;
		std::stringstream movesString;
		movesString << moves << "/" << minMoves;
		SDL_Surface* movesS = TTF_RenderText_Blended(font, movesString.str().c_str(), clrFg );
		SDL_BlitSurface(movesS, NULL, this->surface, &dest);
		SDL_FreeSurface(movesS);
				
		dest.y = 112;
		SDL_Surface* timeLS = TTF_RenderText_Blended(font, "Time:", clrFg);
		SDL_BlitSurface(timeLS, NULL, this->surface, &dest);
		SDL_FreeSurface(timeLS);
		
		dest.y += 32;
		int elapsed = SDL_GetTicks() - gameStartTime;
		std::stringstream timeString;
		timeString << std::setw(2) << std::setfill('0') << elapsed/3600000 << ":"
		           << std::setw(2) << std::setfill('0') << elapsed/60000 % 60 << ":"
		           << std::setw(2) << std::setfill('0') << elapsed/1000 % 60;
		SDL_Surface* timeS = TTF_RenderText_Blended(font, timeString.str().c_str(), clrFg);
		SDL_BlitSurface(timeS, NULL, this->surface, &dest);
		SDL_FreeSurface(timeS);
				
		dest.y = this->surface->h - 64;
		SDL_Surface* diffLS = TTF_RenderText_Blended(font, "Difficulty:", clrFg);
		SDL_BlitSurface(diffLS, NULL, this->surface, &dest);
		SDL_FreeSurface(diffLS);
		
		dest.y += 32;
		std::stringstream difficultyText;
		difficultyText << states << " States";
		SDL_Surface* diffS = TTF_RenderText_Blended(font, difficultyText.str().c_str(), clrFg);
		SDL_BlitSurface(diffS, NULL, this->surface, &dest);
		SDL_FreeSurface(diffS);
	}
	
	//Set surface and return
	///////////////////////////////////////////////////
	surface = *(this->surface);
	
	if (dirty || dirtysub) {
		dirty = false;
		SDL_mutexV(paintMutex);
		
		return true;
	}
	else {
		dirty = false;
		SDL_mutexV(paintMutex);
		
		return false;
	}
}


void LightsOutGame::pressButton(int x, int y) {
	if (x >= lights->getWidth() || x < 0 || y >= lights->getHeight() || y < 0)
		throw 11;
	
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
	if (x >= lights->getWidth() || x < 0 || y >= lights->getHeight() || y < 0)
		return; //Assume the caller was just lazy
	
	SDL_mutexP(paintMutex);
	lights->getTile(x,y)->object->nextState();
	SDL_mutexV(paintMutex);
}


bool LightsOutGame::winningState() {
	for (int y=0; y<lights->getHeight(); y++) {
		for (int x=0; x<lights->getWidth(); x++) {
			if (lights->getTile(x,y)->object->isLightOn())
				return false;
			
		}
	}
	return true;
}


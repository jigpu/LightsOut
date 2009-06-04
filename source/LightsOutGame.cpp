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


LightsOutGame::LightsOutGame(unsigned int width, unsigned int height, unsigned int states, bool autoplay) {
	//std::clog << SDL_GetTicks() << " (" << this << "): new LightsOutGame." << std::endl;
	
	if (font == NULL) {
		//font = TTF_OpenFont("yukari.ttf", 36);
		FILE* file = fopen("yukari.ttf", "r");
		font = TTF_OpenFontRW(SDL_RWFromFP(file, 0), 1, 36);
		//fclose(file); //File must remain open for SDL_TTF to use the font
		if (font == NULL) {
			std::cerr << "Error loading yukari.ttf: " << SDL_GetError() << std::endl;
			throw 1;
		}
	}
	
	if (cursorTexture == NULL) {
		//cursorTexture = IMG_Load("cursor.png");
		FILE* file = fopen("cursor.png", "r");
		cursorTexture = IMG_Load_RW(SDL_RWFromFP(file, 0), 1);
		fclose(file);
		if (cursorTexture == NULL) {
			std::cerr << "Error loading cursor.png: " << SDL_GetError() << std::endl;
			throw 1;
		}
	}
	
	x = 0;
	y = 0;
	this->states = states;
	this->autoplay = autoplay;
	
	lights = new RectangleMap<Light*>(width, height);
	
	//Create all gameboard lights
	for (unsigned int x=0; x<width; x++) {
		for (unsigned int y=0; y<height; y++) {
			lights->setTile(x, y, new Tile<Light*>(new Light(states)));
		}
	}
	
	//MUST create the mutex	prior to initialization, since pressButton
	//acquires it.
	paintMutex = SDL_CreateMutex();
	
	//Initialize the board to some solvable state.
	minMoves = 0;
	for (unsigned int x=0; x<width; x++) {
		for (unsigned int y=0; y<height; y++) {
			int presses = rand()%states;
			minMoves+=(states-presses)%states;
			for (unsigned int i=0; i<presses; i++) {
				pressButton(x,y);
			}
		}
	}
		
	moves = 0; //Moves is modified by the initialization above, so we reset it here
	
	surface = NULL;
	dirty = true;
}


LightsOutGame::~LightsOutGame() {
	//std::clog << SDL_GetTicks() << " (" << this << "): delete LightsOutGame." << std::endl;
	
	SDL_DestroyMutex(paintMutex);
	SDL_FreeSurface(surface);
	//Do not free cursorTexture on destruction since its static
	//Do not close font on destruction since its static
	
	//Ensure the publisher doesn't have any soon-to-be stale reference
	EventPublisher::getInstance().removeEventObserver(this);
	
	for (unsigned int x=0; x<lights->getWidth(); x++) {
		for (unsigned int y=0; y<lights->getHeight(); y++) {
			Tile<Light*>* tile = lights->getTile(x, y);
			delete tile->object;
			delete tile;
		}
	}
	
	delete lights;
}


void LightsOutGame::cheat() {
	unsigned int newX, newY;
	getMoveHint(newX, newY);
	moveAbsolute(newX, newY);
	select();
}


void LightsOutGame::demoMode(const bool enable) {
	SDL_mutexP(paintMutex);
	autoplay = enable;
	SDL_mutexV(paintMutex);
}


void LightsOutGame::eventOccured(const SDL_Event* const event) {
	switch (event->type) {
		case SDL_KEYDOWN:
			switch(event->key.keysym.sym) {
				case SDLK_UP:     move( 0,-1); break;
				case SDLK_DOWN:   move( 0, 1); break;
				case SDLK_LEFT:   move(-1, 0); break;
				case SDLK_RIGHT:  move( 1, 0); break;
				case SDLK_RETURN: select(); break;
				case SDLK_TAB:    cheat(); break;
				case SDLK_1:      demoMode(!autoplay); break;
				default: break;
			}
			break;
		
		#ifndef PC
		case SDL_JOYHATMOTION:
			//std::clog << SDL_GetTicks() << " (" << this << "): Joystick hat moved!" << std::endl;
			
			//The Wiimote hat directions are correct when holding
			//it "NES" style, not "Pointer" style.
			//
			//This does not work correctly with the classic controller
			//since now pressing UP on it causes a RIGHT move :(
			switch(event->jhat.value) {
				case SDL_HAT_RIGHT: move( 0, 1); break;
				case SDL_HAT_LEFT:  move( 0,-1); break;
				case SDL_HAT_UP:    move( 1, 0); break;
				case SDL_HAT_DOWN:  move(-1, 0); break;
			}
			break;
		
		case SDL_JOYBUTTONDOWN:
			//std::clog << SDL_GetTicks() << " (" << this << "): Joystick button down!" << std::endl;
			switch(event->jbutton.button) {
				case 0: //Wiimote A button
				case 9: //Classic controller A button
					select();
					break;
				case 1: //Wiimote B button
				case 10: //Classic controller B button
					cheat();
					break;
				case 2: //Wiimote 1 button
				case 11: //Classic controller X button
					demoMode(!autoplay);
					break;
				default:
					break;
			}
			break;
		#endif
		
		case SDL_USEREVENT:
			//std::clog << SDL_GetTicks() << " (" << this << "): LightsOutGame gracefully stopping." << std::endl;
			stop();
			break;
		
		case SDL_QUIT:
			//std::clog << SDL_GetTicks() << " (" << this << "): LightsOutGame quitting NOW." << std::endl;
			kill();
			break;
		
		default: break;
	}
}


void LightsOutGame::getMoveHint(unsigned int& suggestedX, unsigned int& suggestedY) const {
	bool won = winningState();
	if (won) {
		std::clog << SDL_GetTicks() << " (" << this << "): Game has already been won, providing bogus hint of (0,0)." << std::endl;
		suggestedX = 0;
		suggestedY = 0;
		return;
	}
	
	SDL_mutexP(paintMutex);
	bool moveAvailable = false;
	for (unsigned int x=0; x<lights->getWidth(); x++) {
		for (unsigned int y=0; y<lights->getHeight(); y++) {
			moveAvailable = moveAvailable || lights->getTile(x,y)->object->shouldPress();
		}
	}
	SDL_mutexV(paintMutex);
	if (!moveAvailable) {
		std::clog << SDL_GetTicks() << " (" << this << "): Game not won, yet no move available?? Providing bogus hint of (0,0)." << std::endl;
		suggestedX = 0;
		suggestedY = 0;
		return;
	}
	
	SDL_mutexP(paintMutex);
	int x=0;
	int y=0;
	do {
		x = rand()%lights->getWidth();
		y = rand()%lights->getHeight();
	} while (!(lights->getTile(x,y)->object->shouldPress()));
	SDL_mutexV(paintMutex);
	
	suggestedX = x;
	suggestedY = y;
	
	return;
}


void LightsOutGame::move(int deltaX, int deltaY) {
	SDL_mutexP(paintMutex);
	int newX = x+deltaX;
	int newY = y+deltaY;
	SDL_mutexV(paintMutex);
	
	if (newX < 0) newX = 0;
	if (newY < 0) newY = 0;	
	if (newX >= lights->getWidth()) newX = lights->getWidth()-1;
	if (newY >= lights->getHeight()) newY = lights->getHeight()-1;
	
	moveAbsolute(newX, newY);
}


void LightsOutGame::moveAbsolute(unsigned int x, unsigned int y) {
	SDL_mutexP(paintMutex);
	this->x = x;
	this->y = y;
	
	dirty = true;
	SDL_mutexV(paintMutex);
}


bool LightsOutGame::paint(SDL_Surface& surface, unsigned int width, unsigned int height) const {
	SDL_mutexP(paintMutex);
	
	if (dirty ||
	    this->surface == NULL ||
	    this->surface->w != width ||
	    this->surface->h != height) {
		SDL_FreeSurface(this->surface);
		this->surface = SDL_CreateRGBSurface(SDL_HWSURFACE,width,height,32,0,0,0,0);
		dirty = true;
	}
	
	SDL_Rect dest;
	bool dirtysub = false;
	
	//Create & paint gameboard subsurface
	///////////////////////////////////////////////////
	Uint32 rmask,gmask,bmask,amask;
	#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	rmask = 0xff000000;
	gmask = 0x00ff0000;
	bmask = 0x0000ff00;
	amask = 0x000000ff;
	#else
	rmask = 0x000000ff;
	gmask = 0x0000ff00;
	bmask = 0x00ff0000;
	amask = 0xff000000;
	#endif

	SDL_Surface* gameboard = SDL_CreateRGBSurface(SDL_HWSURFACE,this->surface->h,this->surface->h,32,rmask,gmask,bmask,amask);
	double tileWidth  = (double)gameboard->w/(double)lights->getWidth();
	double tileHeight = (double)gameboard->h/(double)lights->getHeight();
	
	//Paint lights onto gameboard
	for (unsigned int x=0; x<lights->getWidth(); x++) {
		dest.x = (int)(tileWidth*x);
		dest.w = (int)(tileWidth*(x+1)) - dest.x;
		for (unsigned int y=0; y<lights->getHeight(); y++) {
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
		dest.x = (int)(tileWidth*((int)(this->x)-1));
		dest.y = (int)(tileHeight*((int)(this->y)-1));
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
		dest.y = 0;
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
				
		dest.y += 64;
		SDL_Surface* timeLS = TTF_RenderText_Blended(font, "Time:", clrFg);
		SDL_BlitSurface(timeLS, NULL, this->surface, &dest);
		SDL_FreeSurface(timeLS);
		
		dest.y += 32;
		Uint32 elapsed = SDL_GetTicks() - gameStartTime;
		std::stringstream timeString;
		timeString << std::setw(2) << std::setfill('0') << elapsed/3600000 << ":"
		           << std::setw(2) << std::setfill('0') << elapsed/60000 % 60 << ":"
		           << std::setw(2) << std::setfill('0') << elapsed/1000 % 60;
		SDL_Surface* timeS = TTF_RenderText_Blended(font, timeString.str().c_str(), clrFg);
		SDL_BlitSurface(timeS, NULL, this->surface, &dest);
		SDL_FreeSurface(timeS);
		
		if (autoplay) {
			SDL_Color apClr = {128,128,255,0};
			dest.y += 64;
			SDL_Surface* autoplayLS = TTF_RenderText_Blended(font, "DEMO MODE", apClr);
			SDL_BlitSurface(autoplayLS, NULL, this->surface, &dest);
			SDL_FreeSurface(autoplayLS);
		}
				
		dest.y = this->surface->h - 96;
		SDL_Surface* diffLS = TTF_RenderText_Blended(font, "Color Map:", clrFg);
		SDL_BlitSurface(diffLS, NULL, this->surface, &dest);
		SDL_FreeSurface(diffLS);
		
		/*
		dest.y += 32;
		std::stringstream difficultyText;
		difficultyText << states << " States";
		SDL_Surface* diffS = TTF_RenderText_Blended(font, difficultyText.str().c_str(), clrFg);
		SDL_BlitSurface(diffS, NULL, this->surface, &dest);
		SDL_FreeSurface(diffS);
		*/
		dest.y += 48;
		for (unsigned int i=0; i<states; i++) {
			Light* l = new Light(states);
			for (int j=0; j<i; j++)
				l->nextState();
			
			SDL_Surface subsurface;
			l->paint(subsurface, 24, 24);
			SDL_BlitSurface(&subsurface, NULL, this->surface, &dest);
			dest.x += 27;
			delete l;
		}
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


void LightsOutGame::pressButton(unsigned int x, unsigned int y) {
	if (x >= lights->getWidth() || y >= lights->getHeight())
		throw 11;
	
	moves++;
	
	lights->getTile(x,y)->object->press();
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
		SDL_mutexP(paintMutex);
		dirty = true;
		bool automove = autoplay;
		SDL_mutexV(paintMutex);
		if (automove) {
			unsigned int newX, newY;
			getMoveHint(newX, newY);
			moveAbsolute(newX, newY);
			select();
		}
		yield(250);
	};
	
	EventPublisher::getInstance().removeEventObserver(this);
}


void LightsOutGame::select() {
	SDL_mutexP(paintMutex);
	pressButton(x,y);
	SDL_mutexV(paintMutex);
}


void LightsOutGame::toggleLight(unsigned int x, unsigned int y) {
	if (x >= lights->getWidth() || y >= lights->getHeight())
		return; //Assume the caller was just lazy
	
	lights->getTile(x,y)->object->nextState();
}


bool LightsOutGame::winningState() const {
	SDL_mutexP(paintMutex);
	for (unsigned int y=0; y<lights->getHeight(); y++) {
		for (unsigned int x=0; x<lights->getWidth(); x++) {
			if (lights->getTile(x,y)->object->isLightOn()) {
				SDL_mutexV(paintMutex);
				return false;
			}
			
		}
	}
	SDL_mutexV(paintMutex);
	return true;
}


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
#include <string.h>
#include <time.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_keysym.h>
#include "EventPublisher.hpp"
#include "LightsOutGameManager.hpp"


SDL_Surface* LightsOutGameManager::pointerTexture = NULL;


TTF_Font* LightsOutGameManager::font = NULL;


LightsOutGameManager::LightsOutGameManager() {
	//std::clog << SDL_GetTicks() << " (" << this << "): new LightsOutGameManager." << std::endl;
	
	srand ( time(NULL) );
	level = 2;
	gamesCompleted = 0;
	managerStartTime = 0;
	this->game = NULL;
	autoplay = false;
	
	paintMutex = SDL_CreateMutex();
	dirty = true;
	uid_dirty = true;
	surface = NULL;
	uid_surface = NULL;
	
	uid = (rand()%256 << 16) | (rand()%256 << 8) | (rand()%256);
	
	if (pointerTexture == NULL) {
		FILE* file = fopen("generic_point.png", "r");
		pointerTexture = IMG_Load_RW(SDL_RWFromFP(file, 0), 1);
		fclose(file);
		if (pointerTexture == NULL) {
			std::cerr << "Error loading generic_point.png: " << SDL_GetError() << std::endl;
			throw 1;
		}
	}
	
	if (font == NULL) {
		//font = TTF_OpenFont("yukari.ttf", 24);
		FILE* file = fopen("yukari.ttf", "r");
		font = TTF_OpenFontRW(SDL_RWFromFP(file, 0), 1, 24);
		//fclose(file);  //File must remain open for SDL_TTF to use the font
		if (font == NULL) {
			std::cerr << "Error loading yukari.ttf: " << SDL_GetError() << std::endl;
			throw 1;
		}
	}
}


LightsOutGameManager::~LightsOutGameManager() {
	//std::clog << SDL_GetTicks() << " (" << this << "): delete LightsOutGameManager." << std::endl;
	
	SDL_FreeSurface(surface);
	SDL_DestroyMutex(paintMutex);
	//Do not close font on destruction since its static
}


void LightsOutGameManager::eventOccured(const SDL_Event* const event) {
	switch (event->type) {
		case SDL_KEYDOWN:
			switch(event->key.keysym.sym) {
				case SDLK_ESCAPE:
				case SDLK_HOME:
					SDL_Event die;
					die.user.type = SDL_USEREVENT;
					SDL_PushEvent(&die);
					break;
				case SDLK_PAGEUP:
				case SDLK_PLUS:
					level++;
					if (level > 7) level = 7;
					newGame = true;
					break;
				case SDLK_PAGEDOWN:
				case SDLK_MINUS:
					level--;
					if (level < 2) level = 2;
					newGame = true;
					break;
				case SDLK_1:
					autoplay = !autoplay;
					break;
				default:
					break;
			}
			break;
		
		case SDL_MOUSEMOTION: {
			SDL_mutexP(paintMutex);
			mouseX = event->motion.x;
			mouseY = event->motion.y;
			dirty = true;
			SDL_mutexV(paintMutex);
		}
		
		case SDL_USEREVENT:
			//std::clog << SDL_GetTicks() << " (" << this << "): LightsOutGameManager gracefully stopping." << std::endl;
			stop();
			break;
		
		case SDL_QUIT:
			//std::clog << SDL_GetTicks() << " (" << this << "): LightsOutGameManager quitting NOW." << std::endl;
			kill();
			break;
		
		default:
			break;
	}
}


bool LightsOutGameManager::paint(SDL_Surface& surface, unsigned int width, unsigned int height, unsigned int type) const {
	while (this->game == NULL) {
		//std::clog << SDL_GetTicks() << " (" << this << "): No game yet..." << std::endl;
		yield(100);
	}
	
	SDL_mutexP(paintMutex);
	
	if (type == PAINT_NORMAL && (dirty ||
	    this->surface == NULL ||
	    this->surface->w != width ||
	    this->surface->h != height)) {
		SDL_FreeSurface(this->surface);
		this->surface = SDL_CreateRGBSurface(SDL_HWSURFACE,width,height,32,0,0,0,0);
		dirty = true;
	}
	
	if (type == PAINT_UID && (uid_dirty ||
	    this->uid_surface == NULL ||
	    this->uid_surface->w != width ||
	    this->uid_surface->h != height)) {
		SDL_FreeSurface(this->uid_surface);
		this->uid_surface = SDL_CreateRGBSurface(SDL_SWSURFACE,width,height,32,0,0,0,0);
		uid = SDL_MapRGB(this->uid_surface->format, (Uint8)((uid & 0x00FF0000) >> 16), (Uint8)((uid & 0x0000FF00) >> 8), (Uint8)(uid & 0x000000FF));
		SDL_FillRect(this->uid_surface, NULL, uid);
		uid_dirty = true;
	}
	
	SDL_Surface* target = NULL;
	switch (type) {
		case PAINT_NORMAL: target = this->surface; break;
		case PAINT_UID:    target = this->uid_surface; break;
	}
	SDL_Rect dest;
	bool dirtysub = false;
	
	//Create & paint game surface
	///////////////////////////////////////////////////
	dest.x = 8;
	dest.y = 16;
	dest.w = target->w - 2*dest.x;
	dest.h = target->h - dest.y - 48;
	SDL_Surface gameSurface;
	bool dirtyPaint = game->paint(gameSurface, dest.w, dest.h, type);
	dirtysub |= dirtyPaint;
	if ((dirty || dirtyPaint) && type == PAINT_NORMAL)
		SDL_BlitSurface(&gameSurface, NULL, target, &dest);
	if ((uid_dirty || dirtyPaint) && type == PAINT_UID)
		SDL_BlitSurface(&gameSurface, NULL, target, &dest);
		
	//Paint stats onto surface
	///////////////////////////////////////////////////
	if (dirty && type == PAINT_NORMAL) {
		SDL_Color clrFg = {255,255,255,0};
		dest.x = 16;
		dest.y = target->h - 48;
		dest.w = 0;
		dest.h = 0;
		
		std::stringstream gamesString;
		gamesString << "Games Completed: " << gamesCompleted;
		SDL_Surface* gamesLS = TTF_RenderText_Blended(font, gamesString.str().c_str(), clrFg);
		SDL_BlitSurface(gamesLS, NULL, target, &dest);
		SDL_FreeSurface(gamesLS);
		
		dest.x = target->w - 230;
		Uint32 elapsed = SDL_GetTicks() - managerStartTime;
		std::stringstream timeString;
		timeString << "Total Time: " << std::setw(2) << std::setfill('0') << elapsed/3600000 << ":"
		           << std::setw(2) << std::setfill('0') << elapsed/60000 % 60 << ":"
		           << std::setw(2) << std::setfill('0') << elapsed/1000 % 60;
		SDL_Surface* timeLS = TTF_RenderText_Blended(font, timeString.str().c_str(), clrFg);
		SDL_BlitSurface(timeLS, NULL, target, &dest);
		SDL_FreeSurface(timeLS);
	}
	
	// Paint mouse cursor onto surface
	///////////////////////////////////////////////////
	//dest.x = mouseX-48;
	//dest.y = mouseY-48;
	//SDL_SetAlpha(target, SDL_SRCALPHA, 0);
	//SDL_BlitSurface(pointerTexture, NULL, target, &dest);
	
	//Set surface and return
	///////////////////////////////////////////////////
	surface = *target;
	
	switch (type) {
		case PAINT_NORMAL:
			dirtysub |= dirty;
			dirty = false;
			break;
		
		case PAINT_UID:
			dirtysub |= uid_dirty;
			uid_dirty = false;
			break;
	}
	
	SDL_mutexV(paintMutex);
	return dirtysub;
}


void LightsOutGameManager::run() {
	EventPublisher::getInstance().addEventObserver(this);
	
	managerStartTime = SDL_GetTicks();
	
	while (runThread) {
		SDL_mutexP(paintMutex);
		dirty = true;
		if (this->game != NULL) {
			this->game->kill();
			delete this->game;
		}
		
		this->game = new LightsOutGame(5,5,level,autoplay);
		SDL_mutexV(paintMutex);
		
		//Start the game and wait for it to get over
		//I don't just join() the thread though since I still
		//want to make myself dirty every once in a while :)
		newGame = false;
		this->game->start();
		while (runThread && !newGame && !this->game->winningState()) {
			SDL_mutexP(paintMutex);
			dirty = true;
			SDL_mutexV(paintMutex);
			yield(250);
		}
		this->game->stop();
		
		if (!newGame)
			gamesCompleted++;
	};
	
	EventPublisher::getInstance().removeEventObserver(this);
}


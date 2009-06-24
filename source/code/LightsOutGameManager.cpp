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
#include <SDL/SDL_keysym.h>
#include "EventPublisher.hpp"
#include "LightsOutGameManager.hpp"


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
	surface = NULL;
	dirty = true;
	
	if (font == NULL) {
		//font = TTF_OpenFont("yukari.ttf", 24);
		FILE* file = fopen("fonts/yukari.ttf", "r");
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


void LightsOutGameManager::demoMode(const bool enable) {
	SDL_mutexP(paintMutex);
	autoplay = enable;
	SDL_mutexV(paintMutex);
}


void LightsOutGameManager::eventOccured(const SDL_Event* const event) {
	switch (event->type) {
		case SDL_KEYDOWN:
			switch(event->key.keysym.sym) {
				case SDLK_ESCAPE:   quit(); break;
				case SDLK_PAGEUP:   levelChange(1); break;
				case SDLK_PAGEDOWN: levelChange(-1); break;
				case SDLK_1:        demoMode(!autoplay); break;
				default:
					break;
			}
			break;
		
		#ifndef PC
		case SDL_JOYBUTTONDOWN:
			//std::clog << SDL_GetTicks() << " (" << this << "): Joystick button down!" << std::endl;
			switch(event->jbutton.button) {
				case 6: //Wiimote Home button
				case 19: //Classic controller Home button
					quit();
					break;
				case 5: //Wiimote + button
				case 18: //Classic controller + button
					levelChange(1);
					break;
				case 4: //Wiimote - button
				case 17: //Classic controller - button
					levelChange(-1);
					break;
				case 1: //Wiimote B button
				case 11: //Classic controller X button
					demoMode(!autoplay);
					break;
				default:
					break;
			}
			break;
		#endif
		
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


void LightsOutGameManager::levelChange(const int delta, const bool restart) {
	SDL_mutexP(paintMutex);
	int newLevel = level+delta;
	SDL_mutexV(paintMutex);
	
	if (newLevel < 0) newLevel = 0;
	if (newLevel > 7) newLevel = 7;
	
	levelAbsolute(newLevel, restart);
}


void LightsOutGameManager::levelAbsolute(const unsigned int newLevel, const bool restart) {
	SDL_mutexP(paintMutex);
	level = newLevel;
	newGame = restart;
	SDL_mutexV(paintMutex);
}


bool LightsOutGameManager::paint(SDL_Surface& surface, unsigned int width, unsigned int height) const {
	while (this->game == NULL) {
		//std::clog << SDL_GetTicks() << " (" << this << "): No game yet..." << std::endl;
		yield(100);
	}
	
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
	
	//Create & paint game surface
	///////////////////////////////////////////////////
	dest.x = 8;
	dest.y = 16;
	dest.w = this->surface->w - 2*dest.x;
	dest.h = this->surface->h - dest.y - 48;
	SDL_Surface gameSurface;
	bool dirtyPaint = game->paint(gameSurface, dest.w, dest.h);
	dirtysub |= dirtyPaint;
	if (dirty || dirtyPaint)
		SDL_BlitSurface(&gameSurface, NULL, this->surface, &dest);
	
	
	//Paint stats onto surface
	///////////////////////////////////////////////////
	if (dirty) {
		SDL_Color clrFg = {255,255,255,0};
		dest.x = 16;
		dest.y = this->surface->h - 48;
		dest.w = 0;
		dest.h = 0;
		
		std::stringstream gamesString;
		gamesString << "Games Completed: " << gamesCompleted;
		SDL_Surface* gamesLS = TTF_RenderText_Blended(font, gamesString.str().c_str(), clrFg);
		SDL_BlitSurface(gamesLS, NULL, this->surface, &dest);
		SDL_FreeSurface(gamesLS);
		
		dest.x = this->surface->w - 230;
		Uint32 elapsed = SDL_GetTicks() - managerStartTime;
		std::stringstream timeString;
		timeString << "Total Time: " << std::setw(2) << std::setfill('0') << elapsed/3600000 << ":"
		           << std::setw(2) << std::setfill('0') << elapsed/60000 % 60 << ":"
		           << std::setw(2) << std::setfill('0') << elapsed/1000 % 60;
		SDL_Surface* timeLS = TTF_RenderText_Blended(font, timeString.str().c_str(), clrFg);
		SDL_BlitSurface(timeLS, NULL, this->surface, &dest);
		SDL_FreeSurface(timeLS);
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


void LightsOutGameManager::quit() {
	SDL_Event die;
	die.user.type = SDL_USEREVENT;
	SDL_PushEvent(&die);
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


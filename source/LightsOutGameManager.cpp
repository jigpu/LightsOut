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
	std::clog << SDL_GetTicks() << " (" << this << "): new LightsOutGameManager." << std::endl;
	
	srand ( time(NULL) );
	level = 2;
	gamesCompleted = 0;
	managerStartTime = 0;
	this->game = NULL;
	
	paintMutex = SDL_CreateMutex();
	surface = NULL;
	dirty = true;
	
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 512) != 0) {
		std::cerr << "Unable to open audio: " << Mix_GetError() << std::endl;
		throw 1;
	}
	
	if (font == NULL) {
		font = TTF_OpenFont("yukari.ttf", 24);
		if (font == NULL) {
			std::cerr << "Error loading fawn.ttf: " << SDL_GetError() << std::endl;
			throw 1;
		}
	}
}


LightsOutGameManager::~LightsOutGameManager() {
	std::clog << SDL_GetTicks() << " (" << this << "): delete LightsOutGameManager." << std::endl;
	
	SDL_FreeSurface(surface);
	SDL_DestroyMutex(paintMutex);
	Mix_HaltMusic();
	Mix_CloseAudio();
}


void LightsOutGameManager::eventOccured(SDL_Event* event) {
	switch (event->type) {
		case SDL_KEYDOWN: {
			switch(event->key.keysym.sym) {
				case SDLK_ESCAPE:
				case SDLK_HOME:
					SDL_Event die;
					die.quit.type = SDL_QUIT;
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
			}
			break;
		}
		
		case SDL_QUIT: {
			stop();
			break;
		} 
	}
}


bool LightsOutGameManager::paint(SDL_Surface& surface, int width, int height) {
	while (this->game == NULL) {
		std::clog << SDL_GetTicks() << " (" << this << "): No game yet..." << std::endl;
		yield(100);
	}
	
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
	
	//Create & paint game surface
	///////////////////////////////////////////////////	
	dest.x = 8;
	dest.y = 8;
	dest.w = this->surface->w - 2*dest.x;
	dest.h = this->surface->h - dest.y - 42;
	SDL_Surface gameSurface;
	bool dirtyPaint = game->paint(gameSurface, dest.w, dest.h);
	dirtysub |= dirtyPaint;
	if (dirty || dirtyPaint)
		SDL_BlitSurface(&gameSurface, NULL, this->surface, &dest);
	
	
	//Paint stats onto surface
	///////////////////////////////////////////////////
	if (dirty) {		
		SDL_Color clrFg = {255,255,255,0};
		dest.x = 4;
		dest.y = this->surface->h - 32;
		dest.w = 0;
		dest.h = 0;
		
		std::stringstream gamesString;
		gamesString << "Games Completed: " << gamesCompleted;
		SDL_Surface* gamesLS = TTF_RenderText_Blended(font, gamesString.str().c_str(), clrFg);
		SDL_BlitSurface(gamesLS, NULL, this->surface, &dest);
		SDL_FreeSurface(gamesLS);
		
		dest.x = this->surface->w - 230;
		int elapsed = SDL_GetTicks() - managerStartTime;
		std::stringstream timeString;
		timeString << "Total Time: " << std::setw(2) << std::setfill('0') << elapsed/36000000 << ":"
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


void LightsOutGameManager::run() {
	EventPublisher::getInstance().addEventObserver(this);
	
	managerStartTime = SDL_GetTicks();
	
	music = Mix_LoadMUS("bgm.mp3");
	Mix_PlayMusic(music, -1);
	
	while (runThread) {
		SDL_mutexP(paintMutex);
		dirty = true;
		if (this->game != NULL)
			delete this->game;
		
		this->game = new LightsOutGame(5,5,level);
		SDL_mutexV(paintMutex);
		
		//Start the game and wait for it to get over
		//I don't just join() the thread though since I still
		//want to make myself dirty every once in a while :)
		newGame = false;
		this->game->start();
		while (runThread && !newGame && !this->game->winningState()) {
			dirty = true;
			yield(250);
		}
		this->game->stop();
		
		if (!newGame)
			gamesCompleted++;
	};
	
	std::cout << "Thanks for playing!" << std::endl;
	
	EventPublisher::getInstance().removeEventObserver(this);
}


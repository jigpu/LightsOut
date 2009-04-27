#include <SDL/SDL.h>
#include "LightsOutView.hpp"


LightsOutView::LightsOutView(LightsOutGame* game) {
	this->game = game;
}


void LightsOutView::paint(SDL_Surface* surface) {
	//SDL_Surface *image;
	//SDL_Surface *temp;
	SDL_Rect rect;
	
	for (int x=-1; x < game->getWidth(); x++) {
		for (int y=-1; y < game->getHeight(); y++) {
			rect.x = 64*(x+1);
			rect.y = 64*(y+1);
			rect.w = 64;
			rect.h = 64;
			
			if (y == -1) {
				//Top row. Write out column letters.
				//temp = NULL;
			}
			else if (x == -1) {
				//Left column, non-top row. Write out row numbers.
				//temp = NULL;
			}
			else if (x == this->x && y == this->y) {
				//Highlighted tile
				//char* filename = "lightO.png";
				//temp = IMG_Load(filename);
				if (game->getTile(x,y)->object.getState() == 0)
					SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, 255, 128, 255));
				else
					SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, 128, 128, 255));
			}
			else {
				//Light tile. Display tile image.
				//char* filename = "light" + game->getState() + ".png";
				//temp = IMG_Load(filename);
				if (game->getTile(x,y)->object.getState() == 0)
					SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, 128, 128, 128));
				else
					SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, 128, 255, 128));
			}
			/*
			if (temp == NULL) {
				printf("Unable to load bitmap: %s\n", SDL_GetError());
				return;
			}
			
			image = SDL_DisplayFormat(temp);
			SDL_FreeSurface(temp);
			
			SDL_Rect src, dest;
 			
			src.x = 0;
			src.y = 0;
			src.w = image->w;
			src.h = image->h;
		
			dest.x = 64*(x+1);
			dest.y = 64*(y+1);
			dest.w = image->64;
			dest.h = image->64;
		
			SDL_BlitSurface(image, &src, surface, &dest);
			*/
		}
	}
}


void LightsOutView::select(int x, int y) {
	this->x = x;
	this->y = y;
}


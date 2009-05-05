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


#ifndef __RectangleMap_tpp__
#define __RectangleMap_tpp__


#include "Map.tpp"


/**
 * A RectangleMap is a type of Map where the center of tiles are
 * placed on a rectangular grid. Given rectangular tiles, this
 * reproduces the layout of many simple (and not-so-simple) games.
 */
template <typename T>
class RectangleMap : public Map<T> {

protected:
	int width, height;
	
public:
	RectangleMap(int width, int height) {
		this->width  = width;
		this->height = height;
		
		this->tileRows = new Tile<T>**[height];
		for (int y=0; y<height; y++)
			this->tileRows[y] = new Tile<T>*[width];
	}
	
	
	~RectangleMap() {
		for (int y=0; y<height; y++)
			delete (this->tileRows[y])[width];
		delete (this->tileRows)[height];
	}
	
	
	int getWidth() {
		return width;
	}
	
	
	int getHeight() {
		return height;
	}
	
};


#endif


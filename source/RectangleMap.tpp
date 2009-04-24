#ifndef __RectangleMap_tpp__
#define __RectangleMap_tpp__


#include "Map.tpp"


/**
 * A RectangleMap is a type of Map where the center of tiles are placed
 * on a rectangular grid. Given rectangular tiles, this reproduces the
 * layout of many simple (and not-so-simple) games.
 */

template <typename T>
class RectangleMap : public Map<T> {

protected:
	int width, height;
	int deltaX, deltaY;


public:	
	RectangleMap(int width, int height, int deltaX, int deltaY) {
		this->width  = width;
		this->height = height;
		this->deltaX = deltaX;
		this->deltaY = deltaY;
		
		this->tileRows = new Tile<T>**[height];
		for (int y=0; y<height; y++)
			this->tileRows[y] = new Tile<T>*[width];
	}
	
	~RectangleMap() {
		for (int y=0; y<height; y++)
			delete (this->tileRows[y])[width];
		delete (this->tileRows)[height];
	}

};


#endif


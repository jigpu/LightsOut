#ifndef __Map_tpp__
#define __Map_tpp__


#include "Tile.tpp"


/**
 * A Map is a two-dimensional layout of tiles. The actual interlocking
 * of tiles is left to subclasses.
 * 
 * Note that many games are more easily implemented with several "layers" of
 * Maps. For example, a simple stragegy game may contain three seperate maps:
 *
 *    + World Map: Tiles contain "Terrain" tiles that display a graphic
 *         and have associated stat modifiers.
 *
 *    + Overlay Map: Tiles contain a semi-transparent bitmap to show the
 *         area-of-effect for an action.
 *
 *    + Player Map: Tiles contain "Player" objects that define interacting
 *         characters.
 */

template <typename T>
class Map {

protected:
	Tile<T>*** tileRows;


public:	
	Tile<T>* getTile(int x, int y) {
		return (tileRows[y])[x];
	}
	
	void setTile(int x, int y, Tile<T>* tile) {
		(tileRows[y])[x] = tile;
	}

};


#endif


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


#ifndef __Map_tpp__
#define __Map_tpp__


#include "Tile.tpp"


/**
 * A Map is a two-dimensional layout of tiles. The actual
 * interlocking of tiles is left to subclasses.
 * 
 * Note that many games are more easily implemented with several
 * "layers" of Maps. For example, a simple stragegy game may contain
 * three seperate maps:
 *
 *    + World Map: Tiles contain "Terrain" tiles that display a
 *         graphic and have associated stat modifiers.
 *
 *    + Overlay Map: Tiles contain a semi-transparent bitmap to show
 *         the area-of-effect for an action.
 *
 *    + Player Map: Tiles contain "Player" objects that define
 *         interacting characters.
 */
template <typename T>
class Map {

protected:
	/**
	 * A 2D "array" of tile pointers. Although it has a logical
	 * cartesian layout, this is just a convention and subclasses
	 * need not feel bound to being cartesian maps.
	 */
	Tile<T>*** tileRows;
	
public:
	/**
	 * Returns the tile at (X,Y). The coordinate space and
	 * meaning of X and Y are dependent on the specific subclass.
	 */
	Tile<T>* getTile(int x, int y) {
		return (tileRows[y])[x];
	}
	
	
	/**
	 * Sets the tile at (X,Y) to the given object. The coordinate
	 * space and meaning of X and Y are dependent on the specific
	 * subclass.
	 */
	void setTile(int x, int y, Tile<T>* tile) {
		(tileRows[y])[x] = tile;
	}

};


#endif


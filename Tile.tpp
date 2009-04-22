#ifndef __Tile_tpp__
#define __Tile_tpp__


/**
 * A Tile is a container for objects of type T that can be placed onto
 * a Map. Tiles may contain Player objects, Terrain objects, etc as
 * needed.
 */

template <typename T>
class Tile {

public:
	T object;

};


#endif


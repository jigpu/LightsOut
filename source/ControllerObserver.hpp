#ifndef __ControllerObserver_hpp__
#define __ControllerObserver_hpp__


#include <SDL/SDL.h>


class ControllerObserver {

public:
	virtual void controllerAction(int type, SDLKey* value) = 0;
};


#endif

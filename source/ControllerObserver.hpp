#ifndef __ControllerObserver_hpp__
#define __ControllerObserver_hpp__


class ControllerObserver {

public:
	virtual void controllerAction(int type, int value) = 0;
};


#endif

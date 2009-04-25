#ifndef __Controller_hpp__
#define __Controller_hpp__


#include "ControllerObserver.hpp"
#include "Thread.hpp"


#define KEYBOARD_SINGLE 0
#define KEYBOARD_DOUBLE 1
#define WIIMOTE_BUTTON  2

class Controller : public Thread {

public:
	ControllerObserver* observer;
	
	/**
	 * Entry point for a new thread of execution. This method
	 * periodically checks the controller for any new input
	 * and forwards the date to the observer.
	 */
	virtual void run() = 0;

};


#endif

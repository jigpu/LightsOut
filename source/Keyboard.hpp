#ifndef __Keyboard_hpp__
#define __Keyboard_hpp__


#include <termios.h>
#include "Controller.hpp"

class Keyboard : public Controller {

protected:
	struct termios initial_settings, new_settings;

public:
	Keyboard();
	
	~Keyboard();
	
	void run();
};


#endif

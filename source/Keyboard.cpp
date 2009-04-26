#include <iostream>
#include "Controller.hpp"
#include "Keyboard.hpp"


using namespace std;


Keyboard::Keyboard() {
	tcgetattr(0, &initial_settings);
	new_settings = initial_settings;
	new_settings.c_lflag &= ~ICANON;
	new_settings.c_lflag &= ~ECHO;
	new_settings.c_lflag &= ~ISIG;
	new_settings.c_cc[VMIN]  &= 1;
	new_settings.c_cc[VTIME] &= 0;
	tcsetattr(0, TCSANOW, &new_settings);
}


Keyboard::~Keyboard() {
	tcsetattr(0, TCSANOW, &initial_settings);
}


void Keyboard::run() {
	while (true) {
		usleep(20000);
		
		int c = cin.get();
		if (c != 0x00 && c != 0xE0)
			observer->controllerAction(KEYBOARD_SINGLE, c);
		else
			observer->controllerAction(KEYBOARD_DOUBLE, cin.get());
	}
}


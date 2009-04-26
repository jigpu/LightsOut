#include <wiiuse/wpad.h>
#include "Wiimote.hpp"


Wiimote::Wiimote() {
	// This function initialises the attached controllers
	WPAD_Init();
}
	

void Wiimote::run() {
	while (true) {
		//usleep(25000);
		LWP_YieldThread();
		
		WPAD_ScanPads();
		u32 pressed = WPAD_ButtonsDown(0);
		//int pressed = WPAD_ButtonsDown(0);
		
		if (!pressed)
			continue;
		else
			observer->controllerAction(WIIMOTE_BUTTON, pressed);
	}
}


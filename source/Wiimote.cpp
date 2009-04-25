#include <wiiuse/wpad.h>
#include "Wiimote.hpp"


Wiimote::Wiimote() {
	// This function initialises the attached controllers
	WPAD_Init();
}
	

void Wiimote::run() {
	while (true) {
		//Thread sleep.
		
		WPAD_ScanPads();
		//u32 pressed = WPAD_ButtonsDown(0);
		int pressed = WPAD_ButtonsDown(0);
		
		if (!pressed)
			continue;
		
		observer->controllerAction(WIIMOTE_BUTTON, pressed);
	}
}


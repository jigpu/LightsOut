//#include <gccore.h>
#include "LightsOutGameManager.hpp"
#include "Keyboard.hpp"
#include "Wiimote.hpp"

/*
void initVideo() {
	static void *xfb = NULL;
	static GXRModeObj *rmode = NULL;

	// Initialise the video system
	VIDEO_Init();

	// Obtain the preferred video mode from the system
	// This will correspond to the settings in the Wii menu
	rmode = VIDEO_GetPreferredMode(NULL);

	// Allocate memory for the display in the uncached region
	xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
	
	// Initialise the console, required for printf
	console_init(xfb,20,20,rmode->fbWidth,rmode->xfbHeight,rmode->fbWidth*VI_DISPLAY_PIX_SZ);
	
	// Set up the video registers with the chosen mode
	VIDEO_Configure(rmode);
	
	// Tell the video hardware where our display memory is
	VIDEO_SetNextFramebuffer(xfb);
	
	// Make the display visible
	VIDEO_SetBlack(FALSE);

	// Flush the video register changes to the hardware
	VIDEO_Flush();

	// Wait for Video setup to complete
	VIDEO_WaitVSync();
	if(rmode->viTVMode&VI_NON_INTERLACE) VIDEO_WaitVSync();
}
*/


int main(int argc, char** argv) {
	//initVideo();
	
	Keyboard* controller = new Keyboard();
	LightsOutGameManager* game = new LightsOutGameManager();
	
	controller->observer = game;
	
	controller->start();
	game->start();
	game->join();
	
	//Explicit destruction, since the Keyboard controller
	//tweaks the console and only resets it in the
	//destructor (which isn't called by default?!)
	delete(game);
	delete(controller);
	
	return 0;
}


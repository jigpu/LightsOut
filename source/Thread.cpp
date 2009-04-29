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


#include <iostream>
#import "Thread.hpp"


int executor(void* val) {
	Thread* obj = (Thread*)val;
	obj->run();
	return NULL;
}


void Thread::join() {
	//pthreads:
	//pthread_join(thread, NULL);
	
	//SDL:
	SDL_WaitThread(thread, NULL);
}


void Thread::kill() {
	//SDL:
	runThread = false;
	SDL_KillThread(thread);
}


void Thread::start() {
	runThread = true;
	
	//pthreads:
	//pthread_create(thread, NULL, executor, (void*)this);
	
	//SDL:
	thread = SDL_CreateThread(executor, (void*)this);
}


void Thread::stop() {
	runThread = false;
}


void Thread::yield(int milliseconds = 1) {
	//pthreads:
	//usleep(100*milliseconds);
	
	//SDL:
	SDL_Delay(milliseconds);
}


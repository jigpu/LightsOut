#ifndef __Thread_hpp__
#define __Thread_hpp__


#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>


class Thread {

protected:
	//pthread_t*  thread;
	SDL_Thread* thread;

public:
	void join();
	virtual void run() = 0;
	void start();
	void yield(int milliseconds);

};


#endif

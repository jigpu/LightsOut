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


#ifndef __Thread_hpp__
#define __Thread_hpp__


#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>


/**
 * Classes which may be run as a seperate thread can extend this
 * class to obtain some convienient methods. Note that if using SDL
 * and the EventPublisher, Threads should also extend EventObserver
 * and register themselves with the EventPublisher to recieve the
 * SDL_QUIT event and stop running.
 */
class Thread {

protected:
	/**
	 * This boolean will remain true while the thread should be
	 * running. When the stop() method is called, this will
	 * change to false, and the thread should stop. Note that
	 * threads which perform lengthy operations should check this
	 * value often since they should stop soon after the original
	 * call.
	 */
	bool runThread;
	
	/**
	 * A reference to the system's thread object
	 */
	//pthread_t*  thread;
	SDL_Thread* thread;
	
public:
	/**
	 * Join the thread. This call will not return until the
	 * thread has stopped running.
	 */
	void join();
	
	/**
	 * Kill the thread. This forcefully causes the thread to
	 * end. This should be used with great care -- the stop
	 * method should be prefered in almost all cases.
	 */
	void kill();
	
	/**
	 * Threads must override this method with their own
	 * implementation. The thread is considered running while
	 * this method is active. This method should not be called
	 * by others, use the start method instead.
	 */
	virtual void run() = 0;
	
	/**
	 * Start the thread. This creates a new thread in the system
	 * and begins execution from the run method.
	 */
	void start();
	
	/**
	 * Notify the thread that it should stop running. Threads
	 * should check if they are to continue running often,
	 * and a call to this should be all they need. Threads
	 * which continue running for some time after calling this
	 * may need to be forcibly killed, but note that using kill
	 * may have unwanted reprocussions.
	 */
	void stop();
	
	/**
	 * Cause the thread to pause execution for the specified
	 * number of milliseconds.
	 */
	void yield(int milliseconds);
	
};


#endif


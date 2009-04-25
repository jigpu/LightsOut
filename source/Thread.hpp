#ifndef __Thread_hpp__
#define __Thread_hpp__


#include <pthread.h>


class Thread {

protected:
	pthread_t thid;
	int ret;

public:
	void join();
	virtual void run() = 0;
	void start();

};


#endif

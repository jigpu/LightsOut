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


void Thread::start() {
	//pthreads:
	//pthread_create(thread, NULL, executor, (void*)this);
	
	//SDL:
	thread = SDL_CreateThread(executor, (void*)this);
}


void Thread::yield(int milliseconds = 1) {
	//pthreads:
	//usleep(100*milliseconds);
	
	//SDL:
	SDL_Delay(milliseconds);
}


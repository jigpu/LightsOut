#import "Thread.hpp"


void* executor(void* obj) {
	Thread* thread = (Thread*)obj;
	thread->run();
}


void Thread::join() {
	pthread_join(thid, NULL);
}


void Thread::start() {
	ret = pthread_create(&thid, NULL, executor, (void*)this);
}

#ifndef __Thread_hpp__
#define __Thread_hpp__


//#include <pthread.h>
#pragma once
#include <ogcsys.h>
#include <gccore.h>
#define STACKSIZE 8*1024
 
typedef lwp_t pthread_t;
typedef mutex_t pthread_mutex_t;
typedef void* pthread_mutexattr_t;
typedef int pthread_attr_t;
 
inline int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void*), void *arg);
//int pthread_cancel(pthread_t thread);
 
inline int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr);
inline int pthread_mutex_destroy(pthread_mutex_t *mutex);
inline int pthread_mutex_lock(pthread_mutex_t *mutex);
inline int pthread_mutex_unlock(pthread_mutex_t *mutex);
inline int pthread_join(pthread_t th, void **thread_return);
 
//imp
inline int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void*), void *arg)
{
	*thread = 0;
	return LWP_CreateThread(thread, start_routine, arg, 0, STACKSIZE, 64);
}
 
//int pthread_cancel(pthread_t thread);
 
inline int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr)
{
	return LWP_MutexInit(mutex, 0);
}
inline int pthread_mutex_destroy(pthread_mutex_t *mutex){ return LWP_MutexDestroy(*mutex);}
 
inline int pthread_mutex_lock(pthread_mutex_t *mutex)   { return LWP_MutexLock(*mutex);   }
inline int pthread_mutex_trylock(pthread_mutex_t *mutex){ return LWP_MutexTryLock(*mutex);}
inline int pthread_mutex_unlock(pthread_mutex_t *mutex) { return LWP_MutexUnlock(*mutex); }
inline int pthread_join(pthread_t th, void **thread_return) { return LWP_JoinThread(th, thread_return); }


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

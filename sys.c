
typedef void (*SYS_THREAD_PROC)(void *arg);
// SysThreadProc

#ifdef _WIN32

struct Thread {
	HANDLE handle;
};

Thread create_thread(LPTHREAD_START_ROUTINE func) {
	return CreateThread(0, 0, (LPTHREAD_START_ROUTINE)WorkerThreadProc, workerThreadPool, 0, &id);
}

bool compare_and_swap32(void *ptr, int old, int new) {
	volatile long o;
	volatile long n;
	o = old;
	n = new;

	return InterlockedCompareExchange((long volatile*)&gameMixSoundsThreadLock, new, old) == old;
}

void swap32(void *ptr, int swap) {
	
}

#endif

#ifdef __APPLE__

#include <pthread.h>

struct Thread {
	pthread_t handle;
};

// todo: is this safe?
SYS_THREAD_PROC _start_routine;
void *pthread_start_routine(void *arg) {
	_start_routine(arg);
	return arg;
}

Thread create_thread(SYS_THREAD_PROC func, void *arg) {
	Thread thread;
	_start_routine = func;
	pthread_create(&thread.handle, NULL, pthread_start_routine, arg);
	return thread;
}

void destroy_thread(Thread thread) {
	pthread_cancel(thread.handle);
}

int atomic_swap32(volatile int *ptr, int swap) {
	return __sync_lock_test_and_set(ptr, swap);
}

int atomic_compare_swap32(volatile int *ptr, int cmp, int swap) {
	// note: returns true if the new value was written
	return __sync_bool_compare_and_swap(ptr, cmp, swap);
}

int atomic_add32(volatile int *ptr, int value) {
	return __sync_fetch_and_add(ptr, value);
}

int atomic_sub32(volatile int *ptr, int value) {
	return __sync_fetch_and_sub(ptr, value);
}

int atomic_fetch32(volatile int *ptr) {
	return __sync_fetch_and_add(ptr, 0);
}

#endif
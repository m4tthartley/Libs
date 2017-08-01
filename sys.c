
#ifndef _GIANTJELLY_SYS_
#define _GIANTJELLY_SYS_

typedef void (*SYS_THREAD_PROC)(void *arg);
// SysThreadProc

#define atomic_swap atomic_swap32
#define atomic_compare_swap atomic_compare_and_swap32
#define atomic_add atomic_add32
#define atomic_sub atomic_sub32
#define atomic_fetch atomic_fetch32

#ifdef _WIN32

// Threads
struct Thread {
	HANDLE handle;
};

SYS_THREAD_PROC _start_routine;
DWORD WINAPI win32_start_routine(LPVOID arg) {
	_start_routine(arg);
	return 0;
}

Thread create_thread(SYS_THREAD_PROC func, void *arg) {
	Thread thread;
	_start_routine = func; // todo: creating threads is not thread safe
	DWORD id;
	thread.handle = CreateThread(0, 0, win32_start_routine, arg, 0, &id);
	return thread;
}

void destroy_thread(Thread thread) {
	TerminateThread(thread.handle, 0);
}

int atomic_swap32(void *ptr, int swap) {
	return _InterlockedExchange((long volatile*)ptr, swap);
}

bool atomic_compare_swap32(void *ptr, int cmp, int swap) {
	return _InterlockedCompareExchange((long volatile*)ptr, swap, cmp) == cmp;
}

int atomic_add32(void *ptr, int value) {
	return _InterlockedExchangeAdd((long volatile*)ptr, value);
}

int atomic_sub32(void *ptr, int value) {
	return _InterlockedExchangeAdd((long volatile*)ptr, -value);
}

int atomic_fetch32(void *ptr) {
	return _InterlockedExchangeAdd((long volatile*)ptr, 0);
}

// Dynamic libraries
struct DyLib {
	HMODULE handle;
};

DyLib load_dynamic_library(char *file) {
	DyLib lib;
	char path[MAX_PATH];
	snprintf(path, MAX_PATH-1, "%s.dll", file);
	lib.handle = LoadLibraryA(path);
	return lib;
}

void *load_library_proc(DyLib lib, char *proc) {
	return GetProcAddress(lib.handle, proc);
}

#endif

#ifdef __APPLE__

#include <pthread.h>

// Threads
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

// Dynamic libraries
struct DyLib {
	void *handle;
};

DyLib load_dynamic_library(char *file) {
	DyLib lib;
	char path[256];
	snprintf(path, 255, "%s.dylib", file);
	lib.handle = dlopen(path, RTLD_LAZY);
	return lib;
}

void *load_library_proc(DyLib lib, char *proc) {
	return dlsym(lib.handle, proc);
}

#endif

#endif
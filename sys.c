
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

void swap32(void *ptr, int new) {
	
}

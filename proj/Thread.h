#pragma once

typedef CRITICAL_SECTION CriticalSection;
typedef HANDLE PrimitiveSemaphore;
typedef HANDLE PrimitiveThread;
class CThread
{
public:
	CThread(void *(*where_to)(void*), void *parameter, int priority);
	~CThread();
	void join(void **status_p);
	static void exit(void *status);
	static void yield();
	static void sleep();
	static size_t self();
protected:
	PrimitiveThread _thread;
};

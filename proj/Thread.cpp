#include "StdAfx.h"
#include ".\thread.h"


typedef struct {
	void *(*thread_main)(void*);
	void *arg;
} winthreadstart_t;


DWORD WINAPI WinThreadStart(void *arg) {
	winthreadstart_t *wts = (winthreadstart_t*)arg;
	(*wts->thread_main)(wts->arg);
	delete wts;
	return 0;
}

CThread::CThread(void *(*where_to)(void *), void *parameter, int priority)
{
	winthreadstart_t *wts = new winthreadstart_t;
	wts->thread_main = where_to;
	wts->arg = parameter;

	_thread = CreateThread(NULL, 0, WinThreadStart, wts, 0, NULL);
	//    SetThreadPriority(_thread, priority);
}

CThread::~CThread()
{
	CloseHandle(_thread);
}

void CThread::exit(void *status)
{
	ExitThread(*(DWORD*)status);
}

void CThread::join(void **)
{
	WaitForSingleObject(_thread, INFINITE);
}

void CThread::yield()
{
	SwitchToThread();
}

void CThread::sleep()
{
	Sleep(0);
}

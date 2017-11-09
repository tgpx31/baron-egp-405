#include "egpNet\utils\egpThread.h"
#include <Windows.h>

unsigned long __stdcall threadLaunchInternal(void *params)
{
	EGPThread* theThread = (EGPThread*)params;
	theThread->result = theThread->func(theThread->params);
	theThread->flag = 1;	// about to start

	return theThread->result;
}

int threadLaunch(EGPThread *threadOut, EGPThreadFunc func, void *params)
{
	if (threadOut && !threadOut->handle && func)
	{
		threadOut->func = func;
		threadOut->params = params;
		threadOut->flag = -1;	// not started yet
		threadOut->result = -1;

		threadOut->handle = CreateThread(0, 0, threadLaunchInternal, threadOut, 0, &threadOut->id);

		return threadOut->id;
	}
	return -1;
}
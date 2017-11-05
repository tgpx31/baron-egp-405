#include "egpNet\utils\egpThread.h"
#include <Windows.h>

unsigned long __stdcall threadLunchInternal(void *param)
{
	egpThread* theThread = (egpThread*)param;
	theThread->flag = 0;
	theThread->result = theThread->func(theThread->params);
	theThread->flag = 1;

	return theThread->result;
}


int threadLaunch(egpThread* thread_out, egpThreadFunc func, void* params)
{
	if (thread_out && !thread_out->handle && func)
	{
		thread_out->func = func;
		thread_out->params = params;
		thread_out->flag = -1;
		thread_out->result = -1;

		thread_out->handle = CreateThread(0, 0, threadLunchInternal, thread_out, 0, thread_out->id);
		return thread_out->id;
	}

	return -1;
}

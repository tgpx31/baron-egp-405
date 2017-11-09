#ifndef _EGP_THREAD_H
#define _EGP_THREAD_H

#ifdef __cplusplus
extern "C"
{
#else	// !c++
typedef struct EGPThread EGPThread;
#endif	// c++

typedef int(*EGPThreadFunc)(void *);

struct EGPThread
{
	void *handle;
	unsigned long id;

	int flag;
	long result;
	EGPThreadFunc func;
	void *params;
};

int threadLaunch(EGPThread *threadOut, EGPThreadFunc func, void *params);
//unsigned long __stdcall threadLaunchInternal(void *params);

#ifdef __cplusplus
}
#endif	// c++

#endif //_EGP_THREAD_H
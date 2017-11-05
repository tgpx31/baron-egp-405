#ifndef EGP_THREAD_H
#define EGP_THREAD_H

#ifdef __cplusplus
extern "C"
{
#else // !__cplusplus
typedef struct egpThread egpThread;
#endif // __cplusplus

typedef int(__stdcall *egpThreadFunc)(void *);

struct egpThread
{
	void *handle;
	unsigned long id;

	int flag;
	long result;
	egpThreadFunc func;
	void* params;
};

int threadLaunch(egpThread* thread_out, egpThreadFunc func, void* params);


#ifdef __cplusplus
}
#endif // __cplusplus


#endif // MY_THREAD_H


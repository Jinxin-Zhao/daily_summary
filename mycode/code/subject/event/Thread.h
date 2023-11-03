#ifndef THREAD_H_INCLUDED
#define THREAD_H_INCLUDED

#include "Mutex.h"

typedef pthread_t THREAD_HANDLE;


void BindThd2CPU(pthread_t thdid,int ncpuid);


class CThread{
public:
	CThread();

	virtual ~CThread();

	virtual bool Create();

	inline THREAD_HANDLE GetHandle();

	void OSSleep(int nSeconds);

	void SleepMs(int nMilliseconds);

	bool Join();

	bool IsCurrentThread();

	void ExitThread();
private:
	static void * _ThreadEntry(void *pParam);

	virtual InitInstance();

	virtual void ExitInstance();

	virtual void Run() = 0;
private:
	THREAD_HANDLE  m_hThread;
	DWORD          m_IDThread;
};

inline THREAD_HANDLE CThread::GetHandle(){
	return m_hThread;
}

#endif // THREAD_H_INCLUDED

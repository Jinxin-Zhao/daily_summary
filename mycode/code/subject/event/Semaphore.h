#ifndef SEMAPHORE_H_INCLUDED
#define SEMAPHORE_H_INCLUDED

#include <semaphore.h>

class CSemaphore{
public:
	CSemaphore(int nValue = 1);
	~CSemaphore();
	bool Lock(int timeout);
	bool Lock();
	bool UnLock(int *pPrevCount = NULL);
private:
	sem_t m_sid;
};

#endif // EPOLLREACTOR_H_INCLUDED

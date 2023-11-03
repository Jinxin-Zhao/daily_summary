#include "Semaphore.h"

CSemaphore::CSemaphore(int nValue)
{
	sem_init(&m_sid,0,nValue);
}
CSemaphore::~CSemaphore()
{
	sem_destroy(&m_sid);
}
bool CSemaphore::Lock(int timeout){
	timespec m_timeout;
	if(clock_gettime(CLOCK_REALTIME,&m_timeout) == -1){
		return (sem_wait(&m_sid) == 0);
	}
	else{
		m_timeout.tv_sec += timeout;
		return (sem_timedwait(&m_sid,&m_timeout) == 0);
	}
}
bool CSemaphore::Lock(){
	return (sem_wait(&m_sid) == 0);
}
bool CSemaphore::UnLock(int *pPrevCount){
	if(sem_post(&m_sid) == 0){
		if(pPrevCount != NULL){
			sem_getvalue(&m_sid,pPrevCount);
		}
		return true;
	}
	return false;
}
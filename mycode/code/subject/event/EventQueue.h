#ifndef EVENTQUEUE_H_INCLUDED
#define EVENTQUEUE_H_INCLUDED

#include "Semaphore.h"
#include "Mutex.h"
#include "EventHandler.h"

struct CSyncEvent
{
	CEvent event;
	CSemphore sem;
	int nRetval;
	CSyncEvent * pNext;
};

class CEventQueue
{
public:
	CEventQueue(int nQueueSize);
	virtual ~CEventQueue();

	bool AddPostEvent(CEventHandler * pEventHandler,int nEventID,DWORD dwParam,void *pParam);

	bool AddSyncEvent(CSyncEvent &event);

	void ClearEvent(CEventHandler * pEventHandler);

	void RemoveAll();

	bool Empty();

protected:
	CEvent     * m_pPosEvent;
	int 		 m_nEventSize;
	int 		 m_nReadPos;
	int 		 m_nWritePos;
	CSyncEvent * m_pFirstSyncEvent;
	CSyncEvent * m_pLastSyncEvent;
	CMutex       m_lock;
};

#endif
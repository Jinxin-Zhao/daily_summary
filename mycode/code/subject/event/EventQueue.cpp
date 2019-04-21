#inclue "EventQueue.h"

CEventQueue::CEventQueue(int nQueueSize)
{
	m_pPosEvent = new CEvent[nQueueSize];
	memset(m_pPosEvent,0,sizeof(CEvent) * nQueueSize);
	m_nEventSize = nQueueSize;
	m_nReadPos = 0;
	m_nWritePos = 0;

	m_pFirstSyncEvent = NULL;
	m_pLastSyncEvent  = NULL;
}
CEventQueue::~CEventQueue()
{
	delete [] m_pPosEvent;
}

bool CEventQueue::AddPostEvent(CEventHandler * pEventHandler,int nEventID,DWORD dwParam,void *pParam){
	m_lock.Lock();
	if((m_nWritePos + 1) % m_nEventSize == m_nReadPos){
		m_lock.UnLock();
		return false;
	}
	CEvent * pEvent = m_pPostEvent + m_nWritePos;
	pEvent->pEventHandler = pEventHandler;
	pEvent->nEventID = nEventID;
	pEvent->dwParam = dwParam;
	pEvent->pParam = pParam;
	m_nWritePos++;
	if(m_nWritePos >= m_nEventSize){
		m_nWritePos = 0;
	}
	m_lock.UnLock();
	return true;
}
bool CEventQueue::AddSyncEvent(CSyncEvent *pEvent){
	m_lock.Lock();
	if(m_pFirstSyncEvent == NULL){
		m_pFirstSyncEvent = m_pLastSyncEvent = pEvent;
	}
	else{
		m_pLastSyncEvent->pNext = pEvent;
		m_pLastSyncEvent = pEvent;
	}
	m_lock.UnLock();
	return true;
}

bool CEventQueue::PeekEvent(CEvent & event)
{
	m_lock.Lock();

	if(m_pFirstSyncEvent){
		event = m_pFirstSyncEvent->event;
		event.pAdd = m_pFirstSyncEvent;
		m_pFirstSyncEvent = m_pFirstSyncEvent->pNext;

		m_lock.UnLock();
		return true;
	}

	if(m_nReadPos != m_nWritePos)
	{
		event = m_pPosEvent[m_nReadPos];
		event.pAdd = NULL;
		m_nReadPos++;
		if(m_nReadPos >= m_nEventSize){
			m_nReadPos = 0;
		}
		m_lock.UnLock();
		return true;
	}
	m_lock.UnLock();
	return false;
}
void CEventQueue::ClearEvent(CEventHandler * pEventHandler){
	m_lock.Lock();
	CSyncEvent * p = m_pFirstSyncEvent;
	while(p != NULL){
		if(p->event.pEventHandler == pEventHandler){
			//delete p->event.pEventHandler
			p->event.pEventHandler = NULL;
		}
		p = p->pNext;
	}

	int i = m_nReadPos;
	while(i != m_nWritePos){
		if(m_pPosEvent[i].pEventHandler = pEventHandler){
			//delete m_pPosEvent[i].pEventHandler;
			m_pPosEvent[i].pEventHandler = NULL;
		}

		i++;
		if(i >= m_nEventSize){ i = 0;}
	}
	m_lock.UnLock();
}

void CEventQueue::RemoveAll(){
	m_pFirstSyncEvent = NULL;
	m_nReadPos        = 0;
	m_nWritePos       = 0;
}
bool CEventQueue::Empty(){
	return (m_pFirstSyncEvent == NULL) && (m_nReadPos == m_nWritePos);
}
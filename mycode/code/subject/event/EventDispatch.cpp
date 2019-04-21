#include "EventDispatcher.h"
#include "TimerHeap.h"

const int SM_DISPATCHER_STOP = 0x0001;

CEventDispatcher::CEventDispatcher():m_queueEvent(2048){
	SyncTime();
	m_nStopCode = 0;
	m_bShouldRun = false;
	m_pTimerQueue = new CTimerHeap(m_nCurrClock);
}

CEventDispatcher::~CEventDispatcher(){}

void CEventDispatcher::RegisterTimer(CEventHandler* pEventHandler,int nIDEvent,int nElapse){
	m_pTimerQueue->RemoveTimer(pEventHandler,nIDEvent);
	m_pTimerQueue->RegisterTimer(pEventHandler,nIDEvent,nElapse);
}

void CEventDispatcher::RemoveTimer(CEventHandler * pEventHandler,int nIDEvent){
	m_pTimerQueue->RemoveTimer(pEventHandler,nIDEvent);
}

void CEventDispatcher::CheckTimer(){
	m_pTimerQueue->Expire(m_nCurrClock);
}

void CEventDispatcher::Run(){
	m_bShouldRun = true;
	while(m_bShouldRun){
		DispatchIOs();
		SyncTime();
		CheckTimer();
		DispatchEvents();
	}
}

void CEventDispatcher::PostEvent(CEventHandler * pEventHandler,int message,DWORD dwParam,void *pParam){
	return m_queueEvent.AddPostEvent(pEventHandler,message,dwParam,pParam);
}
void CEventDispatcher::SendEvent(CEventHandler *pEventHandler,int nEventID,DWORD dwParam,void *pParam){
	m_lockActive.Lock();
	//the same thread msg
	if(IsCurrentThread() || !m_bShouldRun){
		int nRetVal;
		if(pEventHandler != NULL){
			nRetVal = pEventHandler->HandleEvent(nEventID,dwParam,pParam);
		}else{
			nRetVal = HandleEvent(nEventID,dwParam,pParam);
		}
		m_lockActive.UnLock();
		return nRetVal;
	}
	CSyncEvent syncEvent;
	syncEvent.event.pEventHandler = pEventHandler;
	syncEvent.event.nEventID = nEventID;
	syncEvent.event.dwParam = dwParam;
	syncEvent.event.pParam = pParam;
	syncEvent.pNext = NULL;
	syncEvent.sem.Lock();
	m_queueEvent.AddSyncEvent(&syncEvent);
	m_lockActive.UnLock();

	syncEvent.sem.Lock();
	return syncEvent.nRetVal;

}
void CEventDispatcher::DispatchEvents(){
	CEvent event;
	CSyncEvent * pSyncEvent;
	while(m_queueEvent.PeekEvent(event)){
		int nRetVal;
		if(event.pEventHandler != NULL){
			nRetVal = event.pEventHandler->HandleEvent(event.nEventID,event.dwParam，event.pParam);
		}else{
			nRetVal = HandleEvent(event.nEventID,event.dwParam,event.pParam);
		}
		if(event.pAdd != NULL){
			pSyncEvent = (CSyncEvent *)event.pAdd;
			pSyncEvent->nRetVal = nRetVal;
			pSyncEvent->sem.UnLock();
		}
	}
}
void CEventDispatcher::Stop(DWORD nStopCode){
	m_nStopCode = nStopCode;
	SendEvent(NULL,SM_DISPATCHER_STOP,0,NULL);
}
void CEventDispatcher::HandleEvent(int nEventID,DWORD dwParam,void * pParam){
	if(nEventID == SM_DISPATCHER_STOP && IsCurrentThread()){
		m_lockActive.Lock();
		m_bShouldRun = false;
		m_lockActive.UnLock();
	}
	return 0;
}
#ifndef EVENTDISPATCHER_H_INCLUDED
#define EVENTDISPATCHER_H_INCLUDED

#include "Thread.h"
#include "EventHandler.h"
#include "EventQueue.h"
#include "TimerQueue.h"

const int SM_USER = 0x1000;

class CEventDispatcher : public CThread
{
public:
	CEventDispatcher();
	virtual ~CEventDispatcher();

	void RegisterTimer(CEventHandler *pEventHandler,int nIDEvent,int nElapse);

	void RemoveTimer(CEventHandler * pEventHandler,int nIDEvent);

	bool PostEvent(CEventHandler *pEventHandler,int nEventID,DWORD dwParam,void *pParam);

	int SendEvent(CEventHandler *pEventHandler,int nEventID,DWORD dwParam,void *pParam);

	virtual int HandleEvent(int nEventID,DWORD dwParam,void *pParam);

	inline void ClearEvent(CEventHandler *pEventHandler);

	virtual void Run();

	inline DWORD Time();

	inline DWORD GetMilTime();

	inline DWORD GetClock();

	void Stop(DWORD nStopCode = 0);

	inline DWORD GetStopCode();
protected:
	void CheckTimer();

	inline void SyncTimer();

	void DispatchEvents();
private:
	virtual void DispatchIOs() = 0;
protected:
	bool m_bShouldRun;
	DWORD m_nCurrMilTime;
	DWORD m_nCurrTime;
	DWORD m_nCurrClock;

	CEventQueue   m_queueEvent;
	CTimerQueue * m_pTimerQueue;
	DWORD  		  m_nStopCode;
	CMutex 		  m_lockActive;
};

inline DWORD CEventDispatcher::Time(){
	return m_nCurrTime;
}

inline DWORD CEventDispatcher::GetMilTime(){
	return m_nCurrMilTime;
}

inline DWORD CEventDispatcher::GetClock(){
	return m_nCurrClock;
}

inline void CEventDispatcher::SyncTimer(){
	struct timeval timeout;
	gettimeofday(&timeout,0);
	m_nCurrTime = timeout.tv_sec;
	m_nCurrMilTime = timeout.tv_usec/1000;
	m_nCurrClock = m_nCurrTime*1000 + timeout.tv_usec/1000;
}

inline void CEventDispatcher::ClearEvent(CEventHandler *pEventHandler){
	m_queueEvent.ClearEvent(pEventHandler);
}

inline DWORD CEventDispatcher::GetStopCode(){
	return m_nStopCode;
}

#endif // EVENTDISPATCHER_H_INCLUDED

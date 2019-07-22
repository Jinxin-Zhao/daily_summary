#ifndef TIMERQUEUE_H_INCLUDED
#define TIMERQUEUE_H_INCLUDED

typedef unsigned int DWORD

#include "EventHandler.h"

class CTimerQueue
{
public:
	virtual void RegisterTimer(CEventHandler *pEventHandler,int nIDEvent,int nElapse) = 0;

	virtual void RemoveTimer(CEventHandler * pEventHandler,int nIDEvent) = 0;

	virtual void Expire(DWORD nClock) = 0;

	virtual bool CheckExpire(DWORD nClock) = 0;
protected:
	virtual ~CTimerQueue(){}
};

#endif // EPOLLREACTOR_H_INCLUDED

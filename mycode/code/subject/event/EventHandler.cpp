#include "EventHandler.h"
#include "Reactor.h"

CEventHandler::CEventHandler(CReactor *pReactor){
	m_pReactor = pReactor;
}

virtual CEventHandler::~CEventHandler()
{
	KillTimer(0);
	m_pReactor->RemoveIO(this);
	m_pReactor->ClearEvent(this);
}

void CEventHandler::SetTimer(int nIDEvent,int nElapse){
	m_pReactor->RegisterTimer(this,nIDEvent,nElapse);
}

void CEventHandler::KillTimer(int nIDEvent)
{
	m_pReactor->RemoveTimer(this,nIDEvent);
}

//send asynchronous Event to CEventHandler's object
bool CEventHandler::PostEvent(int message,DWORD dwParam,void * pParam)
{
	return m_pReactor->PostEvent(this,message,dwParam,pParam);
}

//send synchronous Event to CEventHandler's object
int CEventHandler::SendEvent(int nEventID,DWORD dwParam,void *pParam)
{
	return m_pReactor->SendEvent(this,message,dwParam,pParam);
}

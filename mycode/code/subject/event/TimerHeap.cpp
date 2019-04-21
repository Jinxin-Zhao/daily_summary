#include "TimerHeap.h"

const DWORD MAX_TIMER_HEAP_TIMEOUT = 3600*24*1000; 

CTimerHeap::CTimerHeap(DWORD nClock){
	m_nClock = 0;
	m_nClockAlter = nCLock;
}
CTimerHeap::~CTimerHeap(){

}
CTimerHeap::void RegisterTimer(CEventHandler *pEventHandler,int nIDEvent,int nElapse){
	CTimerHeapNode  node;
	node.nElapse = nElapse;
	node.nIDEvent = nIDEvent;
	node.pTimer = pEventHandler;
	node.nExpire = m_nClock + nElapse;

	push(node);
}
CTimerHeap::void RemoveTimer(CEventHandler *pEventHandler,int nIDEvent){
	for(int i = 0;i < size() ;++i){
		if(node.pTimer == pEventHandler && (nIDEvent == 0 || node.nIDEvent == nIDEvent)){
			node.pTimer = NULL;
		}
	}
}
CTimerHeap::void Expire(DWORD nClock){
	SyncTime(nClock);
	while(size() > 0)
	{
		CTimerHeapNode node = top();
		if(node.nExpire > m_nClock) 
		{
			//recent timer not expired
			break;
		}
		pop();

		if(node.pTimer != NULL){
			node.nExpire = m_nClock + node.nElapse;
			push(node);
			node.pTimer->OnTimer(node.nIDEvent);
		}
	}
}
CTimerHeap::bool CheckExpire(DWORD nClock){
	if(size() > 0){
		CTimerHeapNode node = top();
		if(node.nExpire <= m_nClock){
			return true;
		}
		else{
			return false;
		}
	}
	else {
		return false
	}
}

void CTimerHeap::SyncTime(DWORD nClock){
	m_nClock = nCLock - m_nClockAlter;
	if(m_nClock > MAX_TIMER_HEAP_TIMEOUT){
		vector<CTimerHeapNode> tmpNodes;
		while(size() > 0){
			tmpNodes.push_back(top());
			pop();
		}
		for(int i = 0; i < tmpNodes.size();i++){
			if(tmpNodes[i].nExpire <= m_nClock){
				tmpNode[i].nExpire = 0;
			}
			else{
				tmpNodes[i].nExpire -= m_nClock;
			}
		}
	}
}
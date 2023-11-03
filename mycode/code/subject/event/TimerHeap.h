#include "TimerQueue.h"

struct CTimerHeapNode{
    CEventHandle * pTimer;
    int mIDEvent;
    int nElapse;
    DWORD nExpire;
};

struct CCompareTimerHeapNode : public binary_function<CTimerHeapNode,CTimerHeapNode,bool>{
    bool operator()(const CTimerHeapNode & x,const CTimerHeapNode & y) const{
        return y.nExpire < x.nExpire;   //top 1 2 ... n
    }
};

class CTimerHeap : public CTimerQueue : public priority_queue<CTimerHeapNode,vector<CTimerHeapNode>,CCompareTimerHeapNode>{
public:
    CTimerHeap(DWORD nClock);
    virtual ~CTimerHeap();

    virtual void RegisterTimer(CEventHandler * pEventHandler,int nIDEvent,int nElapse);

    virtual void RemoveTimer(CEventHandler * pEventHandler,int nIDEvent);

    virtual void Expire(DWORD nClock);

    virtual bool CheckExpire(DWORD nClock);

private:
    void SyncTime(DWORD nClock);
private:
    DWORD m_nClock;
    DWORD m_nClockAlter;
};
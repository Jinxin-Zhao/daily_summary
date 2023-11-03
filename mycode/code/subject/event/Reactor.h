#ifndef REACTOR_H_INCLUDED
#define REACTOR_H_INCLUDED

#include "EventDispatcher.h"
#include <list>

class CReactor : public CEventDispatcher
{
public:
	CReactor();
	virtual ~CReactor();

	virtual void RegisterIO(CEventHandler *pEventHandler);

	virtual void RemoveIO(CEventHandler *pEventHandler);

	virtual void SetSpecialFlag(bool bFlag);

	virtual bool GetSpecialFlag(void);

protected:

	virtual bool HandleOtherTask();
protected:
	typedef list<CEventHandler *>   CEventHandlerList;
	CEventHandlerList   			m_IOList;
	bool 							m_bIOListHasNULL;
	bool 							m_bSpecialFlag;
};

#endif // REACTOR_H_INCLUDED

#ifndef EVENTHANDLER_H_INCLUDED
#define EVENTHANDLER_H_INCLUDED

class CReactor;

class CEvent{
	CEventHandler * pEventHandler;
	int nEventID;
	DWORD dwParam;
	void  *pParam;
	void  *pAdd;
};

class CEventHandler{
public:
	CEventHandler(CReactor *pReactor);
	virtual ~CEventHandler();
	virtual int HandleInput() {return 0;}
	virtual int HandleOutput() {return 0;}

	virtual void OnTimer(int nIDEvent){}

	//send asynchronous Event to CEventHandler's object
	bool PostEvent(int nEventID,DWORD dwParam,void * pParam);

	//send synchronous Event to CEventHandler's object
	int SendEvent(int nEventID,DWORD dwParam,void *pParam);

};



#endif // EVENTHANDLER_H_INCLUDED

#include "Thread.h"

void * CThread::_ThreadEntry(void *pParam){
	CThread *pThread = (CThread *)pParam;
	if(pThread->InitInstance()){
		pThread->Run();
	}
	pThread->ExitInstance();

	return NULL;
}

CThread::CThread(){
	m_hThread = (THREAD_HANDLE)0;
	m_IDThread = 0;
}

CThread::~CThread(){}

bool CThread::Create(){
	if(m_hThread != (THREAD_HANDLE)0){
		return true;
	}
	bool ret = true;

	ret = (::pthread_create(&m_hThread,NULL,&_ThreadEntry,this) == 0);

	return ret;
}

void CThread::OSSleep(int nSeconds){

}

void CThread::SleepMs(int nMilliseconds){
	::usleep(nMilliseconds);
}
bool CThread::Join(){
	THREAD_HANDLE hThread = GetHandle();
	if(hThread == (THREAD_HANDLE)0){
		return true;
	}
	return (pthread_join(hThread,NULL) == 0);
}
bool CThread::IsCurrentThread(){
	return pThread_self() == m_hThread;
}
void CThread::ExitThread(){

}


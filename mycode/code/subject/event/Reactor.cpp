#include "Reactor.h"

CReactor::CReactor(){
	m_bIOListHasNULL = false;
	m_bSpecialFlag   = false;
}

CReactor::~CReactor(){

}

void CReactor::RegisterIO(CEventHandler *pEventHandler){
	m_IOList.push_back(pEventHandler);
}
void CReactor::RemoveIO(CEventHandler *pEventHandler){
	for(auto i = m_IOList.begin();i != m_IOList.end(); i++){
		if((*i) == pEventHandler){
			(*i) = NULL;
			m_bIOListHasNULL = true;
		}
	}
}
bool CReactor::HandleOtherTask(){
	if(!m_queueEvent.Empty()){
		return true;
	}
	if(m_pTimerQueue->CheckExpire(m_nCurrClock)){
		return true;
	}
	return false;
}

void CReactor::SetSpecialFlag(bool bFlag){
	m_bSpecialFlag = bFlag;
}

bool CReactor::GetSpecialFlag(void){
	return m_bSpecialFlag;
}

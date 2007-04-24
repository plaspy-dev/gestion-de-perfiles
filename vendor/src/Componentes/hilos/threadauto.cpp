# include "stdafx.h"
# include "Componentes.h"
#include "hilos/ThreadAuto.h"


IMPLEMENT_DYNCREATE(CThreadAuto, CWinThread)

CThreadAuto::CThreadAuto(CThreadAuto *old/*=NULL*/)
{
	m_bAutoDelete=false;
	m_bWaitOnDestroy=true;

	m_pThreadOld= old;
	m_bRunning= false;
	m_bRealRunning=false;
	m_pParametro= NULL;
	m_nPrioridad= THREAD_PRIORITY_NORMAL;
	if(m_pThreadOld) {
		if(m_pThreadOld->m_bRunning) {
			m_pThreadOld->Kill();
		}
	}
}

CThreadAuto::~CThreadAuto()
{
	Kill();
	CThreadAuto *info= m_pThreadOld, *aux;
	while( info) {
		aux= info->m_pThreadOld;
		info->m_pThreadOld= NULL;
		ASSERT(info);
		ASSERT(AfxIsValidAddress(info, sizeof(CThreadAuto)));
		delete info;
		info=aux;
	}
	Kill(m_bWaitOnDestroy);
}

void CThreadAuto::WaitOnDestroy(bool wait/*=true*/) {

	m_bWaitOnDestroy=wait;
}

BOOL CThreadAuto::InitInstance()
{
	m_bRunning=true;
	Init();
	m_bRunning=false;
	return FALSE;
}

int CThreadAuto::ExitInstance()
{
	m_bRealRunning=false;
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CThreadAuto, CWinThread)
END_MESSAGE_MAP()

DWORD CThreadAuto::Run(void *parametro/*=NULL*/) {

	FUNCTION_STACK_TRACE("FFFFC026");
	if(m_bRunning || m_bRealRunning)
		return m_nThreadID;
	m_bRealRunning= true;
	m_bRunning= true;
	m_pParametro= parametro;
	CreateThread();
	SetThreadPriority(m_nPrioridad);
	return m_nThreadID;
}

bool CThreadAuto::Kill(bool wait/*=false*/) {

	FUNCTION_STACK_TRACE("FFFFC027");

	if(m_bRunning) {
		m_bRunning= false;
	}
	if(wait && m_bRealRunning) {

		// CWaitCursor wait1;
		while(1) {
			if(!m_bRealRunning) {
				break;
			}
			MSG msg;
			while (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}
	return true;
}

bool CThreadAuto::IsRunning() {

	return m_bRunning;
}

bool CThreadAuto::Init() {
	
	if(m_bRunning && m_pThreadOld) {
		
		CThreadAuto *temp= m_pThreadOld;
		m_pThreadOld= NULL;
		ASSERT(AfxIsValidAddress(temp, sizeof(CThreadAuto)));
		temp->Kill(true);
		delete temp;
		temp=NULL;
	}
	Kill();
	return true;
}

int	CThreadAuto::GetThreadId() {

	return m_nThreadID;
}


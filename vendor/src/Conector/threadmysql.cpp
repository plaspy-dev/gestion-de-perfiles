#include "StdAfx.h"
# include "conector.h"
#include "threadmysql.h"
#include "realconector.h"


CThreadMySQL::CThreadMySQL(CThreadMySQL	*threadOld/*=NULL*/) : CThreadAuto(threadOld)
{
	FUNCTION_STACK_TRACE("FFFFC023");
	m_pParent= NULL;
	m_pConector= NULL;
	m_pArrayDB= NULL;
	m_nReintentos=0;
	m_pMainWnd= NULL;
	m_nType= 0;
}

CThreadMySQL::~CThreadMySQL(void)
{
}

bool CThreadMySQL::Kill(bool wait/*=false*/) {

	return CThreadAuto::Kill(wait);
}

CArrayDB *CThreadMySQL::GetArrayDB() const {

	return m_pArrayDB;
}

int CThreadMySQL::GetState() {

	return m_pArrayDB ? m_pArrayDB->GetEstado() : -1;
}

bool CThreadMySQL::Init() {

	// static DWORD dwTick = GetTickCount();
	// TRACE ("Inicio %d\n", GetTickCount() - dwTick);
	// dwTick = GetTickCount();

	FUNCTION_STACK_TRACE("FFFFC029");
	CArrayDB *info=NULL;
	bool retorno=false;
	if( m_pArrayDB) {
		retorno=true;
		for( int i=0;i<m_pArrayDB->GetCount();i++) {

			info= m_pArrayDB->GetAt(i);
			if( !info) {
				retorno= false;
				break;
			}
			info->SetPadre(m_pParent);
			info->SetRetry(m_nReintentos);
			if( m_nType==1) {
				if(m_pMainWnd==NULL || ::IsWindow(m_pMainWnd->GetSafeHwnd())) {
					if( info->QueryBinary(m_szQuery, m_nID, this)!=-1) {
						if(m_pMainWnd && ::IsWindow(m_pMainWnd->GetSafeHwnd()))
							m_pMainWnd->PostMessage(WM_TREADRESULT,2, i);
						break;
					}
				}
			} else {
				if(m_pMainWnd==NULL || ::IsWindow(m_pMainWnd->GetSafeHwnd())) {
					if(info->Query(m_szQuery, m_nID, this)!=-1) {
						if(m_pMainWnd && ::IsWindow(m_pMainWnd->GetSafeHwnd()))
							m_pMainWnd->PostMessage(WM_TREADRESULT,2, i);
						break;
					}
				}
			}
		}
	}
	// TRACE ("Termino %d\n", GetTickCount()-dwTick);
	// dwTick= GetTickCount();
	// TRACE ("Real Termino %d\n", GetTickCount()-dwTick);
	
	CMySQLMessage mensaje((DWORD)m_nID, m_pParent);
	if( IsRunning() && m_pParent && ::IsWindow(m_pParent->GetSafeHwnd())) 
		m_pParent->SendMessage(WM_MYSQL, THREAD_END, (LPARAM) &mensaje);
	if( IsRunning() && m_pMainWnd && ::IsWindow(m_pMainWnd->GetSafeHwnd()))
		m_pMainWnd->PostMessage(WM_TREADRESULT, 0, 0);	
	bool running= m_bRunning;
	if( IsRunning() && m_pConector)
		m_pConector->RealDeleteThread();
	m_bRunning=running;
	CThreadAuto::Init();
	// dwTick = GetTickCount();
	return retorno;
}

void CThreadMySQL::SetArrayDB( CArrayDB *ArrayDB) {
	
	FUNCTION_STACK_TRACE("FFFFC02A");
	m_pArrayDB= ArrayDB;
}

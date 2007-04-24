///////////////////////////////////////////////////////////////////////////
//                                                                         
// FILE NAME                                                               
//                                                                         
//		XInfoTip.cpp
//                                                                         
// COMPONENT                                                               
//                                                                         
//		CXInfoTip class implementation
//                                                                         
// DESCRIPTION                                                             
//
//		(see header)
//
// AUTHOR
//
//		Mark Bozeman		09-16-2001
//
///////////////////////////////////////////////////////////////////////////
// This software is released into the public domain.  
// You are free to use it in any way you like.
//
// This software is provided "as is" with no expressed 
// or implied warranty.  I accept no liability for any 
// damage or loss of business that this software may cause. 
///////////////////////////////////////////////////////////////////////////

#include "stdafx.h"			// PCH
# include "Componentes.h"
#include "XInfoTip.h"		// Class interface


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define DEFAULT_SHOW_DELAY		50		// Default show delay (ms)
#define TIMER_HIDE				5000	// Hide timer (ms)

/////////////////////////////////////////////////////////////////////
// 
// CXInfoTip::CXInfoTip()
// 
// DESCRIPTION
//     
//		Constructor
//     
/////////////////////////////////////////////////////////////////////
CXInfoTip::CXInfoTip()
{
	// Set the delay defaults
	m_nShowDelay	= DEFAULT_SHOW_DELAY;
	m_pParent=NULL;
	m_bVisible=false;
}

/////////////////////////////////////////////////////////////////////
// 
// CXInfoTip::~CXInfoTip()
// 
// DESCRIPTION
//     
//		Deconstructor
//     
/////////////////////////////////////////////////////////////////////
CXInfoTip::~CXInfoTip()
{
}

// Message map
BEGIN_MESSAGE_MAP(CXInfoTip, CWnd)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////
// 
// CXInfoTip::Create()
// 
// DESCRIPTION
//     
//		Creates the tip window
//
// RETURNS
//
//		[BOOL]			- TRUE on success, FALSE on failure
//
// PARAMETERS
//
//		[pParentWnd]	- Pointer to parent window
//     
/////////////////////////////////////////////////////////////////////
BOOL CXInfoTip::Create(CWnd* pParentWnd, bool globo/*=true*/) 
{
	FUNCTION_STACK_TRACE("FFFFE0D1");
	if(m_hWnd)
		return true;

	DWORD estilo= TTS_NOPREFIX | TTS_ALWAYSTIP;
	if(globo)
		estilo|= TTS_BALLOON;
	BOOL bSuccess = CToolTipCtrl::CreateEx(pParentWnd,  estilo, WS_EX_TOPMOST);

	m_pParent= pParentWnd;

	TOOLINFO m_ToolInfo;
	FillInToolInfo(m_ToolInfo, pParentWnd, 0);
	m_ToolInfo.uFlags = TTF_TRACK | TTF_TRANSPARENT;
	SendMessage(TTM_ADDTOOL, 0, (LPARAM) (LPTOOLINFO) &m_ToolInfo);

	return bSuccess;
}

/////////////////////////////////////////////////////////////////////
// 
// CXInfoTip::Show()
// 
// DESCRIPTION
//     
//		Shows the tip window
//
// RETURNS
//
//		[void]	
//
// PARAMETERS
//
//		[szText]	- Tip text
//		[pt]		- Coordinates to display tip window
//					or NULL to use the current cursor position
//     
/////////////////////////////////////////////////////////////////////
void CXInfoTip::Show(CString szText, CPoint *pt /* = NULL */)
{
	FUNCTION_STACK_TRACE("FFFFE0D2");

	if (pt != NULL)
		m_ptOrigin	= *pt;
	else
		GetCursorPos(&m_ptOrigin);
	if(m_bVisible && m_szText==szText) 
		return;
	m_szText = szText;
	m_nTimer = SetTimer(timerShow, m_nShowDelay, NULL);
}

void CXInfoTip::Show(CString szText,CPoint *pt, CString szTitulo, int icono) {

	SetTitle(icono, szTitulo);
	Show(szText, pt);
}

/////////////////////////////////////////////////////////////////////
// 
// CXInfoTip::OnTimer()
// 
// DESCRIPTION
//     
//		Timer event
//
// RETURNS
//
//		[void]	
//
// PARAMETERS
//
//		[nIDEvent]	- Timer identifier
//     
/////////////////////////////////////////////////////////////////////
void CXInfoTip::OnTimer( UINT nIDEvent ) 
{
	FUNCTION_STACK_TRACE("FFFFE0D6");
	CPoint	ptCursor;

	switch (nIDEvent)
	{
	/////////////////////////////////////////////////////////////////////
	// Show the tip window
	/////////////////////////////////////////////////////////////////////
	case timerShow:
		{
			KillTimer(m_nTimer);

			TOOLINFO m_ToolInfo;
			FillInToolInfo(m_ToolInfo, m_pParent, 0);
			m_ToolInfo.uFlags = TTF_TRACK | TTF_TRANSPARENT;
			m_ToolInfo.lpszText= (LPTSTR)(LPCTSTR)m_szText;

			SendMessage(TTM_UPDATETIPTEXT, 0, (LPARAM) &m_ToolInfo);
			SendMessage(TTM_TRACKPOSITION, 0, (LPARAM) MAKELONG(m_ptOrigin.x, m_ptOrigin.y));
			SendMessage(TTM_TRACKACTIVATE, TRUE, (LPARAM)(LPTOOLINFO) &m_ToolInfo);
			m_bVisible=true;
			m_nTimer = SetTimer(timerHide, TIMER_HIDE, NULL);
		}
		break;
	/////////////////////////////////////////////////////////////////////
	// Hide the tip window
	/////////////////////////////////////////////////////////////////////
	case timerHide:
		GetCursorPos(&ptCursor);
		if (ptCursor != m_ptOrigin)
			Hide();
		break;
	}

	CWnd::OnTimer(nIDEvent);
}

void CXInfoTip::Hide() {

	if(::IsWindow(m_hWnd))
		KillTimer(timerHide);
	if(m_bVisible) {
		TOOLINFO m_ToolInfo;
		FillInToolInfo(m_ToolInfo, m_pParent, 0);
		SendMessage(TTM_TRACKACTIVATE, FALSE, (LPARAM)(LPTOOLINFO) &m_ToolInfo);
	}
	m_bVisible=false;
};
/////////////////////////////////////////////////////////////////////
// 
// CXInfoTip::OnDestroy()
// 
// DESCRIPTION
//     
//		Window destruction
//
// RETURNS
//
//		[void]	
//
/////////////////////////////////////////////////////////////////////
void CXInfoTip::OnDestroy() 
{
	FUNCTION_STACK_TRACE("FFFFE0D6");
	KillTimer(m_nTimer);
	CWnd::OnDestroy();
}

/////////////////////////////////////////////////////////////////////
// 
// CXInfoTip::RelayEvent()
// 
// DESCRIPTION
//     
//		Call this in the parent's PreTranslateMessage() to 
//		relay tooltip event messages.
//
// RETURNS
//
//		[void]	
//
// PARAMETERS
//
//		[lpMsg]	- Pointer to message structure
//     
/////////////////////////////////////////////////////////////////////
void CXInfoTip::RelayEvent(LPMSG lpMsg)
{
	FUNCTION_STACK_TRACE("FFFFE0D7");
	switch(lpMsg->message)
	{
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_KEYDOWN:
	case WM_CHAR:
	case WM_NCLBUTTONDOWN:
	case WM_NCRBUTTONDOWN:
		Hide();
		break;
	case WM_MOUSEMOVE:
		{
			CPoint			point;
			GetCursorPos(&point);
			if (point != m_ptOrigin)
				Hide();
		}
		break;
	}
}

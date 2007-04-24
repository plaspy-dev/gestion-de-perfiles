// MDIChildWndXP.cpp : implementation file
//

#include "stdafx.h"
#include "newmenu/MDIChildWndXP.h"
# include "newmenu/MDIFrameWndXP.h"

// CMDIChildWndXP

IMPLEMENT_DYNCREATE(CMDIChildWndXP, CMDIChildWnd)

CMDIChildWndXP::CMDIChildWndXP()
{
	m_bInMenuLoop = FALSE;
	m_TimerID = NULL;
	m_menubarItemIndex = UINT(-1);
}

CMDIChildWndXP::~CMDIChildWndXP()
{
}


BEGIN_MESSAGE_MAP(CMDIChildWndXP, CMDIChildWnd)
ON_WM_CREATE()
	ON_WM_MEASUREITEM()
    ON_WM_MENUCHAR()
    ON_WM_INITMENUPOPUP()
    ON_WM_ENTERMENULOOP()
    ON_WM_EXITMENULOOP() 
    ON_WM_TIMER()
    ON_WM_NCHITTEST()
    ON_WM_DESTROY()
    ON_WM_SYSCOLORCHANGE()
    ON_WM_NCPAINT()
    ON_WM_PAINT()
    ON_WM_ACTIVATEAPP()
    ON_WM_ACTIVATE()
	ON_WM_CHILDACTIVATE()
	ON_WM_WINDOWPOSCHANGED()
END_MESSAGE_MAP()

int CMDIChildWndXP::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if(m_SystemNewMenu.m_hMenu)
	{
		::DestroyMenu(m_SystemNewMenu.Detach());
	}
	m_SystemNewMenu.Attach(::GetSystemMenu(m_hWnd,FALSE));
	UpdateMenuBarColor(m_DefaultNewMenu);
	return 0;
}

void CMDIChildWndXP::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu) 
{
	CMDIChildWnd::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);
	CNewMenu::OnInitMenuPopup(m_hWnd,pPopupMenu, nIndex, bSysMenu);
}

void CMDIChildWndXP::OnSysColorChange() 
{
	CMDIChildWnd::OnSysColorChange();
	CNewMenu::OnSysColorChange();
	UpdateMenuBarColor(m_DefaultNewMenu);
}

void CMDIChildWndXP::OnEnterMenuLoop(BOOL bIsTrackPopupMenu)
{
	m_bInMenuLoop = TRUE;
	if(m_TimerID!=NULL)
	{
		KillTimer(m_TimerID);
		m_TimerID = NULL;
	}
	if (m_menubarItemIndex!=UINT(-1))
	{
		DrawMenubarItem(this,GetMenu(),m_menubarItemIndex,NULL);
		m_menubarItemIndex=UINT(-1);
	}
	CMDIChildWnd::OnEnterMenuLoop(bIsTrackPopupMenu);
}

void CMDIChildWndXP::OnExitMenuLoop(BOOL bIsTrackPopupMenu)
{
	m_bInMenuLoop = FALSE;
	CMDIChildWnd::OnExitMenuLoop(bIsTrackPopupMenu);
}

void CMDIChildWndXP::OnTimer(UINT_PTR nIDEvent)
{
	CMDIChildWnd::OnTimer(nIDEvent);
	if(m_TimerID==nIDEvent)
	{   
		CPoint pt;
		GetCursorPos(&pt);
		SendMessage(WM_NCHITTEST, 0, MAKELONG(pt.x, pt.y));
	}
}

void CMDIChildWndXP::OnDestroy()
{
	if(m_TimerID!=NULL)
	{
		KillTimer(m_TimerID);
		m_TimerID = NULL;
	}
	CMDIChildWnd::OnDestroy();
}

LRESULT CMDIChildWndXP::OnNcHitTest(CPoint point)
{
	UINT nHitCode = CMDIChildWnd::OnNcHitTest(point);
	// Test Win95/98/me and Win NT 4.0
	if(g_Shell<Win2000 || bRemoteSession)
	{
		UINT nStatus;
		UINT nHotlightStatus;

		if( IsChild(GetFocus()) )
		{ 
		nStatus = 0; 
		nHotlightStatus = ODS_HOTLIGHT;
		}
		else if(g_Shell==Win95 || g_Shell==WinNT4)
		{ // Win95 or winNt 4.0 do not have an inactive menubar
		nStatus = 0; 
		nHotlightStatus = 0;
		}
		else
		{ 
		nStatus = ODS_INACTIVE;
		nHotlightStatus = ODS_HOTLIGHT|ODS_INACTIVE;
		}

		CNewMenu* pNewMenu = DYNAMIC_DOWNCAST(CNewMenu,GetMenu());
		if (!m_bInMenuLoop && nHitCode == HTMENU)
		{
		// I support only CNewMenu ownerdrawings menu!!
		if(pNewMenu)
		{
			UINT nItemIndex = MenuItemFromPoint(m_hWnd, pNewMenu->m_hMenu, point);

			if ( nItemIndex!=(UINT)-1 )
			{
			if(m_menubarItemIndex!=nItemIndex)
			{ 
				// Clear the old Item
				DrawMenubarItem(this,pNewMenu,m_menubarItemIndex,nStatus);

				// Draw the hotlight item.
				if(DrawMenubarItem(this,pNewMenu,nItemIndex,nHotlightStatus))
				{
				// Set a new Timer
				if(m_TimerID==NULL)
				{
					m_TimerID=GetSafeTimerID(m_hWnd,100);
				}
				m_menubarItemIndex = nItemIndex;
				}
				else
				{
				m_menubarItemIndex = UINT(-1);
				}
			}
			else
			{
				// Draw the hotlight item again.
				if(CNewMenu::m_dwLastActiveItem==NULL && 
					DrawMenubarItem(this,pNewMenu,nItemIndex,nHotlightStatus))
				{
				// Set a new Timer
				if(m_TimerID==NULL)
				{
					m_TimerID=GetSafeTimerID(m_hWnd,100);
				}
				m_menubarItemIndex = nItemIndex;
				}
			}
			return nHitCode;
			}
		}
		}

		if (m_menubarItemIndex!=UINT(-1))
		{
		DrawMenubarItem(this,pNewMenu,m_menubarItemIndex,nStatus);
		m_menubarItemIndex=UINT(-1);
		}
		if(m_TimerID!=NULL)
		{
		KillTimer(m_TimerID);
		m_TimerID=NULL;
		}
	}
	return nHitCode;
}

void CMDIChildWndXP::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMIS) 
{
	if(!CNewMenu::OnMeasureItem(GetCurrentMessage()))
	{
		CMDIChildWnd::OnMeasureItem(nIDCtl, lpMIS);
	}
} 

LRESULT CMDIChildWndXP::OnMenuChar(UINT nChar, UINT nFlags, CMenu* pMenu) 
{
	LRESULT lresult;
	if( DYNAMIC_DOWNCAST(CNewMenu,pMenu) )
		lresult=CNewMenu::FindKeyboardShortcut(nChar, nFlags, pMenu);
	else
		lresult=CMDIChildWnd::OnMenuChar(nChar, nFlags, pMenu);

	return lresult;
}

void CMDIChildWndXP::OnNcPaint()
{
	CMDIChildWnd::OnNcPaint();
	DrawSmallBorder();
}

void CMDIChildWndXP::OnPaint()
{
	CMDIChildWnd::OnPaint();
	DrawSmallBorder();
}

#if _MFC_VER < 0x0700 
void CMDIChildWndXP::OnActivateApp(BOOL bActive, HTASK hTask)
{
	CMDIChildWnd::OnActivateApp(bActive, hTask);
	DrawSmallBorder();
}
#else
void CMDIChildWndXP::OnActivateApp(BOOL bActive, DWORD dwThreadID)
{
	CMDIChildWnd::OnActivateApp(bActive, dwThreadID);
	DrawSmallBorder();
}
#endif

void CMDIChildWndXP::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized )
{
	CMDIChildWnd::OnActivate(nState,pWndOther, bMinimized );
	DrawSmallBorder();
}

void CMDIChildWndXP::DrawSmallBorder()
{
// To eliminate small border between menu and client rect
	MENUINFO menuInfo = {0};
	menuInfo.cbSize = sizeof(menuInfo);
	menuInfo.fMask = MIM_BACKGROUND;
	if(::GetMenuInfo(::GetMenu(m_hWnd),&menuInfo) && menuInfo.hbrBack)
	{
		CDC* pDC = GetWindowDC(); 
		if( !pDC || !pDC->m_hDC)
		  return;
		CRect clientRect;
		GetClientRect(clientRect);
		ClientToScreen(clientRect);
		CRect windowRect;
		GetWindowRect(windowRect);
		CRect rect(clientRect.left-windowRect.left,clientRect.top-windowRect.top-1,clientRect.right-windowRect.left,clientRect.top-windowRect.top);

		CBrush *pBrush = CBrush::FromHandle(menuInfo.hbrBack);
		// need for win95/98/me
		VERIFY(pBrush->UnrealizeObject());
		CPoint oldOrg = pDC->SetBrushOrg(0,0);
		pDC->FillRect(rect,pBrush);
		pDC->SetBrushOrg(oldOrg);
		// must be called
		ReleaseDC(pDC);
	}
}

void CMDIChildWndXP::ActivateFrame(int nCmdShow)
{
	nCmdShow = SW_MAXIMIZE;
	CMDIChildWnd::ActivateFrame(nCmdShow);
}

BOOL CMDIChildWndXP::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style&= ~WS_SYSMENU;
	for (HWND wnd = ::GetTopWindow(*AfxGetMainWnd()); wnd; wnd = ::GetNextWindow(wnd, GW_HWNDNEXT))
	{
		char wndClass[32];
		::GetClassName(wnd, wndClass, 32);
		if (strncmp(wndClass, "MDIClient", 32) == 0) {
		
			CRect rect;
			::GetWindowRect(wnd, rect);
			cs.cx= rect.Width()+10;
			cs.cy= rect.Height()+10;
			break;
		}
	}
	return CMDIChildWnd::PreCreateWindow(cs);
}

void CMDIChildWndXP::OnChildActivate()
{	
	CMDIChildWnd::OnChildActivate();
	if( ::IsWindow(*this)) {
		WINDOWPLACEMENT wp;
		if( GetWindowPlacement(&wp)) {
			DWORD estilo=GetStyle();
			if(wp.showCmd==SW_SHOWMAXIMIZED) {
				estilo&= ~WS_SYSMENU;
				SetWindowLong(*this, GWL_STYLE, estilo);
			} else {
				estilo|= WS_SYSMENU;
				SetWindowLong(*this, GWL_STYLE, estilo);
			}
		}
	}
}


void CMDIChildWndXP::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	CMDIChildWnd::OnWindowPosChanged(lpwndpos);
	if( IsWindowVisible()) {
		WINDOWPLACEMENT wp;
		if( GetWindowPlacement(&wp)) {
			DWORD estilo=GetStyle();
			if(wp.showCmd==SW_SHOWMAXIMIZED) {
				estilo&= ~WS_SYSMENU;
				SetWindowLong(*this, GWL_STYLE, estilo);
			} else {
				estilo|= WS_SYSMENU;
				SetWindowLong(*this, GWL_STYLE, estilo);
			}
		}
	}
}

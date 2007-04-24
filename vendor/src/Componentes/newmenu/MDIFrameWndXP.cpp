#include "StdAfx.h"
# include "Componentes.h"
#include "newmenu/mdiframewndxp.h"
# include "newmenu/MultiDocTemplateXP.h"
# include "atlbase.h"

# include "ComponentesDoc.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CMDIFrameWndXP,CMDIFrameWnd)
IMPLEMENT_JDOCK(CMDIFrameWndXP)

const DWORD CFrameWnd::dwDockBarMap[4][2] =
{
  { AFX_IDW_DOCKBAR_TOP,      CBRS_TOP    },
  { AFX_IDW_DOCKBAR_BOTTOM,   CBRS_BOTTOM },
  { AFX_IDW_DOCKBAR_LEFT,     CBRS_LEFT   },
  { AFX_IDW_DOCKBAR_RIGHT,    CBRS_RIGHT  },
};

BEGIN_MESSAGE_MAP(CMDIFrameWndXP, CMDIFrameWnd)
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
	  ON_JDOCK_MESSAGES()
	  ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	  ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	  ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	  ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	  ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	  ON_COMMAND(ID_EDIT_SELECT_ALL, OnEditSelectAll)
	  ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	  ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	  ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	  ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	  ON_UPDATE_COMMAND_UI(ID_EDIT_CLEAR, OnUpdateEditClear)
	  ON_UPDATE_COMMAND_UI(ID_EDIT_SELECT_ALL, OnUpdateEditSelectAll)
	  ON_UPDATE_COMMAND_UI_RANGE(VENTANAS_PREFIJO, VENTANAS_PREFIJO+50, OnUpdateVentanas)
	  ON_COMMAND_RANGE(VENTANAS_PREFIJO, VENTANAS_PREFIJO+50, OnCommandVentanas)
	  ON_MESSAGE(WM_TREENAVEGADOR,OnTreeNavegador)
	  ON_MESSAGE(WM_STATUSBAR, OnStatusBar)
	  ON_MESSAGE(WM_SETSTATUS,OnSetStatus)
	  ON_MESSAGE(WM_TREADRESULT,OnThreadResult)
	  ON_UPDATE_COMMAND_UI_RANGE(ACELERADORES_PREFIJO, ACELERADORES_PREFIJO+50, OnUpdateMenus)
	  ON_COMMAND_RANGE(ACELERADORES_PREFIJO, ACELERADORES_PREFIJO+50, OnCommandMenu)
	  ON_WM_CLOSE()
	  ON_COMMAND(ID_VER_NAVEGADOR, OnVerNavegador)
	  ON_UPDATE_COMMAND_UI(ID_VER_NAVEGADOR, OnUpdateVerNavegador)
	  ON_MESSAGE(WM_QUERYTIME, OnQueryTime)
	  ON_COMMAND(ID_NEXT_PANEEX, OnNextPane)
	  ON_UPDATE_COMMAND_UI(ID_NEXT_PANEEX, OnUpdateNextPane)
	  ON_COMMAND(ID_PREV_PANEEX, OnPrevPane)
	  ON_UPDATE_COMMAND_UI(ID_PREV_PANEEX, OnUpdatePrevPane)
	  ON_COMMAND(ID_WINDOW_CLOSE_ALL, OnCloseAll)
	  ON_COMMAND(ID_HELP, OnHelpIndex)
	  ON_COMMAND(ID_HELP_INDEX, OnHelpIndex)
	  ON_COMMAND(ID_HELP_FINDER, OnHelpIndex)
END_MESSAGE_MAP()

CMDIFrameWndXP::CMDIFrameWndXP() {

	m_bInMenuLoop = FALSE;
	m_hShowMenu= NULL;
    m_TimerID = NULL;
    m_menubarItemIndex = UINT(-1);
	m_ulTotal=0;
	m_ulRealizados=0;
	m_nCurrentHilos=0;
	m_nAnimation=0;
	m_pTLocal= m_pTServidor= CTime::GetCurrentTime();
	m_nInactivo= 0;
	m_bInactivo= false;
	m_bFocus= true;
	m_bConectando= false;
	m_pFadeWnd= NULL;
}

CMDIFrameWndXP::~CMDIFrameWndXP() {
	CMiscelaneosEx *info;
	for( int i=0; i<m_pMenus.GetCount(); i++) {
		if( info= m_pMenus.GetAt(i)) {
			CNewMenu *menu = (CNewMenu *)info->GetData();
			delete menu;
		}
	}
	if(m_pFadeWnd) {
		delete m_pFadeWnd;
		m_pFadeWnd=NULL;
	}	
}

BOOL CMDIFrameWndXP::ShowMenu(BOOL bShow)
{
  // Gets the actual menu
  HMENU hMenu = ::GetMenu(m_hWnd);   
  if(bShow)
  {
    if(m_hShowMenu)
    {
      ::SetMenu(m_hWnd, m_hShowMenu); 
      DrawMenuBar();
      m_hShowMenu = NULL;
      return FALSE;
    }
    return TRUE;
  }
  else
  {
    m_hShowMenu = hMenu;
    if(m_hShowMenu)
    {
      ::SetMenu(m_hWnd, NULL); 
      DrawMenuBar();
      return TRUE;
    }
    return FALSE;
  }
}

#if _MFC_VER < 0x0700 

BOOL CMDIFrameWndXP::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle,
                                CWnd* pParentWnd, CCreateContext* pContext)
{
  // only do this once
  ASSERT_VALID_IDR(nIDResource);
  ASSERT(m_nIDHelp == 0 || m_nIDHelp == nIDResource);

  m_nIDHelp = nIDResource;    // ID for help context (+HID_BASE_RESOURCE)

  CString strFullString;
  if (strFullString.LoadString(nIDResource))
    AfxExtractSubString(m_strTitle, strFullString, 0);    // first sub-string

  VERIFY(AfxDeferRegisterClass(AFX_WNDFRAMEORVIEW_REG));

  // attempt to create the window
  LPCTSTR lpszClass = GetIconWndClass(dwDefaultStyle, nIDResource);
  LPCTSTR lpszTitle = m_strTitle;
  if (!Create(lpszClass, lpszTitle, dwDefaultStyle, rectDefault,
              pParentWnd, MAKEINTRESOURCE(nIDResource), 0L, pContext))
  {
    return FALSE;   // will self destruct on failure normally
  }

  // save the default menu handle
  ASSERT(m_hWnd != NULL);
  m_hMenuDefault = ::GetMenu(m_hWnd);

  // load accelerator resource
  LoadAccelTable(MAKEINTRESOURCE(nIDResource));

  if (pContext == NULL)   // send initial update
    SendMessageToDescendants(WM_INITIALUPDATE, 0, 0, TRUE, TRUE);

  // save menu to use when no active MDI child window is present
  ASSERT(m_hWnd != NULL);
  m_hMenuDefault = ::GetMenu(m_hWnd);
  if (m_hMenuDefault == NULL)
    TRACE0("Warning: CMDIFrameWnd without a default menu.\n");
  return TRUE;
}
#endif

BOOL CMDIFrameWndXP::Create( LPCTSTR lpszClassName,
                              LPCTSTR lpszWindowName,
                              DWORD dwStyle,
                              const RECT& rect,
                              CWnd* pParentWnd,
                              LPCTSTR lpszMenuName,
                              DWORD dwExStyle,
                              CCreateContext* pContext)
{
  if (lpszMenuName != NULL)
  {
    m_DefaultNewMenu.LoadMenu(lpszMenuName);
    // load in a menu that will get destroyed when window gets destroyed
    if (m_DefaultNewMenu.m_hMenu == NULL)
    {
#if _MFC_VER < 0x0700 
      TRACE0("Warning: failed to load menu for CMDIFrameWndXP.\n");
#else
      TRACE(traceAppMsg, 0, "Warning: failed to load menu for CMDIFrameWndXP.\n");
#endif
      PostNcDestroy();            // perhaps delete the C++ object
      return FALSE;
    }
  }

  m_strTitle = lpszWindowName;    // save title for later

  if (!CreateEx(dwExStyle, lpszClassName, lpszWindowName, dwStyle,
    rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
    pParentWnd->GetSafeHwnd(), m_DefaultNewMenu.m_hMenu, (LPVOID)pContext))
  {
#if _MFC_VER < 0x0700 
    TRACE0("Warning: failed to create CMDIFrameWndXP.\n");
#else
    TRACE(traceAppMsg, 0, "Warning: failed to create CMDIFrameWndXP.\n");
#endif

    return FALSE;
  } 

  UpdateMenuBarColor(m_DefaultNewMenu);

  return TRUE;
}
// dock bars will be created in the order specified by dwDockBarMap
// this also controls which gets priority during layout
// this order can be changed by calling EnableDocking repetitively
// with the exact order of priority
void CMDIFrameWndXP::EnableDocking(DWORD dwDockStyle)
{
 ASSERT((dwDockStyle & ~(CBRS_ALIGN_ANY|CBRS_FLOAT_MULTI)) == 0);
  for (int i = 0; i < 4; i++)
  {
    if (dwDockBarMap[i][1] & dwDockStyle & CBRS_ALIGN_ANY)
    {
      CDockBar* pDock = (CDockBar*)GetControlBar(dwDockBarMap[i][0]);
      if (pDock == NULL)
      {
        pDock = new CNewDockBar;
        if (!pDock->Create(this,
          WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_CHILD|WS_VISIBLE |
            dwDockBarMap[i][1], dwDockBarMap[i][0]))
        {
          AfxThrowResourceException();
        }
      }
    }
  }
  CMDIFrameWnd::EnableDocking(dwDockStyle);
}

int CMDIFrameWndXP::OnCreate(LPCREATESTRUCT lpCreateStruct)
  {
    if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
      return -1;

    if(m_SystemNewMenu.m_hMenu)
    {
      ::DestroyMenu(m_SystemNewMenu.Detach());
    }
    m_SystemNewMenu.Attach(::GetSystemMenu(m_hWnd,FALSE));

    return 0;
  }

  void CMDIFrameWndXP::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu) 
  {
    CMDIFrameWnd::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);
    CNewMenu::OnInitMenuPopup(m_hWnd,pPopupMenu, nIndex, bSysMenu);
  }

  void CMDIFrameWndXP::OnSysColorChange() 
  {
    CMDIFrameWnd::OnSysColorChange();
    CNewMenu::OnSysColorChange();
    UpdateMenuBarColor(m_DefaultNewMenu);
  }

  void CMDIFrameWndXP::OnEnterMenuLoop(BOOL bIsTrackPopupMenu)
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
    CMDIFrameWnd::OnEnterMenuLoop(bIsTrackPopupMenu);
  }

  void CMDIFrameWndXP::OnExitMenuLoop(BOOL bIsTrackPopupMenu)
  {
    m_bInMenuLoop = FALSE;
    CMDIFrameWnd::OnExitMenuLoop(bIsTrackPopupMenu);
  }

  void CMDIFrameWndXP::OnTimer(UINT_PTR nIDEvent)
  {
    if(m_TimerID==nIDEvent)
    {
      CPoint pt;
      GetCursorPos(&pt);
      SendMessage(WM_NCHITTEST, 0, MAKELONG(pt.x, pt.y));
	} else if( nIDEvent==TIMER_RESTORE_STATUS_BAR) {

		KillTimer(TIMER_RESTORE_STATUS_BAR);
		m_wndStatusBar.RestorePane(0);
	} //funcion obsoleta con las nuevas definiciones
	/*else if( nIDEvent==	JUPDATER_TIMER) {

		KillTimer( JUPDATER_TIMER);
		CString temp;
		temp.Format( "\"%s\" %d %s",CApplicacion::GetAppPath("jupdater.exe"), AfxGetApp()->GetProfileInt("jupdater", "Actualizacion", 1), CApplicacion::GetAppName());
		WinExec( temp, SW_MINIMIZE);
	} */
	CMDIFrameWnd::OnTimer(nIDEvent);
  }

  void CMDIFrameWndXP::OnDestroy()
  {
    if(m_TimerID!=NULL)
    {
      KillTimer(m_TimerID);
      m_TimerID = NULL;
    }
    CMDIFrameWnd::OnDestroy();
  }

  LRESULT CMDIFrameWndXP::OnNcHitTest(CPoint point)
  {
    UINT nHitCode = CMDIFrameWnd::OnNcHitTest(point);
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

  void CMDIFrameWndXP::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMIS) 
  {
    if(!CNewMenu::OnMeasureItem(GetCurrentMessage()))
    {
      CMDIFrameWnd::OnMeasureItem(nIDCtl, lpMIS);
    }
  } 

  LRESULT CMDIFrameWndXP::OnMenuChar(UINT nChar, UINT nFlags, CMenu* pMenu) 
  {
    LRESULT lresult;
    if( DYNAMIC_DOWNCAST(CNewMenu,pMenu) )
      lresult=CNewMenu::FindKeyboardShortcut(nChar, nFlags, pMenu);
    else
      lresult=CMDIFrameWnd::OnMenuChar(nChar, nFlags, pMenu);
    
    return lresult;
  }

  void CMDIFrameWndXP::OnNcPaint()
  {
    CMDIFrameWnd::OnNcPaint();
    DrawSmallBorder();
  }
  
  void CMDIFrameWndXP::OnPaint()
  {
    CMDIFrameWnd::OnPaint();
    DrawSmallBorder();
  }

#if _MFC_VER < 0x0700 
 	void CMDIFrameWndXP::OnActivateApp(BOOL bActive, HTASK hTask)
  {
    CMDIFrameWnd::OnActivateApp(bActive, hTask);
    DrawSmallBorder();
	m_bFocus= bActive;
	if( bActive) {
		if( m_bInactivo && !m_bConectando) {
			KillTimer(50000);
			SetTimer(50000, 100, NULL);
		}
	}
  }
#else
  void CMDIFrameWndXP::OnActivateApp(BOOL bActive, DWORD dwThreadID)
  {
    CMDIFrameWnd::OnActivateApp(bActive, dwThreadID);
    DrawSmallBorder();
	m_bFocus= bActive;
	if( bActive) {
		if( m_bInactivo && !m_bConectando) {
			KillTimer(50000);
			SetTimer(50000, 100, NULL);
		}
	}
  }
#endif

  void CMDIFrameWndXP::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized )
  {
    CMDIFrameWnd::OnActivate(nState,pWndOther, bMinimized );
    DrawSmallBorder();
  }

  void CMDIFrameWndXP::DrawSmallBorder()
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

 void CMDIFrameWndXP::OnCreateChild(HMENU menu) {

	 CNewMenu *nose= (CNewMenu *)CNewMenu::FromHandle(menu);
	 if( nose) {
		 if( nose->GetSafeHmenu() && nose->GetMenuItemCount()) {
			 CNewMenu *submenu, *add;
			 CString nombre;
			 CMiscelaneosEx *info;

			 for( int i=0; i<m_pMenus.GetCount(); i++) {
				 submenu= (CNewMenu *)nose->GetSubMenu(0);
				 if(submenu) {
					 submenu->GetMenuString(0, nombre, MF_BYPOSITION);
					 info= m_pMenus.GetAt(i);
					 if( nombre!=info->GetNombre()) {
						 add= (CNewMenu *) info->GetData();
						 if( i==0)
							 submenu->ModifyMenu(i, MF_BYPOSITION | MF_POPUP, (UINT)add->m_hMenu, info->GetNombre());
						 else 
							 submenu->InsertMenu(i, MF_BYPOSITION | MF_POPUP, (UINT) add->m_hMenu, info->GetNombre());
					 }
				 }
			 }
		 }
	 }
  }


 void CMDIFrameWndXP::OnEditPaste()
 {
	 GetFocus()->SendMessage(WM_PASTE);
 }

 void CMDIFrameWndXP::OnEditCopy()
 {
	 GetFocus()->SendMessage(WM_COPY);
 }

 void CMDIFrameWndXP::OnEditCut()
 {
	 GetFocus()->SendMessage(WM_CUT);
 }

 void CMDIFrameWndXP::OnEditUndo()
 {
	 GetFocus()->SendMessage(WM_UNDO);
 }

 void CMDIFrameWndXP::OnEditClear()
 {
	 GetFocus()->SendMessage(WM_CLEAR);
 }

 void CMDIFrameWndXP::OnEditSelectAll()
 {
	 CWnd *focus= GetFocus();
	 if( focus) {
		 if( focus->IsKindOf(RUNTIME_CLASS(CEdit))) 
			 ((CEdit *)focus)->SetSel(0, focus->GetWindowTextLength());
		 else  if( focus->IsKindOf(RUNTIME_CLASS(CRichEditCtrl))) 
			 ((CRichEditCtrl*) focus)->SetSel(0, focus->GetWindowTextLength());
		 else focus->SendMessage(WM_COMMAND, ID_EDIT_SELECT_ALL,0);
	 }
 }

 void CMDIFrameWndXP::OnUpdateEditPaste(CCmdUI *pCmdUI)
 {
	 CWnd *focus=GetFocus();
	 bool enable=false;
	 if( focus && focus->IsKindOf(RUNTIME_CLASS(CEdit))) {
		 if(!(focus->GetStyle()&ES_READONLY)) {
			 COleDataObject	obj;
			 if (obj.AttachClipboard() && obj.IsDataAvailable(CF_TEXT))
				 enable=true;
		 }
	 } else enable=true;
	 pCmdUI->Enable( enable);
 }

 void CMDIFrameWndXP::OnUpdateEditUndo(CCmdUI *pCmdUI)
 {
	 bool enable= false;
	 CWnd *focus= GetFocus();
	 if( focus) 
		 if(focus->IsKindOf(RUNTIME_CLASS(CEdit))) 
			 enable= ((CEdit *)focus)->CanUndo();
		 else if( focus->IsKindOf(RUNTIME_CLASS(CListCtrl)))
			 enable= ((CListCtrl*)focus)->GetItemCount()!=0;
		 else enable=true;
		 pCmdUI->Enable(enable);
 }

 void CMDIFrameWndXP::OnUpdateEditCopy(CCmdUI *pCmdUI)
 {
	 CWnd *focus= GetFocus();
	 int x=0, y=0;
	 if( focus) 
		 if(focus->IsKindOf(RUNTIME_CLASS(CEdit)) || focus->IsKindOf(RUNTIME_CLASS(CRichEditCtrl)))
			 ((CEdit *)focus)->GetSel(x,y);
		 else if( focus->IsKindOf(RUNTIME_CLASS(CListCtrl)))
			 y= ((CListCtrl *)focus)->GetItemCount();
		 else y= 1;
		 pCmdUI->Enable(x!=y);
 }

 void CMDIFrameWndXP::OnUpdateEditCut(CCmdUI *pCmdUI)
 {
	 CWnd *focus= GetFocus();
	 int x=0, y=0;
	 if( focus) {
		 if(focus->IsKindOf(RUNTIME_CLASS(CEdit)) || focus->IsKindOf(RUNTIME_CLASS(CRichEditCtrl))) {
			 if(!(focus->GetStyle()&ES_READONLY)) {
				 ((CEdit *)focus)->GetSel(x,y);
			 }
		 } else if( focus->IsKindOf(RUNTIME_CLASS(CListCtrl))) {
			 y=((CListCtrl *)focus)->GetItemCount();
		 } else y=1;
	 }
	 pCmdUI->Enable(x!=y);
 }

 void CMDIFrameWndXP::OnUpdateEditClear(CCmdUI *pCmdUI)
 {
	 bool enable=false;
	 CWnd *focus= GetFocus();
	 if( focus) {
		 if( focus->IsKindOf(RUNTIME_CLASS(CEdit))) {
			 if(!(focus->GetStyle()&ES_READONLY)) {
				 enable= focus->GetWindowTextLength()>0;
			 }
		 } else if( focus->IsKindOf(RUNTIME_CLASS(CListCtrl))) {
			 enable= ((CListCtrl *)focus)->GetItemCount()!=0;
		 } else enable=true;
	 }
	 pCmdUI->Enable(enable);
 }

 void CMDIFrameWndXP::OnUpdateEditSelectAll(CCmdUI *pCmdUI)
 {
	 bool enable=false;
	 CWnd *focus= GetFocus();
	 if( focus) {
		 if(focus->IsKindOf(RUNTIME_CLASS(CEdit)))
			 enable= focus->GetWindowTextLength()>0;
		 else if (focus->IsKindOf(RUNTIME_CLASS(CListCtrl)))
			 enable= ((CListCtrl *)focus)->GetItemCount()!=0;
		 else if( focus->IsKindOf(RUNTIME_CLASS(CRichEditCtrl)))
			 enable= focus->GetWindowTextLength()>0;
		 else enable=true;
	 }
	 pCmdUI->Enable(enable);
 }

 void CMDIFrameWndXP::DockControlBarLeftOf(CToolBar* Bar, CToolBar* LeftOf)
 {
	 CRect rect;
	 DWORD dw;
	 UINT n;

	 RecalcLayout(TRUE);

	 LeftOf->GetWindowRect(&rect);
	 rect.OffsetRect(1,0);
	 dw=LeftOf->GetBarStyle();
	 n = 0;
	 n = (dw&CBRS_ALIGN_TOP) ? AFX_IDW_DOCKBAR_TOP : n;
	 n = (dw&CBRS_ALIGN_BOTTOM && n==0) ? AFX_IDW_DOCKBAR_BOTTOM : n;
	 n = (dw&CBRS_ALIGN_LEFT && n==0) ? AFX_IDW_DOCKBAR_LEFT : n;
	 n = (dw&CBRS_ALIGN_RIGHT && n==0) ? AFX_IDW_DOCKBAR_RIGHT : n;
	 DockControlBar(Bar,n,&rect);
 }

 void CMDIFrameWndXP::LoadState() {

	 CWaitCursor wait;

	 m_LogoMdiClientWnd.Pause();

	 // cargar el estado de la barra de navegacion.
	 /*m_pNavBar.LoadStateBar("Barras\\Barras");

	 if( !AfxGetApp()->GetProfileInt("barras\\Barras", "store",0)) {
		 m_pNavBar.ShowWindow(SW_SHOW);
	 }*/

	 int nfocus;
	 CString szDocumentos=AfxGetApp()->GetProfileString("control", "classname");
	 CStringArray documentos;

	 while( (nfocus=szDocumentos.Find(","))>-1) {

		 documentos.Add(szDocumentos.Mid(0, nfocus));
		 szDocumentos.Delete(0, nfocus+1);
	 }
	 if( !szDocumentos.IsEmpty())
		 documentos.Add(szDocumentos);

	 nfocus= AfxGetApp()->GetProfileInt("control","focus", -1);

	 CDocManager *doc= AfxGetApp()->m_pDocManager;
	 CMultiDocTemplateXP *temp, *focus=NULL;
	 CString temporal;
	 if( !doc)
		 return;

	 for( int i=documentos.GetCount()-1; i>=0; i--) {

		 temporal= documentos.GetAt(i);
		 POSITION pos= doc->GetFirstDocTemplatePosition();
		 while( pos) {

			 temp= (CMultiDocTemplateXP *) doc->GetNextDocTemplate(pos);
			 if( temp && temp->GetViewClassName()==temporal) {

				 PostMessage(WM_TREENAVEGADOR, NM_DBLCLK, (LPARAM) temp);
				 Sleep(10);
				 if( nfocus==i)
					 focus= temp;
				 break;
			 }
		 }
	 }
	 if( focus)
		 PostMessage(WM_TREENAVEGADOR, NM_DBLCLK, (LPARAM) focus);
	 if(m_mdiTabs.GetItemCount()==0)
		 m_LogoMdiClientWnd.Start();
 }

 bool CMDIFrameWndXP::AddAcelerator(ACCEL nueva) {

	 int cAcelerator= CopyAcceleratorTable(m_hAccelTable, NULL, 0)+1;
	 LPACCEL lpaccelnew= (LPACCEL) LocalAlloc( LPTR, cAcelerator* sizeof(ACCEL));
	 cAcelerator--;
	 if(!lpaccelnew)
		 return false;
	 CopyAcceleratorTable(m_hAccelTable, lpaccelnew, cAcelerator);
	 lpaccelnew[cAcelerator].cmd=nueva.cmd;
	 lpaccelnew[cAcelerator].fVirt= nueva.fVirt;
	 lpaccelnew[cAcelerator].key= nueva.key;
	 DestroyAcceleratorTable(m_hAccelTable);
	 cAcelerator++;
	 m_hAccelTable= CreateAcceleratorTable(lpaccelnew, cAcelerator);
	 ASSERT(CopyAcceleratorTable(m_hAccelTable, NULL, 0)==cAcelerator);
	 return true;
 }

 CString CMDIFrameWndXP::ParseAcelerator(CString label, int id) {

	 ACCEL nueva;
	 nueva.fVirt= FCONTROL | FNOINVERT | FVIRTKEY;
	 nueva.cmd= ACELERADORES_PREFIJO+id;
	 nueva.key= '1'+id;
	 CString temp;

	 if( nueva.key<='9') {
		 AddAcelerator(nueva);
		 temp.Format("%s\tCtrl+%c", label, nueva.key);
	 } else if( nueva.key=='9'+1) {
		 nueva.key='0';
		 AddAcelerator(nueva);
		 temp.Format("%s\tCtrl+%c", label, nueva.key);
	 } else if( nueva.key<='9'+10) {
		 nueva.key-=10;
		 nueva.fVirt= FALT | FNOINVERT | FVIRTKEY;
		 AddAcelerator(nueva);
		 temp.Format("%s\tAlt+%c", label, nueva.key);
	 } else if( nueva.key=='9'+11) {
		 nueva.key='0';
		 nueva.fVirt= FALT | FNOINVERT | FVIRTKEY;
		 AddAcelerator(nueva);
		 temp.Format("%s\tAlt+%c", label, nueva.key);
	 } else if( nueva.key<='9'+20) {
		 nueva.key-=20;
		 nueva.fVirt= FALT | FNOINVERT | FVIRTKEY | FCONTROL;
		 AddAcelerator(nueva);
		 temp.Format("%s\tCtrl+Alt+%c", label, nueva.key);
	 } else if( nueva.key=='9'+21) {
		 nueva.key='0';
		 nueva.fVirt= FALT | FNOINVERT | FVIRTKEY | FCONTROL;
		 AddAcelerator(nueva);
		 temp.Format("%s\tCtrl+Alt+%c", label, nueva.key);
	 } else temp= label;
	 return temp;
 }

 bool CMDIFrameWndXP::LoadDocument( CNewMenu *menu, HTREEITEM hRoot, CString nombre, UINT nIDResource,CRuntimeClass* pViewClass, const CMiscelaneosEx &permisos, DWORD grupo, CRuntimeClass *pDocClass/*=NULL*/) {

	 if(grupo==0)
		 return LoadDocument(menu, hRoot, nombre, nIDResource, pViewClass, pDocClass);
	 CMiscelaneosEx *info= permisos.Find(pViewClass->m_lpszClassName);
	 if(info && info->GetData())
		return LoadDocument(menu, hRoot, nombre, nIDResource, pViewClass, pDocClass);
	 return false;
 }

 bool CMDIFrameWndXP::LoadDocument( CNewMenu *menu, HTREEITEM hRoot, CString nombre, UINT nIDResource,CRuntimeClass* pViewClass, CRuntimeClass *pDocClass/*=NULL*/) {

	 HTREEITEM item= ::IsWindow(m_pNavBar) ? m_pNavBar.m_Tree.InsertItem( nombre, hRoot) : NULL;

	 CString temp;

	 CMultiDocTemplateXP* pDocTemplate;
	 pDocTemplate = new CMultiDocTemplateXP(nIDResource,pDocClass ? pDocClass : RUNTIME_CLASS(CComponentesDoc),RUNTIME_CLASS(CMDIChildWndXP), pViewClass);
	 AfxGetApp()->AddDocTemplate( pDocTemplate);

	 if(item) {
		temp= pViewClass->m_lpszClassName;
		m_pNavBar.m_Tree.SetItemData( item, (DWORD) pDocTemplate);
	 }
	 if(menu) {
		 m_pAceleradores.Add(temp, ACELERADORES_PREFIJO+m_pAceleradores.GetCount());
		 temp= ParseAcelerator(nombre, m_pAceleradores.GetCount()-1);
		 menu->AppendMenu( MF_STRING, ACELERADORES_PREFIJO+m_pAceleradores.GetCount()-1, temp);
	 }
	 return true;
 }

 void CMDIFrameWndXP::SaveState() {

	 if(AfxGetApp()->GetProfileInt("control", "notstore", 0)==0) {
		 CString temp;
		 TCITEM item;
		 item.mask = TCIF_PARAM;
		 HWND hWnd;

		 for( int i=0; i<m_mdiTabs.GetItemCount(); i++) {

			 m_mdiTabs.GetItem(i, &item);
			 hWnd = HWND(item.lParam);
			 for( CWnd *child=m_LogoMdiClientWnd.GetTopWindow(); child; child =child->GetNextWindow()) {

				 if( child && hWnd ==(*child) && child->IsKindOf(RUNTIME_CLASS(CMDIChildWnd))) {
					 if( !temp.IsEmpty())
						 temp+=",";
					 temp+=((CMDIChildWnd *) child)->GetActiveView()->GetRuntimeClass()->m_lpszClassName;
					 break;
				 }
			 }
		 }
		 AfxGetApp()->WriteProfileString("control","classname",temp);
		 AfxGetApp()->WriteProfileInt("control","focus", m_mdiTabs.GetCurFocus());
	 } else {
		 AfxGetApp()->WriteProfileString("control","classname","");
		 AfxGetApp()->WriteProfileInt("control","focus", -1);
	 }
	 AfxGetApp()->WriteProfileInt("barras\\Barras", "store",1);
	 SaveBarState("barras\\Barras");
	 if(::IsWindow(m_pNavBar))
		m_pNavBar.SaveBar("Barras\\Barras");
 }

 BOOL CMDIFrameWndXP::PreCreateWindow(CREATESTRUCT& cs)
 {
	 
	 if( AfxGetApp()->GetProfileInt("control", "loadstate", 0)) {
		 CRegKey key;
		 if(	key.Open(HKEY_CURRENT_USER, CApplicacion::GetAppKey() +  "\\control")==ERROR_SUCCESS) {

			 key.DeleteValue("posicion");
			 key.DeleteValue("status");
			 key.DeleteValue("focus");
			 key.DeleteValue("classname");
			 key.Close();
		 }
		 if( key.Open(HKEY_CURRENT_USER, CApplicacion::GetAppKey())==ERROR_SUCCESS) {
			 if(key.RecurseDeleteKey("barras")!=ERROR_SUCCESS)
				 key.DeleteSubKey("barras");
			 if(key.RecurseDeleteKey("ingreso")!=ERROR_SUCCESS)
				 key.DeleteSubKey("ingreso");
			 if(key.RecurseDeleteKey("listas")!=ERROR_SUCCESS)
				 key.DeleteSubKey("listas");
			 if(key.RecurseDeleteKey("consulta")!=ERROR_SUCCESS)
				 key.DeleteSubKey("consulta");
			 key.Close();
		 }
	 }
	 int show= AfxGetApp()->GetProfileInt("control", "status", -1);
	 if( show!=-1) { // ya se habia escrito

		 if( show!= SW_SHOWMINIMIZED)
			 AfxGetApp()->m_nCmdShow= show;
		 RECT *myrect;
		 CRect rect(0,0,0,0);
		 show = 0;
		 if( AfxGetApp()->GetProfileBinary("control", "posicion", (LPBYTE*)&myrect, (UINT *)&show)) {
			 rect= *myrect;
			 delete [] myrect;
			 if( rect.Width()>0 && rect.Height()>0) { // restauramos la posicion de la ventana

				 if( rect.Height()<400)
					 rect.bottom= rect.top+400;
				 if( rect.Width()<400)
					 rect.right= rect.left+400;	
				 if( rect.left<0)
					 rect.MoveToX(0);
				 if( rect.top<0)
					 rect.MoveToY(0);

				 int x= GetSystemMetrics(SM_CXSCREEN);
				 int y= GetSystemMetrics(SM_CYSCREEN);

				 if( rect.left<x && rect.top<y) {

					 if( rect.Height()<y && rect.Width()<x) {

						 if( rect.right>x)
							 rect.MoveToX(x-rect.Width());
						 if( rect.bottom>y)
							 rect.MoveToY(y-rect.Height());

						 cs.x = rect.left;
						 cs.y = rect.top;
						 cs.cx= rect.Width();
						 cs.cy= rect.Height();
					 }
				 }
			 }
		 }
	 }
	 return CMDIFrameWnd::PreCreateWindow(cs);
 }

 void CMDIFrameWndXP::OnUpdateVentanas(CCmdUI *pCmdUI) {

	 pCmdUI->Enable((pCmdUI->m_nID-VENTANAS_PREFIJO)<m_mdiTabs.GetItemCount());
 }

 void CMDIFrameWndXP::OnCommandVentanas(UINT id) {
	 if((id-VENTANAS_PREFIJO)<m_mdiTabs.GetItemCount())
		 m_mdiTabs.SetSelEx(id-VENTANAS_PREFIJO);
 }

 void CMDIFrameWndXP::OnUpdateFrameTitle(BOOL bAddToTitle)
 {
	 FUNCTION_STACK_TRACE("AAAAAAAC");
	 CMDIFrameWnd::OnUpdateFrameTitle(bAddToTitle);
	 m_mdiTabs.Update();
	 int sel= m_mdiTabs.GetCurSel();
	 if( sel!=-1) {

		 TCITEM item;
		 item.mask = TCIF_PARAM;
		 m_mdiTabs.GetItem(sel, &item);
		 HWND hWnd = HWND(item.lParam);

		 for (CWnd *child=m_LogoMdiClientWnd.GetTopWindow(); child; child =child->GetNextWindow()) 

			 if( child && hWnd== (*child) && child->IsKindOf(RUNTIME_CLASS(CMDIChildWnd))) {

				 CView *view= ((CMDIChildWnd *) child)->GetActiveView();
				 if( view && ::IsWindow(m_pNavBar)) m_pNavBar.SetSel(view->GetRuntimeClass()->m_lpszClassName);
				 break;
			 }
	 }
	 if(m_mdiTabs.GetItemCount())
		 m_LogoMdiClientWnd.Pause();
	 else m_LogoMdiClientWnd.Start();
 }

 BOOL CMDIFrameWndXP::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
 {
	 if (CMDIFrameWnd::OnCreateClient(lpcs, pContext)){  
		 m_LogoMdiClientWnd.SubclassWindow(m_hWndMDIClient); 
		 return TRUE ;
	 } 
	 return FALSE ;
 }

 LRESULT CMDIFrameWndXP::OnTreeNavegador(WPARAM wParam, LPARAM lParam) {

	 if( wParam== NM_DBLCLK) {

		 if( lParam) {
			 CWaitCursor wait;
			 CMultiDocTemplateXP* temp= (CMultiDocTemplateXP *)lParam;
			 ASSERT(temp);
			 CWnd *child=m_LogoMdiClientWnd.GetTopWindow();
			 CWnd *view;

			 for (; child; child =child->GetNextWindow()) {

				 if(child && child->IsKindOf(RUNTIME_CLASS(CMDIChildWnd))) {
					 for(view=((CMDIChildWnd *) child)->GetWindow(GW_CHILD); view; view= view->GetNextWindow()) {

						 if( view && view->GetRuntimeClass()->m_lpszClassName==temp->GetViewClassName()) {
							 child->BringWindowToTop();
							 child->SetFocus();
							 return 0;
						 }
					 }
				 }
			 }
			 m_LogoMdiClientWnd.Pause();
			 temp->OpenDocumentFile(NULL);
		 }
	 } else if( wParam==100 && ::IsWindow(m_pNavBar)) {
		 HTREEITEM item=m_pNavBar.FindItemEx((const char *)lParam);
		 if( item)
			 SendMessage(WM_TREENAVEGADOR, NM_DBLCLK, m_pNavBar.m_Tree.GetItemData(item));
	 }
	 return 0;
 }

 LRESULT CMDIFrameWndXP::OnStatusBar(WPARAM wParam, LPARAM lParam) {

	 if( wParam==1 || wParam==4) { // agregamos un nuevo elemento
		 m_ulRealizados= wParam==1 ? m_ulRealizados+1 : lParam;
		 if( m_ulTotal) {
			 int pos= (int) (m_ulRealizados*100)/m_ulTotal;
			 m_wndStatusBar.SetPos(1,pos);
			 char real[MAX_PATH],real1[MAX_PATH];
			 CString atributo;
			 _i64toa(m_ulRealizados, real, 10);
			 _i64toa(m_ulTotal, real1, 10);
			 atributo.Format( "%s de %s", real, real1);
			 m_wndStatusBar.GetProgressCtrl(1)->SetWindowText(atributo);
		 }
	 } else if( wParam==0) {// iniciamos la busqueda donde lparam es el total

		 m_ulTotal= lParam;
		 m_wndStatusBar.GetProgressCtrl(1)->SetShowText(AfxGetApp()->GetProfileInt("toolbar", "showtext",false));
		 m_wndStatusBar.GetProgressCtrl(1)->SetPos(0);

	 } else if( wParam==2) { // terminamos la busqueda
		 m_wndStatusBar.GetProgressCtrl(1)->SetShowText(true);
		 m_wndStatusBar.GetProgressCtrl(1)->SetPos(0);
		 if( m_ulTotal) {
			 char real[MAX_PATH];
			 CString atributo;
			 _i64toa(m_ulTotal, real, 10);
			 atributo.Format( "%s Resultado%s", real, m_ulTotal>1 ? "s" : "");
			 m_wndStatusBar.GetProgressCtrl(1)->SetWindowText(atributo);
		 }
		 m_ulTotal=0;
		 m_ulRealizados=0;
	 }
	 return 0;
 }

 bool CMDIFrameWndXP::LoadDocumentos(const CMiscelaneosEx &permisos, DWORD grupo) {

	 FUNCTION_STACK_TRACE("AAAAAAAB");

	 CMyRegKey key(HKEY_LOCAL_MACHINE, CApplicacion::GetAppKey() + "\\Addins");
	 CStringArray llaves;
	 key.EnumRegKey("", llaves);
	 for( int i=0; i<llaves.GetCount(); i++) {

		 CString path= key.GetString(llaves[i], "FullPath");
		 if(!path.IsEmpty()) {
			 LoadDocumentosEx(path, NULL, permisos, grupo);
		 }
	 }

	 // modificamos el menu del mainframe
	 OnCreateChild(m_DefaultNewMenu.m_hMenu);

	 CDocManager *doc= AfxGetApp()->m_pDocManager;
	 if( doc) {
		 POSITION pos= doc->GetFirstDocTemplatePosition();
		 CMultiDocTemplate *temp;
		 while( pos) {
			 temp= (CMultiDocTemplate *)doc->GetNextDocTemplate(pos);
			 if( temp)
				 OnCreateChild(temp->m_hMenuShared);
		 }
	 }
	 return true;
 }

 bool CMDIFrameWndXP::LoadDocumentos(DWORD parametro) {

	 FUNCTION_STACK_TRACE("AAAAAAAB");

	 CMyRegKey key(HKEY_LOCAL_MACHINE, CApplicacion::GetAppKey() + "\\Addins");
	 CStringArray llaves;
	 key.EnumRegKey("", llaves);
	 for( int i=0; i<llaves.GetCount(); i++) {
		 
		 CString path= key.GetString(llaves[i], "FullPath");
		 if(!path.IsEmpty()) {
			LoadDocumentosEx(path, NULL, parametro);
		 }
	 }

	 // modificamos el menu del mainframe
	 OnCreateChild(m_DefaultNewMenu.m_hMenu);

	 CDocManager *doc= AfxGetApp()->m_pDocManager;
	 if( doc) {
		 POSITION pos= doc->GetFirstDocTemplatePosition();
		 CMultiDocTemplate *temp;
		 while( pos) {
			 temp= (CMultiDocTemplate *)doc->GetNextDocTemplate(pos);
			 if( temp)
				 OnCreateChild(temp->m_hMenuShared);
		 }
	 }
	 return true;
 }

 LRESULT CMDIFrameWndXP::OnSetStatus(WPARAM wParam, LPARAM lParam) {

	 ASSERT_VALID(this);
	 CString temp= (const char *) lParam;
	 m_wndStatusBar.RestorePane(0);
	 m_wndStatusBar.SetWindowText(temp);
	 if( wParam>1) {

		 m_wndStatusBar.SavePane(0);
		 m_wndStatusBar.SetBkColor(0, wParam==2 ? GetSysColor(COLOR_ACTIVECAPTION) : RGB(255,0,0));
		 m_wndStatusBar.SetFgColor(0, GetSysColor(COLOR_CAPTIONTEXT));
		 MessageBeep(MB_OK);
		 SetTimer( TIMER_RESTORE_STATUS_BAR, MESSAGE_DELAY, NULL);
	 }
	 return 0;
 } 

 LRESULT CMDIFrameWndXP::OnThreadResult(WPARAM wParam, LPARAM lParam) {

	 switch( wParam) {
		case 0:
			if( m_nAnimation) {
				m_nCurrentHilos--;
			} else m_wndStatusBar.Decrement(4);
			PostMessage( WM_STATUSBAR, 2);
			break;
		case 1:
			if( m_nAnimation) {
				m_nCurrentHilos++;
			} else m_wndStatusBar.Increment(4);
			break;
		default:
			{
				CString temp;
				temp.Format("conector\\servidor%d", (UINT)lParam);
				m_wndStatusBar.SetText(3, AfxGetApp()->GetProfileString(temp, "servidor"));
			}
			break;
	 }
	 return 0;
 } 

 void CMDIFrameWndXP::OnUpdateMenus(CCmdUI *pCmdUI) {

	 pCmdUI->Enable(m_pAceleradores.Find(pCmdUI->m_nID)!=NULL);
 }

 void CMDIFrameWndXP::OnCommandMenu(UINT id) {

	 CMiscelaneosEx *info=m_pAceleradores.Find(id);
	 if( info && ::IsWindow(m_pNavBar))
		 PostMessage(WM_TREENAVEGADOR, NM_DBLCLK, m_pNavBar.m_Tree.GetItemData(m_pNavBar.FindItemEx(info->GetNombre())));
 }

 void CMDIFrameWndXP::SendMessageBroadCast(UINT message,WPARAM wParam, LPARAM lParam) {

	 for ( CWnd *child=m_LogoMdiClientWnd.GetTopWindow(); child; child =child->GetNextWindow()) {
		 if(child && child->IsKindOf(RUNTIME_CLASS(CMDIChildWnd))) {
			 CView *vista=((CMDIChildWnd *) child)->GetActiveView();
			 if( vista && ::IsWindow(*vista))
				 vista->SendMessage(message, wParam, lParam);
		 }
	 }
 }

 void CMDIFrameWndXP::OnClose()
 {
	 if( AfxGetApp()->GetProfileInt("control", "notstore", 0)) {
		 AfxGetApp()->WriteProfileInt("control","status",SW_SHOWMINIMIZED);
		 AfxGetApp()->WriteProfileBinary("control", "posicion", 0, 1);
	 } else {
		 WINDOWPLACEMENT wp;
		 GetWindowPlacement(&wp);
		 AfxGetApp()->WriteProfileInt("control","status", wp.showCmd);
		 AfxGetApp()->WriteProfileBinary("control", "posicion", (LPBYTE)&wp.rcNormalPosition, (UINT) sizeof( wp.rcNormalPosition));
	 }
	 SaveState();
	 AfxGetApp()->WriteProfileInt("control","notstore",0);

	 CMDIFrameWnd::OnClose();
 }

 void CMDIFrameWndXP::OnVerNavegador()
 {
	 if(::IsWindow(m_pNavBar)) {
		if( !m_pNavBar.IsWindowVisible()) 

		 if(m_pNavBar.IsHide()) {

			 m_pNavBar.InitShow();
			 return;
		 }
		 m_pNavBar.ShowWindow((m_pNavBar.IsWindowVisible()) == 0);
		 RecalcLayout();
	 }
 }

 void CMDIFrameWndXP::OnUpdateVerNavegador(CCmdUI *pCmdUI)
 {
	 pCmdUI->SetCheck( ::IsWindow(m_pNavBar) && m_pNavBar.IsWindowVisible());
 }

 LRESULT CMDIFrameWndXP::OnQueryTime(WPARAM wParam, LPARAM lParam) {

	 if( lParam) 
		 ((CWnd *) lParam)->SendMessage(WM_QUERYTIME, 0, (LPARAM)&m_pTServidor);
	 return (LRESULT) &m_pTServidor;
 }

 void CMDIFrameWndXP::OnNextPane()
 {
	 m_mdiTabs.SetSelEx(m_mdiTabs.GetCurSel()+1);
 }

 void CMDIFrameWndXP::OnUpdateNextPane(CCmdUI *pCmdUI)
 {
	 pCmdUI->Enable((m_mdiTabs.GetCurSel()<(m_mdiTabs.GetItemCount()-1)));
 }

 void CMDIFrameWndXP::OnPrevPane()
 {
	 m_mdiTabs.SetSelEx(m_mdiTabs.GetCurSel()-1);
 }

 void CMDIFrameWndXP::OnUpdatePrevPane(CCmdUI *pCmdUI)
 {
	 pCmdUI->Enable(m_mdiTabs.GetCurSel()>0);
 }

 void CMDIFrameWndXP::OnCloseAll() {

	 int pos= m_mdiTabs.GetCurSel();
	 if( pos>=0) {
		 CWaitCursor wait;
		 DWORD data= m_mdiTabs.GetItemData(pos);
		 DWORD del;
		 ASSERT(data);
		 pos=0;
		 while( m_mdiTabs.GetItemCount()>1) {
			 del= m_mdiTabs.GetItemData(pos);
			 if( del==data)
				 pos++;
			 else ::SendMessage(HWND(del), WM_CLOSE, 0,0);
		 }
	 }
 }

 int CMDIFrameWndXP::LoadDocumentosEx(CString nombreDLL, HTREEITEM hRoot, const CMiscelaneosEx &permisos, DWORD grupo) {

	 // cargamos mas documentos desde dll
	 HINSTANCE hDLL;
	 int i=0;
	 LOADFUNCIONES lpfnInicial;
	 CMiscelaneosEx *info,*acel;
	 CNewMenu *menu;
	 CString temp;

	 // cargamos todos los nombres de las funciones
	 hDLL = LoadLibrary( nombreDLL);
	 lpfnInicial= (LOADFUNCIONES)::GetProcAddress( hDLL, "LoadFunciones3");
	 if( !lpfnInicial) {

		 FreeLibrary(hDLL);
		 return false;
	 }
	 else {

		 LPDLLFUNC3 lpfnDllFunc;
		 CStringArray funciones;
		 CMultiDocTemplateXP *pDocTemplate;

		 lpfnInicial(&funciones);
		 for( i=0; i<funciones.GetCount(); i++) {

			 lpfnDllFunc = (LPDLLFUNC3)::GetProcAddress(hDLL,funciones.GetAt(i));
			 if (lpfnDllFunc) {
				 CString strLabel;
				 CString strGrupo;
				 pDocTemplate = lpfnDllFunc(&strLabel, permisos, grupo, &strGrupo);
				 if( pDocTemplate) {

					 if( strLabel.IsEmpty())
						 continue;
					 if(strGrupo.IsEmpty())
						 strGrupo= ::IsWindow(m_pNavBar) ? m_pNavBar.m_Tree.GetItemText(m_pNavBar.m_Tree.GetRootItem()) : "";
					 HTREEITEM item= ::IsWindow(m_pNavBar) ? m_pNavBar.FindItem( strGrupo) : NULL;
					 info= m_pMenus.FindNoCase(strGrupo);
					 if( item==NULL && ::IsWindow(m_pNavBar)) {
						 item= m_pNavBar.m_Tree.InsertItem(strGrupo);
						 m_pNavBar.m_Tree.SetItemState(item, TVIS_BOLD,TVIS_BOLD);
						}
					 if( item && ::IsWindow(m_pNavBar))
						 item= m_pNavBar.m_Tree.InsertItem( strLabel,item);
					 if( item && ::IsWindow(m_pNavBar))
						 m_pNavBar.m_Tree.SetItemData( item, (DWORD) pDocTemplate);

					 temp= ParseAcelerator(strLabel, m_pAceleradores.GetCount());

					 acel= m_pAceleradores.Add(pDocTemplate->GetViewClassName(), m_pAceleradores.GetCount()+ACELERADORES_PREFIJO);

					 if( info) {
						 menu= (CNewMenu *) info->GetData();
						 menu->AppendMenu(MF_STRING, acel->GetData(), temp);
					 } else {
						 menu= new CNewMenu();
						 menu->CreateMenu();
						 menu->AppendMenu(MF_STRING, acel->GetData(), temp);
						 m_pMenus.Add(strGrupo, (DWORD) menu);
						}
					}
				}
			}
	 }
	 if(::IsWindow(m_pNavBar))
		 m_pNavBar.m_Tree.Expand( hRoot, TVE_EXPAND);
	 return i;	 
 }

 int CMDIFrameWndXP::LoadDocumentosEx(CString nombreDLL, HTREEITEM hRoot, DWORD parametro) {

	 // cargamos mas documentos desde dll
	 HINSTANCE hDLL;
	 int i=0;
	 LOADFUNCIONES lpfnInicial;
	 CMiscelaneosEx *info,*acel;
	 CNewMenu *menu;
	 CString temp;

	 // cargamos todos los nombres de las funciones
	 hDLL = LoadLibrary( nombreDLL);
	 lpfnInicial= (LOADFUNCIONES)::GetProcAddress( hDLL, "LoadFunciones");
	 if( !lpfnInicial) {

		 FreeLibrary(hDLL);
		 return false;
	 }
	 else {

		 LPDLLFUNC lpfnDllFunc;
		 CStringArray funciones;
		 CMultiDocTemplateXP *pDocTemplate;

		 lpfnInicial(&funciones);
		 for( i=0; i<funciones.GetCount(); i++) {

			 lpfnDllFunc = (LPDLLFUNC)::GetProcAddress(hDLL,funciones.GetAt(i));
			 if (lpfnDllFunc) {
				 CString strLabel;
				 CString grupo;
				 pDocTemplate = lpfnDllFunc(&strLabel, parametro, &grupo);
				 if( pDocTemplate) {

					 if( strLabel.IsEmpty())
						 continue;
					 if(grupo.IsEmpty())
						 grupo= ::IsWindow(m_pNavBar) ? m_pNavBar.m_Tree.GetItemText(m_pNavBar.m_Tree.GetRootItem()) : "";
					 HTREEITEM item= ::IsWindow(m_pNavBar) ? m_pNavBar.FindItem( grupo) : NULL;
					 info= m_pMenus.FindNoCase(grupo);
					 if( item==NULL && ::IsWindow(m_pNavBar)) {
						 item= m_pNavBar.m_Tree.InsertItem(grupo);
						 m_pNavBar.m_Tree.SetItemState(item, TVIS_BOLD,TVIS_BOLD);
						}
					 if( item && ::IsWindow(m_pNavBar))
						 item= m_pNavBar.m_Tree.InsertItem( strLabel,item);
					 if( item && ::IsWindow(m_pNavBar))
						 m_pNavBar.m_Tree.SetItemData( item, (DWORD) pDocTemplate);

					 temp= ParseAcelerator(strLabel, m_pAceleradores.GetCount());

					 acel= m_pAceleradores.Add(pDocTemplate->GetViewClassName(), m_pAceleradores.GetCount()+ACELERADORES_PREFIJO);

					 if( info) {
						 menu= (CNewMenu *) info->GetData();
						 menu->AppendMenu(MF_STRING, acel->GetData(), temp);
					 } else {
						 menu= new CNewMenu();
						 menu->CreateMenu();
						 menu->AppendMenu(MF_STRING, acel->GetData(), temp);
						 m_pMenus.Add(grupo, (DWORD) menu);
						}
					}
				}
			}
	 }
	 if(::IsWindow(m_pNavBar))
		m_pNavBar.m_Tree.Expand( hRoot, TVE_EXPAND);
	 return i;
 }

 void CMDIFrameWndXP::OnHelpIndex() {

	 CString temp;
	 temp.Format( "\"%s\"",CApplicacion::GetAppPath("jayuda.exe"));
	 CWnd *child=m_LogoMdiClientWnd.GetTopWindow();
	 if( child && child->IsKindOf(RUNTIME_CLASS(CMDIChildWnd))) {
		 
		 CView *view= ((CMDIChildWnd *) child)->GetActiveView();
		 if(view) {
			 CDocument *documento= view->GetDocument();
			 if(documento)
				 temp+=" /t \"" + documento->GetTitle() + "\"";
		 }
	 }
	 WinExec( temp, SW_NORMAL);
 }

 void CMDIFrameWndXP::LoadBarState(LPCTSTR lpszProfileName)
{
	CDockState state;
	state.LoadState(lpszProfileName);
	SetDockState(state);
}

void CMDIFrameWndXP::SetDockState(const CDockState& state)
{
	int i;

	// first pass through barinfo's sets the m_pBar member correctly
	// creating floating frames or docking floating controls if necessary
	for (i = 0; i < state.m_arrBarInfo.GetSize(); i++)
	{
		CControlBarInfo* pInfo = (CControlBarInfo*)state.m_arrBarInfo[i];
		ASSERT(pInfo != NULL);
		if (pInfo->m_bFloating) // control is floating in the loading state
		{
			// need to create floating frame to match
			CMiniDockFrameWnd* pDockFrame = CreateFloatingFrame(
				pInfo->m_bHorz ? CBRS_ALIGN_TOP : CBRS_ALIGN_LEFT);
			ASSERT(pDockFrame != NULL);
			CRect rect(pInfo->m_pointPos, CSize(10, 10));
			pDockFrame->CalcWindowRect(&rect);
			pDockFrame->SetWindowPos(NULL, rect.left, rect.top, 0, 0,
				SWP_NOSIZE|SWP_NOZORDER|SWP_NOACTIVATE);
			CDockBar* pDockBar =
				(CDockBar*)pDockFrame->GetDlgItem(AFX_IDW_DOCKBAR_FLOAT);
			ASSERT(pDockBar != NULL);
			ASSERT_KINDOF(CDockBar, pDockBar);
			pInfo->m_pBar = pDockBar;
		}
		else // regular dock bar or toolbar in the loading state
		{
			pInfo->m_pBar = GetControlBar(pInfo->m_nBarID);
			// ASSERT(pInfo->m_pBar != NULL); //toolbar id's probably changed
			if (pInfo->m_pBar!=NULL && (pInfo->m_pBar->m_pDockBar != NULL) && 
				(pInfo->m_pBar->m_pDockBar->m_bFloating)) // control is floating in the current state
			{
				// need to dock the control bar
				DockControlBar (pInfo->m_pBar);
			}
		}
		if (pInfo->m_pBar != NULL)
			pInfo->m_pBar->m_nMRUWidth = pInfo->m_nMRUWidth;
	}

	// the second pass will actually dock all of the control bars and
	//  set everything correctly
	for (i = 0; i < state.m_arrBarInfo.GetSize(); i++)
	{
		CControlBarInfo* pInfo = (CControlBarInfo*)state.m_arrBarInfo[i];
		ASSERT(pInfo != NULL);
		if (pInfo->m_pBar != NULL)
			pInfo->m_pBar->SetBarInfo(pInfo, this);
	}

	// last pass shows all the floating windows that were previously shown
	for (i = 0; i < state.m_arrBarInfo.GetSize(); i++)
	{
		CControlBarInfo* pInfo = (CControlBarInfo*)state.m_arrBarInfo[i];
		ASSERT(pInfo != NULL);
		// ASSERT(pInfo->m_pBar != NULL);
		if(pInfo->m_pBar != NULL)
		{
			if (pInfo->m_bFloating)
			{
				CDockBar* pDockBar = (CDockBar*)pInfo->m_pBar;
				ASSERT_KINDOF(CDockBar, pDockBar);
				if (pDockBar->GetDockedVisibleCount() > 0)
				{
    				CFrameWnd* pFrameWnd = pInfo->m_pBar->EnsureParentFrame();
					pFrameWnd->RecalcLayout();
					pFrameWnd->ShowWindow(SW_SHOWNA);
				}
			}
		}
	}
	DelayRecalcLayout();
}

CTreeNavBar	*CMDIFrameWndXP::GetNavigateBar() {

	return &m_pNavBar;
}

// DialogXP.cpp : implementation file
//

#include "stdafx.h"
#include "newmenu/DialogXP.h"

// CDialogXP dialog

IMPLEMENT_DYNAMIC(CDialogXP, CDialog)
CDialogXP::CDialogXP(CWnd* pParent /*=NULL*/)
{
	m_bInMenuLoop = FALSE;
    m_TimerID = NULL;
    m_menubarItemIndex = UINT(-1);
}

CDialogXP::~CDialogXP()
{
}

void CDialogXP::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

CDialogXP::CDialogXP(LPCTSTR lpszTemplateName, CWnd* pParentWnd)
{
  ASSERT(IS_INTRESOURCE(lpszTemplateName) || AfxIsValidString(lpszTemplateName));

	m_bInMenuLoop = FALSE;
	m_TimerID = NULL;
    m_menubarItemIndex = UINT(-1);

  m_pParentWnd = pParentWnd;
  m_lpszTemplateName = lpszTemplateName;
  if (IS_INTRESOURCE(m_lpszTemplateName))
    m_nIDHelp = LOWORD((DWORD_PTR)m_lpszTemplateName);
}

CDialogXP::CDialogXP(UINT nIDTemplate, CWnd* pParentWnd)
{
	m_bInMenuLoop = FALSE;
    m_TimerID = NULL;
    m_menubarItemIndex = UINT(-1);

	m_pParentWnd = pParentWnd;
	m_lpszTemplateName = MAKEINTRESOURCE(nIDTemplate);
	m_nIDHelp = nIDTemplate;
}

BOOL CDialogXP::OnInitDialog()
{
  BOOL bRetval = CDialog::OnInitDialog();

  HMENU hMenu = m_SystemNewMenu.Detach();
  HMENU hSysMenu = ::GetSystemMenu(m_hWnd,FALSE);
  if(hMenu!=hSysMenu)
  {
    if(IsMenu(hMenu))
    {
      if(!::DestroyMenu(hMenu))
      {
        ShowLastError();
      }
    }
  }
  m_SystemNewMenu.Attach(hSysMenu);
  m_DefaultNewMenu.LoadMenu(::GetMenu(m_hWnd));

  if(IsMenu(m_DefaultNewMenu.m_hMenu))
  {
    UpdateMenuBarColor(m_DefaultNewMenu);
  }
  return bRetval;
}


BEGIN_MESSAGE_MAP(CDialogXP, CDialog)
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
END_MESSAGE_MAP()


// CDialogXP message handlers

int CDialogXP::OnCreate(LPCREATESTRUCT lpCreateStruct)
  {
    if (CDialog::OnCreate(lpCreateStruct) == -1)
      return -1;

    if(m_SystemNewMenu.m_hMenu)
    {
      ::DestroyMenu(m_SystemNewMenu.Detach());
    }
    m_SystemNewMenu.Attach(::GetSystemMenu(m_hWnd,FALSE));
    return 0;
}

extern void AFXAPI AfxCancelModes(HWND hWndRcvr);

void CDialogXP::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu) 
{
	AfxCancelModes(m_hWnd);

  // don't support system menu
  if (!bSysMenu)
  {
    ASSERT(pPopupMenu!= NULL);
    // check the enabled state of various menu items

    CCmdUI state;
    state.m_pMenu = pPopupMenu;
    ASSERT(state.m_pOther == NULL);
    ASSERT(state.m_pParentMenu == NULL);

    // determine if menu is popup in top-level menu and set m_pOther to
    //  it if so (m_pParentMenu == NULL indicates that it is secondary popup)
    HMENU hParentMenu;
    if (AfxGetThreadState()->m_hTrackingMenu == pPopupMenu->m_hMenu)
    {
      state.m_pParentMenu = pPopupMenu;    // parent == child for tracking popup
    }
    else if ((hParentMenu = ::GetMenu(m_hWnd)) != NULL)
    {
      CWnd* pParent = GetTopLevelParent();
      // child windows don't have menus -- need to go to the top!
      if (pParent != NULL &&
        (hParentMenu = ::GetMenu(pParent->m_hWnd)) != NULL)
      {
        int nIndexMax = ::GetMenuItemCount(hParentMenu);
        for (int nIndex = 0; nIndex < nIndexMax; nIndex++)
        {
          if (::GetSubMenu(hParentMenu, nIndex) == pPopupMenu->m_hMenu)
          {
            // when popup is found, m_pParentMenu is containing menu
            state.m_pParentMenu = CMenu::FromHandle(hParentMenu);
            break;
          }
        }
      }
    }

    state.m_nIndexMax = pPopupMenu->GetMenuItemCount();
    for (state.m_nIndex = 0; state.m_nIndex<state.m_nIndexMax; state.m_nIndex++)
    {
      state.m_nID = pPopupMenu->GetMenuItemID(state.m_nIndex);
      if (state.m_nID == 0)
        continue; // menu separator or invalid cmd - ignore it

      ASSERT(state.m_pOther == NULL);
      ASSERT(state.m_pMenu != NULL);
      if (state.m_nID == (UINT)-1)
      {
        // possibly a popup menu, route to first item of that popup
        state.m_pSubMenu = pPopupMenu->GetSubMenu(state.m_nIndex);
        if (state.m_pSubMenu == NULL ||
          (state.m_nID = state.m_pSubMenu->GetMenuItemID(0)) == 0 ||
          state.m_nID == (UINT)-1)
        {
          continue;       // first item of popup can't be routed to
        }
        state.DoUpdate(this, FALSE);    // popups are never auto disabled
      }
      else
      {
        // normal menu item
        // Auto enable/disable if frame window has 'm_bAutoMenuEnable'
        //    set and command is _not_ a system command.
        state.m_pSubMenu = NULL;
        state.DoUpdate(this, state.m_nID < 0xF000);
      }

      // adjust for menu deletions and additions
      UINT nCount = pPopupMenu->GetMenuItemCount();
      if (nCount < state.m_nIndexMax)
      {
        state.m_nIndex -= (state.m_nIndexMax - nCount);
        while (state.m_nIndex < nCount &&
          pPopupMenu->GetMenuItemID(state.m_nIndex) == state.m_nID)
        {
          state.m_nIndex++;
        }
      }
      state.m_nIndexMax = nCount;
    }
  }
  // Do the work for update
	CDialog::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);
	CNewMenu::OnInitMenuPopup(m_hWnd,pPopupMenu, nIndex, bSysMenu);
}

  void CDialogXP::OnSysColorChange() 
  {
    CDialog::OnSysColorChange();
    CNewMenu::OnSysColorChange();
    UpdateMenuBarColor(m_DefaultNewMenu);
  }

  void CDialogXP::OnEnterMenuLoop(BOOL bIsTrackPopupMenu)
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
    CDialog::OnEnterMenuLoop(bIsTrackPopupMenu);
  }

  void CDialogXP::OnExitMenuLoop(BOOL bIsTrackPopupMenu)
  {
    m_bInMenuLoop = FALSE;
    CDialog::OnExitMenuLoop(bIsTrackPopupMenu);
  }

  void CDialogXP::OnTimer(UINT_PTR nIDEvent)
  {
    CDialog::OnTimer(nIDEvent);
    if(m_TimerID==nIDEvent)
    {   
      CPoint pt;
      GetCursorPos(&pt);
      SendMessage(WM_NCHITTEST, 0, MAKELONG(pt.x, pt.y));
    }
  }

  void CDialogXP::OnDestroy()
  {
    if(m_TimerID!=NULL)
    {
      KillTimer(m_TimerID);
      m_TimerID = NULL;
    }
    CDialog::OnDestroy();
  }

  LRESULT CDialogXP::OnNcHitTest(CPoint point)
  {
    UINT nHitCode = CDialog::OnNcHitTest(point);
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

  void CDialogXP::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMIS) 
  {
    if(!CNewMenu::OnMeasureItem(GetCurrentMessage()))
    {
      CDialog::OnMeasureItem(nIDCtl, lpMIS);
    }
  } 

  LRESULT CDialogXP::OnMenuChar(UINT nChar, UINT nFlags, CMenu* pMenu) 
  {
    LRESULT lresult;
    if( DYNAMIC_DOWNCAST(CNewMenu,pMenu) )
      lresult=CNewMenu::FindKeyboardShortcut(nChar, nFlags, pMenu);
    else
      lresult=CDialog::OnMenuChar(nChar, nFlags, pMenu);
    
    return lresult;
  }

  void CDialogXP::OnNcPaint()
  {
    CDialog::OnNcPaint();
    DrawSmallBorder();
  }
  
  void CDialogXP::OnPaint()
  {
    CDialog::OnPaint();
    DrawSmallBorder();
  }

#if _MFC_VER < 0x0700 
 	void CDialogXP::OnActivateApp(BOOL bActive, HTASK hTask)
  {
    CDialog::OnActivateApp(bActive, hTask);
    DrawSmallBorder();
  }
#else
  void CDialogXP::OnActivateApp(BOOL bActive, DWORD dwThreadID)
  {
    CDialog::OnActivateApp(bActive, dwThreadID);
    DrawSmallBorder();
  }
#endif

  void CDialogXP::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized )
  {
    CDialog::OnActivate(nState,pWndOther, bMinimized );
    DrawSmallBorder();
  }

  void CDialogXP::DrawSmallBorder()
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


  void CDialogXP::OnEditPaste()
  {
	  GetFocus()->SendMessage(WM_PASTE);
  }

  void CDialogXP::OnEditCopy()
  {
	  GetFocus()->SendMessage(WM_COPY);
  }

  void CDialogXP::OnEditCut()
  {
	  GetFocus()->SendMessage(WM_CUT);
  }

  void CDialogXP::OnEditUndo()
  {
	  GetFocus()->SendMessage(WM_UNDO);
  }

  void CDialogXP::OnEditClear()
  {
	  GetFocus()->SendMessage(WM_CLEAR);
  }

  void CDialogXP::OnEditSelectAll()
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

  void CDialogXP::OnUpdateEditPaste(CCmdUI *pCmdUI)
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

  void CDialogXP::OnUpdateEditUndo(CCmdUI *pCmdUI)
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

  void CDialogXP::OnUpdateEditCopy(CCmdUI *pCmdUI)
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

  void CDialogXP::OnUpdateEditCut(CCmdUI *pCmdUI)
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

  void CDialogXP::OnUpdateEditClear(CCmdUI *pCmdUI)
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

  void CDialogXP::OnUpdateEditSelectAll(CCmdUI *pCmdUI)
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

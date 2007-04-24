#pragma once
# include "newmenu.h"

// CMDIChildWndXP frame

class   __COMPONENTES_EXPORT__ CMDIChildWndXP : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CMDIChildWndXP)
protected:
	CMDIChildWndXP();           // protected constructor used by dynamic creation
	virtual ~CMDIChildWndXP();
protected:
	BOOL m_bInMenuLoop;
	UINT m_TimerID;
	UINT m_menubarItemIndex;

public:
	CNewMenu m_DefaultNewMenu;
	CNewMenu m_SystemNewMenu; 

protected:
	DECLARE_MESSAGE_MAP()
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	void OnSysColorChange();
	void OnEnterMenuLoop(BOOL bIsTrackPopupMenu);
	void OnExitMenuLoop(BOOL bIsTrackPopupMenu);
	void OnTimer(UINT_PTR nIDEvent);
	void OnDestroy();
	LRESULT OnNcHitTest(CPoint point);
	void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMIS);
	LRESULT OnMenuChar(UINT nChar, UINT nFlags, CMenu* pMenu);
	void OnNcPaint();
	void OnPaint();
#if _MFC_VER < 0x0700 
 	void OnActivateApp(BOOL bActive, HTASK hTask);
#else
	void OnActivateApp(BOOL bActive, DWORD dwThreadID);
	void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized );
	void DrawSmallBorder();
#endif
public:
	virtual void ActivateFrame(int nCmdShow = -1);
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	afx_msg void OnChildActivate();
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
};



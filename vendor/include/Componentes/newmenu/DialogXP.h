#pragma once
# include "newmenu.h"

class   __COMPONENTES_EXPORT__ CDialogXP : public CDialog
{

	DECLARE_DYNAMIC(CDialogXP)

	
	CDialogXP(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL);
	CDialogXP(UINT nIDTemplate, CWnd* pParentWnd = NULL);

  // Overridables (special message map entries)
  virtual BOOL OnInitDialog();

protected:
	BOOL	m_bInMenuLoop;
	UINT	m_TimerID;
	UINT	m_menubarItemIndex;

public:
	CNewMenu m_DefaultNewMenu;
	CNewMenu m_SystemNewMenu; 

public:
	CDialogXP(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDialogXP();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

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
	// nuevo para el portapapeles
	afx_msg void OnEditPaste();
	afx_msg void OnEditCopy();
	afx_msg void OnEditCut();
	afx_msg void OnEditUndo();
	afx_msg void OnEditClear();
	afx_msg void OnEditSelectAll();
	afx_msg void OnUpdateEditPaste(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditUndo(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditCopy(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditCut(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditClear(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditSelectAll(CCmdUI *pCmdUI);
};

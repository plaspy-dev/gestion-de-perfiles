#pragma once
# include "../extras/TreeNavBar.h"
# include "../extras/MiscelaneosEx.h"
# include "../logomdi.h"
# include "../MDITabs/MDITabs.h"
# include "../XStatusBar.h"
# include "newmenu.h"
# include "../FadeWnd.h"
# include "GuiDockToolBarEx.h"

/////////////////////////////////////////////////////////////////////////////
// CMDIFrameWndXP for menu to documents

class   __COMPONENTES_EXPORT__ CMDIFrameWndXP : public CMDIFrameWnd
{

  DECLARE_DYNCREATE(CMDIFrameWndXP);
  HMENU m_hShowMenu;

  ULONGLONG		m_ulTotal;
  ULONGLONG		m_ulRealizados;

  int		m_nCurrentHilos;
  int		m_nAnimation;

protected:
	BOOL m_bInMenuLoop;
	UINT m_TimerID;
	UINT m_menubarItemIndex;

	CMiscelaneosEx	m_pMenus;
	CMiscelaneosEx  m_pAceleradores;

	CTreeNavBar		m_pNavBar;
	CMDITabs		m_mdiTabs;
	CLogoMdi		m_LogoMdiClientWnd;
	XStatusBar		m_wndStatusBar;

	CTime			m_pTLocal, m_pTServidor;
	CTimeSpan		m_psDiferencia;
	CFadeWnd		*m_pFadeWnd;

public:
	bool			m_bInactivo;
	int				m_nInactivo;
	bool			m_bFocus;
	bool			m_bConectando;
public:
	~CMDIFrameWndXP();
	CNewMenu m_DefaultNewMenu;
	CNewMenu m_SystemNewMenu; 
	CMDIFrameWndXP();
	BOOL ShowMenu(BOOL bShow);
	virtual	void OnCreateChild(HMENU menu);
#if _MFC_VER < 0x0700 
    // dynamic creation - load frame and associated resources
  virtual BOOL LoadFrame(UINT nIDResource,
        DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE,
        CWnd* pParentWnd = NULL,
        CCreateContext* pContext = NULL);
#endif
    // under MFC 7.0 the next function is virtual so we don't neet to owerwrite
    // loadframe
    BOOL Create(LPCTSTR lpszClassName,
        LPCTSTR lpszWindowName,
        DWORD dwStyle = WS_OVERLAPPEDWINDOW,
        const RECT& rect = rectDefault,
        CWnd* pParentWnd = NULL,        // != NULL for popups
        LPCTSTR lpszMenuName = NULL,
        DWORD dwExStyle = 0,
        CCreateContext* pContext = NULL);

  // control bar docking
  void EnableDocking(DWORD dwDockStyle);
  
  DECLARE_MESSAGE_MAP()
  DECLARE_JDOCK()

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
#endif
	void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized );
	void DrawSmallBorder();

	// nuevo solo para el portapapeles
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
	
protected:
	virtual void LoadState();
	bool AddAcelerator(ACCEL nueva);
	CString ParseAcelerator(CString label, int id);
	bool LoadDocument( CNewMenu *menu, HTREEITEM hRoot, CString nombre, UINT nIDResource,CRuntimeClass* pViewClass, CRuntimeClass *pDocClass=NULL);
	bool LoadDocument( CNewMenu *menu, HTREEITEM hRoot, CString nombre, UINT nIDResource,CRuntimeClass* pViewClass, const CMiscelaneosEx &permisos, DWORD grupo, CRuntimeClass *pDocClass=NULL);
	virtual void SaveState();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	void OnUpdateVentanas(CCmdUI *pCmdUI);
	void OnCommandVentanas(UINT id);
	virtual void OnUpdateFrameTitle(BOOL bAddToTitle);
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	afx_msg LRESULT OnTreeNavegador(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnStatusBar(WPARAM wParam, LPARAM lParam);
	bool LoadDocumentos(DWORD parametro);
	bool LoadDocumentos(const CMiscelaneosEx &permisos, DWORD grupo);
	afx_msg LRESULT OnSetStatus(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnThreadResult(WPARAM wParam, LPARAM lParam);
	void OnUpdateMenus(CCmdUI *pCmdUI);
	void OnCommandMenu(UINT id);
public:
	void DockControlBarLeftOf(CToolBar* Bar, CToolBar* LeftOf);
	void SendMessageBroadCast(UINT message,WPARAM wParam=0, LPARAM lParam=0);
	afx_msg void OnClose();
	afx_msg void OnVerNavegador();
	afx_msg void OnUpdateVerNavegador(CCmdUI *pCmdUI);
	afx_msg LRESULT OnQueryTime(WPARAM wParam, LPARAM lParam);
	afx_msg void OnNextPane();
	afx_msg void OnUpdateNextPane(CCmdUI *pCmdUI);
	afx_msg void OnPrevPane();
	afx_msg void OnUpdatePrevPane(CCmdUI *pCmdUI);
	afx_msg void OnCloseAll();
	int LoadDocumentosEx(CString nombreDLL, HTREEITEM hRoot, const CMiscelaneosEx &permisos, DWORD grupo);
	int LoadDocumentosEx(CString nombreDLL, HTREEITEM hRoot, DWORD parametro);
	afx_msg void OnHelpIndex();
	CTreeNavBar	*GetNavigateBar();
public:
	// estas funciones son necesarias debido a un bug de visual 2008
	void LoadBarState(LPCTSTR lpszProfileName);
	void SetDockState(const CDockState& state);
	// fin bug 2008
};


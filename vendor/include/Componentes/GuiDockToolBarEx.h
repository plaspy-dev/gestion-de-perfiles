#pragma once

#include "GuiControlBar.h"


// definiciones de mapa de mensaje

#define DECLARE_JDOCK()	                                    \
    protected:                                              \
	CGuiDockToolBarEx m_AutoHideBar[4];						\
	afx_msg LRESULT OnJDock(WPARAM, LPARAM);				\
	afx_msg LRESULT OnJInfo(WPARAM, LPARAM);

#define ON_JDOCK_MESSAGES()									\
	ON_MESSAGE( WM_JDOCK, OnJDock)							\
	ON_MESSAGE( WM_JINFO, OnJInfo)

#define IMPLEMENT_JDOCK(theClass)											\
LRESULT theClass::OnJDock(WPARAM wParam, LPARAM lParam)						\
{																			\
	if(wParam<4)															\
		m_AutoHideBar[wParam].AddToolBars((CGuiControlBar *)lParam);		\
	else if(wParam<8) m_AutoHideBar[wParam-4].DeleteToolBars((CGuiControlBar *)lParam);	\
	else if(wParam<12) m_AutoHideBar[wParam-8].UpdateToolBars((CGuiControlBar *)lParam);	\
	return 0;																\
}																			\
																			\
LRESULT theClass::OnJInfo(WPARAM wParam, LPARAM)							\
{																			\
	return (LRESULT)&m_AutoHideBar[wParam];									\
}

# define CREATE_JDOCK(theClass)									\
																\
	m_AutoHideBar[0].Create(theClass,CBRS_ALIGN_RIGHT, 0x997);	\
	m_AutoHideBar[0].EnableDocking(CBRS_ALIGN_RIGHT);			\
	DockControlBar(&m_AutoHideBar[0], AFX_IDW_DOCKBAR_RIGHT);	\
	m_AutoHideBar[1].Create(theClass,CBRS_ALIGN_LEFT, 0x998);	\
	m_AutoHideBar[1].EnableDocking(CBRS_ALIGN_LEFT);			\
	DockControlBar(&m_AutoHideBar[1], AFX_IDW_DOCKBAR_LEFT);	\
	m_AutoHideBar[2].Create(theClass,CBRS_ALIGN_TOP, 0x999);	\
	m_AutoHideBar[2].EnableDocking(CBRS_ALIGN_TOP);				\
	DockControlBar(&m_AutoHideBar[2], AFX_IDW_DOCKBAR_TOP);		\
	m_AutoHideBar[3].Create(theClass, CBRS_ALIGN_BOTTOM, 0x99A);\
	m_AutoHideBar[3].EnableDocking(CBRS_ALIGN_BOTTOM);			\
	DockControlBar(&m_AutoHideBar[3], AFX_IDW_DOCKBAR_BOTTOM);	



static CFont m_fontHorzMenu, m_fontVertMenu;
static int _cyHorzFont, _cyMenuOnBar, _cyTextMargin;
const int CXTEXTMARGIN = 5;

static BOOL InitGlobalFont()
{

	if (m_fontHorzMenu.GetSafeHandle() != NULL &&  m_fontVertMenu.GetSafeHandle() != NULL)
		return TRUE;
	m_fontHorzMenu.DeleteObject();
	m_fontVertMenu.DeleteObject();

	// create fonts
	NONCLIENTMETRICS info; info.cbSize = sizeof(info);
	if(!::SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(info), &info, 0)) {
		
		CFont *fuente= CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT));
		fuente->GetLogFont(&info.lfMenuFont);
	}


	if(!m_fontHorzMenu.CreateFontIndirect(&info.lfMenuFont))
		return FALSE;

	// create vertical font
	info.lfMenuFont.lfEscapement = -900;
	info.lfMenuFont.lfOrientation = -900;
	strcpy(info.lfMenuFont.lfFaceName,"Tahoma");

	if(!m_fontVertMenu.CreateFontIndirect(&info.lfMenuFont))
		return FALSE;

	_cyHorzFont = abs(info.lfMenuFont.lfHeight);

	// calc Y text margin
	_cyMenuOnBar = info.iMenuHeight;
	_cyMenuOnBar = max(_cyMenuOnBar, ::GetSystemMetrics(SM_CYSMICON));
	_cyTextMargin = (_cyMenuOnBar - _cyHorzFont) / 2;

	return TRUE;
}

class __COMPONENTES_EXPORT__ CDockButton : public CWnd
{
// Construction
public:
	CDockButton();

protected:
	CRect   rect;
	CString m_Caption;
	UINT	m_nDockBarAling;
	HWND	m_hWnd;
	BOOL	m_bMouseOver;
	HICON	m_hIcon;
	CSize   m_szBtn;
// Attributes

public:
	
	void SetRect(CRect rc=CRect(0,0,0,0))
	{
		rect=rc;
	}

	HWND  GethWnd() {return m_hWnd; }
	CGuiControlBar* GetBar(HWND hWnd);
	CGuiControlBar* IsControlVisible(CGuiControlBar*);
	void DrawTextHorz(CDC* pDC, CRect m_rect);
	void DrawTextVert(CDC* pDC, CRect m_rect);
	CString GetString(){ return m_Caption;}
	UINT GetAling() {return m_nDockBarAling;}

	void SetIconBtn(HICON hIcon); 
	CSize GetSizeIconBtn()
	{
		if(m_hIcon != NULL)
			return m_szBtn;
		return CSize(0,0);
	} 
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDockButton)
	public:
	virtual BOOL Create(LPCTSTR lpszWindowName,DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, HWND m_hWnd);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDockButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDockButton)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


class __COMPONENTES_EXPORT__ CGuiDockToolBarEx : public CControlBar
{
public:
	CGuiDockToolBarEx();
	int n_numBars;
	CPtrArray	m_ArrayButton;
	virtual BOOL Create(CWnd* pParentWnd, DWORD dwAling, UINT id);
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);
	virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);
	virtual ~CGuiDockToolBarEx();
	void AddToolBars(CGuiControlBar* m_ctrlBars);
	void DeleteToolBars(CGuiControlBar* m_ctrlBars);
	void UpdateToolBars(CGuiControlBar * m_ctrlBars);
	void UpdateBars();
	void CalcButton();
	virtual int GetCount(){return n_numBars;}
	CGuiControlBar* GetBar(int );
protected:
	int      m_Size;
	COLORREF m_clrface;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	afx_msg void OnSysColorChange();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};

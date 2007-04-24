#pragma once
# include "EnBitmap.h"
//***************************************************************************
//
// AUTHOR:  JUHU
//
// DESCRIPTION: ToolBarXP.h: definition of the CToolBarXP class.
//
// ORIGINAL: 
//
//***************************************************************************
class  __COMPONENTES_EXPORT__ CToolBarXP : public CToolBar
{
DECLARE_DYNAMIC(CToolBarXP);
	// Construction
public:
	//***********************************************************************
	// Name:		CToolBarXP 
	// Description:	Constructor.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	CToolBarXP ();

// Attributes
protected:
	CXWinVersion		m_WVersion;
	BYTE m_bCheckVisibility;
	CImageList	m_imgToolbar;
	CImageList	m_imgToolbarDis;
	CImageList	m_imgToolbarNA;
	CEnBitmap	m_bmToolbar;
	CEnBitmap	m_bmToolbarDis;
	CEnBitmap	m_bmToolbarNA;

// Operations
public:
	//***********************************************************************
	// Name:		LoadToolBar 
	// Description:	Hacher visible la barra de herramientas.
	// Parameters:	UINT nIDResource, COLORREF rgbBack=RGB(192,192,192).
	// Return:		BOOL.	
	// Notes:		None.
	//***********************************************************************
	BOOL LoadToolBar (UINT nIDResource, COLORREF rgbBack=RGB(192,192,192));
	//***********************************************************************
	// Name:		InsertControl
	// Description:	Insertar un control a la barra de tareas.
	// Parameters:	int nIndex, CWnd& Ctrl.
	// Return:		bool.	
	// Notes:		None.
	//***********************************************************************
    bool InsertControl (int nIndex, CWnd& Ctrl);
	
protected:
	void TrackPopupMenu (int nID, CMenu* pMenu);
    virtual LRESULT DefWindowProc (UINT nMsg, WPARAM wParam, LPARAM lParam);
    virtual CSize CalcDynamicLayout (int nLength, DWORD nMode);
	virtual void DrawGripper(CDC* pDC, const CRect& rect); 
    afx_msg void OnPaint();
    afx_msg void OnButtonDropDown(NMHDR*, LRESULT*);
	DECLARE_MESSAGE_MAP();
};

#define DROPDOWN(item) ((WORD)-item)

#define IS_STDBTN(button)        (((button).fsStyle&(TBSTYLE_SEP|TBSTYLE_CHECKGROUP))==TBSTYLE_BUTTON)
#define IS_CHECKBTN(button)      (((button).fsStyle&(TBSTYLE_SEP|TBSTYLE_CHECKGROUP))==TBSTYLE_CHECK)
#define IS_GROUPBTN(button)      (((button).fsStyle&(TBSTYLE_SEP|TBSTYLE_CHECKGROUP))==TBSTYLE_CHECKGROUP)
#define IS_DROPDOWN(button)      (((button).fsStyle&TBSTYLE_DROPDOWN)==TBSTYLE_DROPDOWN)
#define IS_SEPARATOR(button)     (((button).fsStyle&TBSTYLE_SEP)&&((button).idCommand==0))
#define IS_CONTROL(button)       (((button).fsStyle&TBSTYLE_SEP)&&((button).idCommand!=0))
#define IS_CHECKED(button)       (((button).fsState&TBSTATE_CHECKED)==TBSTATE_CHECKED)
#define IS_ENABLED(button)       (((button).fsState&TBSTATE_ENABLED)==TBSTATE_ENABLED)
#define IS_INDETERMINATE(button) (((button).fsState&TBSTATE_INDETERMINATE)==TBSTATE_INDETERMINATE)
#define IS_PRESSED(button)       (((button).fsState&TBSTATE_ENABLED)==TBSTATE_PRESSED)
#define IS_VISIBLE(button)       (((button).fsState&TBSTATE_HIDDEN)==0)
#define IS_WRAP(button)          (((button).fsState&TBSTATE_WRAP)==TBSTATE_WRAP)

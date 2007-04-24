#if !defined(AFX_INFOBARCTRL_H__4CCD6BDB_9694_47EC_A643_42A2CEBF62DF__INCLUDED_)
#define AFX_INFOBARCTRL_H__4CCD6BDB_9694_47EC_A643_42A2CEBF62DF__INCLUDED_
//***************************************************************************
//
// AUTHOR:  JUHU
//
// DESCRIPTION: No se desea usar mas esta funcion. Predecapited (NO USAR)
//
// ORIGINAL: 
//
//***************************************************************************
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InfobarCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInfobarCtrl window

class __COMPONENTES_EXPORT__ CInfobarCtrl : public CStatic
{
// Construction
public:
	CInfobarCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInfobarCtrl)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetBkColor(COLORREF color);
	void SetFontColor(COLORREF color);
	void Show(bool show=true);
	virtual ~CInfobarCtrl();
	void Update();

	// Generated message map functions
protected:
	COLORREF m_dwBkColor;
	COLORREF m_dwFontColor;
	//{{AFX_MSG(CInfobarCtrl)
	afx_msg void OnPaint();
	LRESULT OnSetText(WPARAM wParam, LPARAM lParam);
	CRect GetRect();
	CFont	m_font;
	void    SetFuente(CFont &fuente);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INFOBARCTRL_H__4CCD6BDB_9694_47EC_A643_42A2CEBF62DF__INCLUDED_)

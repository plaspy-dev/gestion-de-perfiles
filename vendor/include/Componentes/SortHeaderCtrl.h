#ifndef SORTHEADERCTRL_H
#define SORTHEADERCTRL_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Tools.h"
//***************************************************************************
//
// AUTHOR:  JUHU
//
// DESCRIPTION: Son titulos de columnas y tabs.
//
// ORIGINAL: www.mjsoft.co.uk
//
//***************************************************************************
class   __COMPONENTES_EXPORT__ CSortHeaderCtrl : public CHeaderCtrl
{
// Construction
public:
	//***********************************************************************
	// Name:		CSortHeaderCtrl
	// Description:	Constructor.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	CSortHeaderCtrl();
	//***********************************************************************
	// Name:		CSortHeaderCtrl
	// Description:	Destructor.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	virtual ~CSortHeaderCtrl();

// Attributes
public:
	bool	m_bShowOver;
protected:
	CXWinVersion		m_WVersion;
	COLORREF m_cr3DHighLight;
	COLORREF m_cr3DShadow;
	COLORREF m_cr3DFace;
	COLORREF m_crBtnText;
	COLORREF m_crText;

	CMouseMgr m_MouseMgr;
	int		m_nItemOver;

// Operations
public:
	//***********************************************************************
	// Name:		SetSortArrow
	// Description:	Activar orden decendente o asendente.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	void SetSortArrow( const int iColumn, const BOOL bAscending );
	//***********************************************************************
	// Name:		GetSortColumn
	// Description:	Obtener como esta definido el sort.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	int GetSortColumn(BOOL* bSortCol );

	virtual void Serialize(CArchive& ar);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	void OnAjustarColumnas();
	void OnAjustarContenido();
	int GetHeaderHeight();

protected:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnMouseOut();

	void DrawCtrl(CDC* pDC);
	int DrawText(CDC* pDC, CRect rect, LPHDITEM lphdi, bool over);
	void DrawItem(CDC* pDC, CRect rect, LPHDITEM lphdi, bool sort, bool over);
	afx_msg void OnSysColorChange();
	void DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct );

	int m_iSortColumn;
	BOOL m_bSortAscending;

	virtual void PreSubclassWindow();
DECLARE_MESSAGE_MAP()
};

#endif // SORTHEADERCTRL_H

#pragma once
//***************************************************************************
//
// AUTHOR:  JUHU
//
// DESCRIPTION: Este objeto deciende de CButton, cambia el estilo a XP.
//
// ORIGINAL: 
//
//***************************************************************************

#include "Tools.h"

class  __COMPONENTES_EXPORT__ CButtonXP : public CButton
{
// Construction
public:
	//***********************************************************************
	// Name:		CButtonXP
	// Description:	Default constructor.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
    CButtonXP();
	//***********************************************************************
	// Name:		CButtonXP
	// Description:	Destructor.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		virtual.
	//***********************************************************************
	virtual ~CButtonXP();

// Attributes
public:
	bool	m_bValidate;

protected:
    CMouseMgr		m_MouseMgr;
	CXWinVersion	m_WVersion;

// Operations
public:
    virtual void PreSubclassWindow();
    virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSetFocus(CWnd* pOldWnd);

protected:
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnMouseOut();

DECLARE_MESSAGE_MAP()
};
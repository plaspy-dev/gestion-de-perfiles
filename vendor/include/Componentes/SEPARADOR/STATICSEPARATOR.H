#pragma once
#include "Separator.h"
//***************************************************************************
//
// AUTHOR:  JUHU
//
// DESCRIPTION: CStaticSeparator (Separador de paginas verticales)
//
// ORIGINAL: 
//
//***************************************************************************
class __COMPONENTES_EXPORT__ CStaticSeparator : public CStatic
{
	// Construction
public:
	//***********************************************************************
	// Name:		CStaticSeparator
	// Description:	Constructor.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************	
	CStaticSeparator();
	//***********************************************************************
	// Name:		CStaticSeparator
	// Description:	Destructor.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************	
    virtual ~CStaticSeparator();

// Attributes
protected:
    CFont m_font;
    CSeparator m_ctrlSeparator;
    int m_nOldWidth;

// Operations
public:
	void OnUpdateContents();
    afx_msg void OnSize( UINT nType, int cx, int cy );
    afx_msg void OnMove(int x, int y);
    afx_msg void OnDestroy();
    afx_msg LRESULT OnSetText(WPARAM wParam, LPARAM lParam);
protected:
	virtual void PreSubclassWindow();
DECLARE_MESSAGE_MAP()
};

#pragma once
//***************************************************************************
//
// AUTHOR:  JUHU
//
// DESCRIPTION: Es un separador.
//
// ORIGINAL: 
//
//***************************************************************************
class __COMPONENTES_EXPORT__ CSeparator : public CStatic
{
// Construction
public:
	//***********************************************************************
	// Name:		CSeparator
	// Description:	Constructor.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	CSeparator();
	//***********************************************************************
	// Name:		CSeparator
	// Description:	Destructor.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	virtual ~CSeparator();


// Attributes

// Operations
public:
	//***********************************************************************
	// Name:		Create
	// Description:	Crea un separador a partir CRect.
	// Parameters:	DWORD dwStyle, CRect rc, CWnd* pParent, int nID = 0xffff.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	void Create(DWORD dwStyle, CRect rc, CWnd* pParent, int nID = 0xffff);
	afx_msg void OnPaint();
	afx_msg LRESULT OnPrint(WPARAM, LPARAM);

protected:
    virtual void Print(CDC* pDC, DWORD dwFlags);
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    virtual void PreSubclassWindow();
DECLARE_MESSAGE_MAP()
};

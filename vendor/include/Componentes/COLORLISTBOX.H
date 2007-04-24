//***************************************************************************
//
// AUTHOR:  JUHU
//
// DESCRIPTION: Lista de colores estilo de Visual
//
// ORIGINAL: 
//
//***************************************************************************
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class __COMPONENTES_EXPORT__ CColorListBox : public CListBox
{
// Construction
public:
	//***********************************************************************
	// Name:		CColorListBox
	// Description:	Default constructor.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		See ColorButton.
	//***********************************************************************
	CColorListBox();
	//***********************************************************************
	// Name:		CColorListBox
	// Description:	Destructor.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		Virtual.
	//***********************************************************************
	virtual ~CColorListBox();

// Attributes

// Operations
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);


// Implementation
public:
	int AddString(LPCTSTR lpszItem, COLORREF itemColor = RGB(0,0,0));

DECLARE_MESSAGE_MAP()
};



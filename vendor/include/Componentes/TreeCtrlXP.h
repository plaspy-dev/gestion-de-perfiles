#pragma once
//***************************************************************************
//
// AUTHOR:  JUHU
//
// DESCRIPTION: Un arbolito.
//
// ORIGINAL: 
//
//***************************************************************************
class __COMPONENTES_EXPORT__ CTreeCtrlXP : public CTreeCtrl
{
DECLARE_DYNAMIC(CTreeCtrlXP)
// Construction
public:
	//***********************************************************************
	// Name:		CTreeCtrlXP 
	// Description:	Constructor.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	CTreeCtrlXP();
	//***********************************************************************
	// Name:		CTreeCtrlXP 
	// Description:	Destructor.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	virtual ~CTreeCtrlXP();
public:
	afx_msg void OnPaint();
DECLARE_MESSAGE_MAP()
};



#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "separador/Separator.h"
//***************************************************************************
//
// AUTHOR:  JUHU
//
// DESCRIPTION: Un mensajeBox con opciones.
//
// ORIGINAL: 
//
//***************************************************************************
class  __COMPONENTES_EXPORT__  CnbMessageBox : public CWnd
{
	// Construction
public:
	//***********************************************************************
	// Name:		CnbMessageBox
	// Description:	Constructor.
	// Parameters:	CWnd *pParent = NULL.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	CnbMessageBox(CWnd *pParent = NULL);
	//***********************************************************************
	// Name:		CnbMessageBox
	// Description:	Destructor.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	virtual ~CnbMessageBox();

// Attributes
protected:
	CWnd *m_pParent;
	CButton m_cbDoNotAgain;
	CSeparator m_ctrlSep;

	BOOL m_bChecked;

	int m_nType;

	static CString m_strDoNotShowAgain;
	static CString m_strDoNotAskAgain;

// Operations
public:
	//***********************************************************************
	// Name:		MessageBox
	// Description:	Crea un mensaje box.
	// Parameters:	LPCTSTR lpszText, UINT nType = MB_OK, UINT nIDHelp = 0 .
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
    int MessageBox(LPCTSTR lpszText, UINT nType = MB_OK, UINT nIDHelp = 0 );
	//***********************************************************************
	// Name:		MessageBox
	// Description:	Crea un mensaje box.
	// Parameters:	UINT nIDPrompt, UINT nType = MB_OK, UINT nIDHelp = (UINT) -1 .
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
    int MessageBox( UINT nIDPrompt, UINT nType = MB_OK, UINT nIDHelp = (UINT) -1 );
	//***********************************************************************
	// Name:		SetChecked
	// Description:	Cambia el valor de la opcion del comboBox.
	// Parameters:	BOOL bChecked.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
    void SetChecked(BOOL bChecked);
	//***********************************************************************
	// Name:		GetChecked
	// Description:	Obtiene el valor de la opcion del comboBox.
	// Parameters:	None.
	// Return:		BOOL.	
	// Notes:		None.
	//***********************************************************************
    BOOL GetChecked();

    static void SetDoNotAgainStrings(CString &strDoNotShowAgain, CString &strDoNotAskAgain);
    static void SetDoNotAgainStrings(int nDoNotShowAgainId, int nDoNotAskAgainId);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    LRESULT OnSubclassedInit(WPARAM wParam, LPARAM lParam);
    afx_msg void OnDoNotAgain();

DECLARE_MESSAGE_MAP()
};

#pragma once
//***************************************************************************
//
// AUTHOR:  JUHU
//
// DESCRIPTION: Este clase crea una lista de checkBoxes en donde es posible
// mediante el teclado el nombre de label ubicar el foco de la 
// aplicacion en el checkBox requerido.
//
// ORIGINAL: 
//
//***************************************************************************

class  __COMPONENTES_EXPORT__ CCheckListBoxEx : public CCheckListBox
{
	DECLARE_DYNAMIC(CCheckListBoxEx)
// Construction
public:
	//***********************************************************************
	// Name:		CCheckListBoxEx
	// Description:	Default constructor.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	CCheckListBoxEx();
	//***********************************************************************
	// Name:		CCheckListBoxEx
	// Description:	Destructor.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	~CCheckListBoxEx();

// Attributes
public:
	bool		m_bSaltoTab;
	CString		m_szUsuario;
	
// Operations
// Propias
public:
	//***********************************************************************
	// Name:		LoadDataFromXML
	// Description:	Crea la lista de CheckBoxes a partir de un XML.
	// Parameters:	CString.
	// Return:		void.	
	// Notes:		requiere que el XML tenga cierta estructura.
	//***********************************************************************
	void LoadDataFromXML(CString Archivo);

//visual
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnTimer(UINT nIDEvent);
DECLARE_MESSAGE_MAP()
};



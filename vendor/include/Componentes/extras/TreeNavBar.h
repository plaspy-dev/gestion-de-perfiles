# pragma once
#include "../GuiControlBar.h"
#include "../treectrlxp.h"
//***************************************************************************
//
// AUTHOR:  JUHU
//
// DESCRIPTION: Es el arbol que esta en el x contraible.
//
// ORIGINAL: 
//
//***************************************************************************
class __COMPONENTES_EXPORT__ CTreeNavBar : public CGuiControlBar
{
	// Construction
public:
	//***********************************************************************
	// Name:		CTreeCtrlXP 
	// Description:	Constructor.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	CTreeNavBar();
	//***********************************************************************
	// Name:		CTreeCtrlXP 
	// Description:	Destructor.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	virtual ~CTreeNavBar();
	
// Attributes
public:
	bool	m_bHiden;
	CTreeCtrlXP m_Tree;

// Operations
public:
	//***********************************************************************
	// Name:		FindItem
	// Description:	Encontrar un item por identificador.
	// Parameters:	UINT id, HTREEITEM hItem=NULL.
	// Return:		HTREEITEM.	
	// Notes:		None.
	//***********************************************************************
	HTREEITEM FindItem(UINT id, HTREEITEM hItem=NULL);
	//***********************************************************************
	// Name:		FindItem
	// Description:	Encontrar un item por nombre.
	// Parameters:	CString nombre, HTREEITEM hItem=NULL.
	// Return:		HTREEITEM.	
	// Notes:		None.
	//***********************************************************************
	HTREEITEM FindItem(CString nombre, HTREEITEM hItem=NULL);
	//***********************************************************************
	// Name:		GetNextItemEx
	// Description:	Obtener el siguiente item despues del ingresado.
	// Parameters:	HTREEITEM hItem.
	// Return:		HTREEITEM.	
	// Notes:		None.
	//***********************************************************************
	HTREEITEM GetNextItemEx(HTREEITEM hItem);
	//***********************************************************************
	// Name:		FindItemEx
	// Description:	Encontrar el nombre de la ventana con la cual esta asociada.
	// Parameters:	CString nombre, HTREEITEM hItem=NULL.
	// Return:		HTREEITEM.	
	// Notes:		None.
	//***********************************************************************
	HTREEITEM FindItemEx(CString nombre, HTREEITEM hItem=NULL);
	//***********************************************************************
	// Name:		SetSel
	// Description:	Ubique el foco en este elemento.
	// Parameters:	UINT id.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	void SetSel( UINT id);
	//***********************************************************************
	// Name:		SetSel
	// Description:	Ubique el foco en este elemento mediante la vista asociada.
	// Parameters:	CString vista.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	void SetSel( CString vista);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnChangeSelArbol(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OpenDocument();
	afx_msg void OnChangeSelArborReal(NMHDR* pNMHDR, LRESULT* pResult);
	
DECLARE_MESSAGE_MAP()
};


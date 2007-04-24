#pragma once
#include "mylistctrl.h"
//***************************************************************************
//
// AUTHOR:  JUHU
//
// DESCRIPTION: Virtual list.
//
// ORIGINAL: 
//
//***************************************************************************
class __COMPONENTES_EXPORT__ CVListCtrl :	public CMyListCtrl
{
DECLARE_DYNAMIC(CVListCtrl)
	// Construction
public:
	//***********************************************************************
	// Name:		CVListCtrl
	// Description:	Constructor.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	CVListCtrl(void);
	//***********************************************************************
	// Name:		CVListCtrl
	// Description:	Destructor.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	~CVListCtrl(void);

// Attributes
protected:
	CPtrArray	m_pItems;

// Operations
public:
	//***********************************************************************
	// Name:		SetItemData
	// Description:	Nuevo intem en la pocicion nItem.
	// Parameters:	int nItem, DWORD_PTR dwData.
	// Return:		BOOL.	
	// Notes:		None.
	//***********************************************************************
	BOOL SetItemData(int nItem, DWORD_PTR dwData);
	//***********************************************************************
	// Name:		DeleteItem
	// Description:	Borra item en la posicicon nItem.
	// Parameters:	int nItem.
	// Return:		BOOL.	
	// Notes:		None.
	//***********************************************************************
	BOOL DeleteItem(int nItem);
	//***********************************************************************
	// Name:		DeleteAllItems
	// Description:	Eliminar todos los items.
	// Parameters:	None.
	// Return:		BOOL.	
	// Notes:		None.
	//***********************************************************************
	BOOL DeleteAllItems( );
	//***********************************************************************
	// Name:		GetCheck
	// Description:	El checkBox en la lista (el chulito).
	// Parameters:	int nItem.
	// Return:		BOOL.	
	// Notes:		None.
	//***********************************************************************
	BOOL GetCheck(int nItem);
	//***********************************************************************
	// Name:		SetCheck
	// Description:	El checkBox en la lista (el chulito).
	// Parameters:	int nItem,BOOL fCheck = TRUE.
	// Return:		BOOL.	
	// Notes:		None.
	//***********************************************************************
	BOOL SetCheck(int nItem,BOOL fCheck = TRUE);
	//***********************************************************************
	// Name:		SetItemText
	// Description:	Colocar texto en la pocicion nItem.
	// Parameters:	int nItem,int nSubItem,LPCTSTR lpszText.
	// Return:		BOOL.	
	// Notes:		None.
	//***********************************************************************
	BOOL SetItemText(int nItem,int nSubItem,LPCTSTR lpszText);
	//***********************************************************************
	// Name:		*GetItems
	// Description:	Traer todos los items.
	// Parameters:	None.
	// Return:		CPtrArray.	
	// Notes:		None.
	//***********************************************************************
	CPtrArray *GetItems();
	//***********************************************************************
	// Name:		GetItemData
	// Description:	Traer el item en la pocicion nItem.
	// Parameters:	int nItem.
	// Return:		DWORD_PTR.	
	// Notes:		None.
	//***********************************************************************
	DWORD_PTR GetItemData(int nItem);
	//***********************************************************************
	// Name:		GetItemText
	// Description:	Traer el texto en la pocicion nItem.
	// Parameters:	int nItem, int nSubItem=0.
	// Return:		CString.	
	// Notes:		None.
	//***********************************************************************
	CString GetItemText(int nItem, int nSubItem=0);

	//***********************************************************************
	// Name:		InsertItem
	// Description:	.
	// Parameters:	None.
	// Return:		BOOL.	
	// Notes:		None.
	//***********************************************************************
	virtual int InsertItem(int nItem,LPCTSTR lpszItem);
	//***********************************************************************
	// Name:		InsertItem
	// Description:	.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	virtual int InsertItem(int nItem,LPCTSTR lpszItem,int nImage);


	//convertimos la lista en virtual
	afx_msg void OnGetdispinfoList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnOdfinditemList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnOdcachehintList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();

protected:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
DECLARE_MESSAGE_MAP()
};

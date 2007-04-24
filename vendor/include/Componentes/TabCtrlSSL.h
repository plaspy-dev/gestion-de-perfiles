#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "TabPageSSL.h"

#ifdef _DEBUG
#pragma pack (push, 1)

typedef struct {
	WORD dlgVer;
	WORD signature;
	DWORD helpID;
	DWORD exStyle;
	DWORD style;
	WORD cDlgItems;
	short x;
	short y;
	short cx;
	short cy;
} DLGTEMPLATEEX;

#pragma pack (pop)
#endif // _DEBUG
//***************************************************************************
//
// AUTHOR:  JUHU
//
// DESCRIPTION: Tabs dentro de tab.
//
// ORIGINAL: 
//
//***************************************************************************

class __COMPONENTES_EXPORT__ CTabCtrlSSL : public CTabCtrl {
	// Construction
public:
	//***********************************************************************
	// Name:		CTabCtrlSSL
	// Description:	Constructor.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************	
	CTabCtrlSSL();
	//***********************************************************************
	// Name:		CTabCtrlSSL
	// Description:	Constructor.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************	
	virtual ~CTabCtrlSSL (void);

// Attributes
protected:
	struct TabDelete {
		CTabPageSSL*   pTabPage;
		BOOL        bDelete;
	};
	CArray<TabDelete, TabDelete> m_tabs;
	CArray<HWND, HWND> m_hFocusWnd;
	CArray<int, int> m_nPageIDs;

// Operations
public:
	//***********************************************************************
	// Name:		AddSSLPage 
	// Description:	Añade un nuevo formulario.
	// Parameters:	LPCTSTR pszTitle, int nPageID, CTabPageSSL* pTabPage.
	// Return:		int.	
	// Notes:		None.
	//***********************************************************************	
	int		AddSSLPage (LPCTSTR pszTitle, int nPageID, CTabPageSSL* pTabPage);
	//***********************************************************************
	// Name:		AddSSLPage 
	// Description:	Añade un nuevo formulario.
	// Parameters:	LPCTSTR pszTitle, int nPageID,	LPCTSTR pszTemplateName.
	// Return:		int.	
	// Notes:		None.
	//***********************************************************************	
	int		AddSSLPage (LPCTSTR pszTitle, int nPageID,	LPCTSTR pszTemplateName);
	//***********************************************************************
	// Name:		AddSSLPage 
	// Description:	Añade un nuevo formulario.
	// Parameters:	LPCTSTR pszTitle, int nPageID, int nTemplateID.
	// Return:		int.	
	// Notes:		None.
	//***********************************************************************	
	int		AddSSLPage (LPCTSTR pszTitle, int nPageID, int nTemplateID);
	//***********************************************************************
	// Name:		RemoveSSLPage 
	// Description:	Remueve un formulario.
	// Parameters:	int nIndex.
	// Return:		BOOL.	
	// Notes:		None.
	//***********************************************************************	
	BOOL	RemoveSSLPage (int nIndex);
	//***********************************************************************
	// Name:		GetSSLPageCount
	// Description:	Obtiene el numero actual de tabs.
	// Parameters:	None.
	// Return:		int.	
	// Notes:		None.
	//***********************************************************************	
	int		GetSSLPageCount (void);
	//***********************************************************************
	// Name:		GetSSLPageTitle 
	// Description:	Obtiene el tituloo del tab.
	// Parameters:	int nIndex, CString& strTitle.
	// Return:		BOOL.	
	// Notes:		None.
	//***********************************************************************	
	BOOL	GetSSLPageTitle (int nIndex, CString& strTitle);
	//***********************************************************************
	// Name:		SetSSLPageTitle
	// Description:	Fija el tituloo del tab.
	// Parameters:	int nIndex, LPCTSTR pszTitle.
	// Return:		BOOL.	
	// Notes:		None.
	//***********************************************************************	
	BOOL	SetSSLPageTitle (int nIndex, LPCTSTR pszTitle);
	//***********************************************************************
	// Name:		GetSSLPageID 
	// Description:	Destructor.
	// Parameters:	int nIndex.
	// Return:		int.	
	// Notes:		None.
	//***********************************************************************	
	int		GetSSLPageID (int nIndex);
	//***********************************************************************
	// Name:		SetSSLPageID
	// Description:	Cambia el ID del dialog.
	// Parameters:	int nIndex, int nPageID.
	// Return:		int.	
	// Notes:		None.
	//***********************************************************************	
	int		SetSSLPageID (int nIndex, int nPageID);
	//***********************************************************************
	// Name:		ActivateSSLPage
	// Description:	Activa un tab ("Lo sobre pone sobre los demas").
	// Parameters:	int nIndex.
	// Return:		BOOL.	
	// Notes:		None.
	//***********************************************************************	
	BOOL	ActivateSSLPage (int nIndex);
	//***********************************************************************
	// Name:		GetSSLActivePage
	// Description:	Obtiene el tab activo en el momento.
	// Parameters:	None.
	// Return:		int.	
	// Notes:		None.
	//***********************************************************************	
	int		GetSSLActivePage (void);
	//***********************************************************************
	// Name:		GetSSLPage 
	// Description:	Retorna el puntero a un tab.
	// Parameters:	int nIndex.
	// Return:		CWnd*.	
	// Notes:		None.
	//***********************************************************************	
	CWnd*	GetSSLPage (int nIndex);
	//***********************************************************************
	// Name:		GetSSLPageIndex
	// Description:	Destructor.
	// Parameters:	int nPageID.
	// Return:		int.	
	// Notes:		None.
	//***********************************************************************	
	int		GetSSLPageIndex (int nPageID);
	
	afx_msg void OnSize(UINT nType, int cx, int cy);

protected:
	int AddPage (LPCTSTR pszTitle, int nPageID, TabDelete tabDelete);
    virtual BOOL OnInitPage (int nIndex, int nPageID);
	virtual void OnActivatePage (int nIndex, int nPageID);
	virtual void OnDeactivatePage (int nIndex, int nPageID);
	virtual void OnDestroyPage (int nIndex, int nPageID);
	virtual BOOL OnCommand (WPARAM wParam, LPARAM lParam);
	virtual BOOL OnNotify (WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual BOOL OnCmdMsg (UINT nID, int nCode, void* pExtra,
		AFX_CMDHANDLERINFO* pHandlerInfo);
	afx_msg void OnDestroy (void);
	afx_msg void OnSetFocus (CWnd* pOldWnd);
	afx_msg void OnKillFocus (CWnd* pNewWnd);
	afx_msg void OnSelChanging (NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelChange (NMHDR* pNMHDR, LRESULT* pResult);

#ifdef _DEBUG
	BOOL CheckDialogTemplate (LPCTSTR pszTemplateName);
#endif // _DEBUG
	void ResizeDialog (int nIndex, int cx, int cy);
DECLARE_MESSAGE_MAP()
};
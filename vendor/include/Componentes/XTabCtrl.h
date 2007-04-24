#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <afxtempl.h>
//***************************************************************************
//
// AUTHOR:  JUHU
//
// DESCRIPTION: Tabs dentro de tab (see TabCtrlSSL).
//
// ORIGINAL: 
//
//***************************************************************************
class __COMPONENTES_EXPORT__ CXTabCtrl : public CTabCtrl
{
	// Construction
public:
	//***********************************************************************
	// Name:		CXTabCtrl()
	// Description:	Constructor.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************	
	CXTabCtrl();
	//***********************************************************************
	// Name:		CXTabCtrl()
	// Description:	Desstructor.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************	
	virtual ~CXTabCtrl();

// Attributes
public:
protected:
	CArray<BOOL, BOOL> m_arrayStatusTab; //** enabled Y\N
	int m_iSelectedTab;
	POINT m_ptTabs;
	COLORREF m_crSelected;
	COLORREF m_crDisabled;
	COLORREF m_crNormal;
	COLORREF m_crMouseOver;

	int m_iIndexMouseOver;

	bool m_bMouseOver;
	bool m_bColorMouseOver;
	bool m_bColorNormal;
	bool m_bColorDisabled;
	bool m_bColorSelected;


// Operations
public:
	//***********************************************************************
	// Name:		AddTab
	// Description:	Añade un nuevo tab.
	// Parameters:	CWnd* pWnd, LPTSTR lpszCaption, int iImage =0.
	// Return:		int.	
	// Notes:		None.
	//***********************************************************************	
	void AddTab(CWnd* pWnd, LPTSTR lpszCaption, int iImage =0);
	//***********************************************************************
	// Name:		EnableTab
	// Description:	Permite editar un tab iIndex.
	// Parameters:	CWnd* pWnd, LPTSTR lpszCaption, int iImage =0.
	// Return:		int.	
	// Notes:		None.
	//***********************************************************************	
	void EnableTab(int iIndex, BOOL bEnable = TRUE);
	//***********************************************************************
	// Name:		SelectTab
	// Description:	Permite editar un tab iIndex.
	// Parameters:	int iIndex.
	// Return:		BOOL.	
	// Notes:		None.
	//***********************************************************************	
	BOOL SelectTab(int iIndex);
	//***********************************************************************
	// Name:		DeleteAllTabs
	// Description:	Cierra todos los tabs.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************	
	void DeleteAllTabs();
	//***********************************************************************
	// Name:		DeleteTab
	// Description:	Cierra el tab iIndex.
	// Parameters:	int iIndex.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************	
	void DeleteTab(int iIndex);
	//***********************************************************************
	// Name:		SetTopLeftCorner
	// Description:	Indique la esquina superior derecha del conjunto de tabs.
	// Parameters:	CPoint pt.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************	
	void SetTopLeftCorner(CPoint pt);
	//***********************************************************************
	// Name:		IsTabEnabled
	// Description:	Indique la esquina superior derecha del conjunto de tabs.
	// Parameters:	int iIndex.
	// Return:		BOOL.	
	// Notes:		None.
	//***********************************************************************	
	BOOL IsTabEnabled(int iIndex);
	//***********************************************************************
	// Name:		SetDisabledColor
	// Description:	Ajuste el color de "deshabilitado".
	// Parameters:	COLORREF cr.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************	
	void SetDisabledColor(COLORREF cr);
	//***********************************************************************
	// Name:		SetSelectedColor
	// Description:	Ajuste el color de "Selecionado".
	// Parameters:	COLORREF cr.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************	
	void SetSelectedColor(COLORREF cr);
	//***********************************************************************
	// Name:		SetNormalColor
	// Description:	Ajuste el color de "default".
	// Parameters:	COLORREF cr.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************	
	void SetNormalColor(COLORREF cr);
	//***********************************************************************
	// Name:		SetMouseOverColor
	// Description:	Ajuste el color de "Foco con el mouse".
	// Parameters:	COLORREF cr.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************	
	void SetMouseOverColor(COLORREF cr);
	//***********************************************************************
	// Name:		SelectNextTab
	// Description:	Muestreme el siguiente tab.
	// Parameters:	BOOL bForward.
	// Return:		BOOL.	
	// Notes:		None.
	//***********************************************************************	
	BOOL SelectNextTab(BOOL bForward);
	//***********************************************************************
	// Name:		SelectTabByName
	// Description:	Muestre el tab que contenga lpszTabCaption.
	// Parameters:	LPCTSTR lpszTabCaption.
	// Return:		BOOL.	
	// Notes:		None.
	//***********************************************************************	
	BOOL SelectTabByName(LPCTSTR lpszTabCaption);
	//***********************************************************************
	// Name:		ChangeTab
	// Description:	Cambie determindado tab (iIndex) por (pNewTab).
	// Parameters:	int iIndex, CWnd* pNewTab, LPTSTR lpszCaption, int iImage.
	// Return:		BOOL.	
	// Notes:		None.
	//***********************************************************************	
	void ChangeTab(int iIndex, CWnd* pNewTab, LPTSTR lpszCaption, int iImage);
	
	afx_msg void OnSelchange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchanging(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);

protected:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void PreSubclassWindow();


DECLARE_MESSAGE_MAP()
};

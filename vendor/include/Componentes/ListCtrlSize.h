//***************************************************************************
//
// AUTHOR:  JUHU
//
// DESCRIPTION: Lista con tamaño variable dependiendo del contenido.
//
// ORIGINAL: 
//
//***************************************************************************
#pragma once
#include "mylistctrl.h"

class  __COMPONENTES_EXPORT__ CListCtrlSize : public CMyListCtrl
{
// Construction
public:
	//***********************************************************************
	// Name:		CListCtrlSize
	// Description:	Default constructor.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	CListCtrlSize(void);
	//***********************************************************************
	// Name:		CListCtrlSize
	// Description:	Destructor.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	~CListCtrlSize(void);

// Attributes
protected:
	CWnd		*m_pLabel;
	CWnd		*m_pFin;
	CSize	m_sizeMin;
	CSize	m_sizeMax;
	CSize	m_sizeDelta;
	
// Operaciones
public:
	void Init();
	//***********************************************************************
	// Name:		SetLabel
	// Description:	Es el marco de la lista.
	// Parameters:	CWnd.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	// funcion obsoleta ya no se usa, se detecta automaticamente
	void SetLabel(CWnd *label);
	//***********************************************************************
	// Name:		SetFin
	// Description:	Final de la lista.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	void SetFin(CWnd *fin);
	//***********************************************************************
	// Name:		SetMinSize
	// Description:	Minimo tamaño.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	void SetMinSize(CSize size);
	//***********************************************************************
	// Name:		SetMaxSize
	// Description:	Maximo tamaño.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	void SetMaxSize(CSize size);
	//***********************************************************************
	// Name:		UpdateSize
	// Description:	Actualizar el tamaño.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	void UpdateSize();
	void RealUpdateSize();

protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void UpdateCtrlItems();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
public:
	BOOL DeleteItem(int nItem);
	BOOL DeleteAllItems();
	virtual BOOL SetItemText(int nItem,int nSubItem,LPCTSTR lpszText);
	virtual int InsertItem(int nItem,LPCTSTR lpszItem);
	virtual int InsertItem(int nItem,LPCTSTR lpszItem,int nImage);
	int	AddItem(LPCTSTR lpszItem);
DECLARE_MESSAGE_MAP()
afx_msg void OnTimer(UINT_PTR nIDEvent);
afx_msg void OnDestroy();
};

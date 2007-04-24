#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
//***************************************************************************
//
// AUTHOR:  JUHU
//
// DESCRIPTION: Forms para TabCtrlSSL
//
// ORIGINAL: Barra de progreso
//
//***************************************************************************
class __COMPONENTES_EXPORT__ CTextProgressCtrl : public CProgressCtrl
{
// Construction
public:
	//***********************************************************************
	// Name:		CTabPageSSL
	// Description:	Constructor.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	CTextProgressCtrl();
	//***********************************************************************
	// Name:		CTabPageSSL
	// Description:	Destructor.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	virtual ~CTextProgressCtrl();

// Attributes
protected:
	int	m_nPos, 
		m_nStepSize, 
		m_nMax, 
		m_nMin;
	CString		m_strText;
	BOOL		m_bShowText;
	int			m_nBarWidth;
	COLORREF	m_colFore,
				m_colBk,
				m_colTextFore,
				m_colTextBk;
	CFont		m_fountHorz;
	DWORD		m_dwFormato;


// Operations
public:
	//***********************************************************************
	// Name:		SetPos
	// Description:	Coloca el cursor en la pocicion indicada.
	// Parameters:	None.
	// Return:		int.	
	// Notes:		See SetRange.
	//***********************************************************************
    int			SetPos(int nPos);
	//***********************************************************************
	// Name:		StepIt
	// Description:	Nuevo paso (avance un poco).
	// Parameters:	None.
	// Return:		int.	
	// Notes:		None.
	//***********************************************************************
    int			StepIt();
	//***********************************************************************
	// Name:		SetRange
	// Description:	Definir el rango de la barra.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
    void		SetRange(int nLower, int nUpper);
	//***********************************************************************
	// Name:		OffsetPos
	// Description:	Fuera del ranbo (terminar).
	// Parameters:	int nPos.
	// Return:		int.	
	// Notes:		None.
	//***********************************************************************
    int			OffsetPos(int nPos);
	//***********************************************************************
	// Name:		SetStep
	// Description:	Nuevo paso.
	// Parameters:	int nStep.
	// Return:		int.	
	// Notes:		None.
	//***********************************************************************
    int			SetStep(int nStep);
	//***********************************************************************
	// Name:		SetStep
	// Description:	Color de la estela.
	// Parameters:	COLORREF col.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	void		SetForeColour(COLORREF col);
	//***********************************************************************
	// Name:		SetBkColour
	// Description:	Color del fondo.
	// Parameters:	COLORREF col.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	void		SetBkColour(COLORREF col);
	//***********************************************************************
	// Name:		SetTextForeColour
	// Description:	Color del texto de la estela.
	// Parameters:	COLORREF col.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	void		SetTextForeColour(COLORREF col);
	//***********************************************************************
	// Name:		SetTextForeColour
	// Description:	Color del texto de fondo.
	// Parameters:	COLORREF col.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	void		SetTextBkColour(COLORREF col);
	//***********************************************************************
	// Name:		GetForeColour
	// Description:	Obtener el color de la estela.
	// Parameters:	None.
	// Return:		COLORREF col.	
	// Notes:		None.
	//***********************************************************************
	COLORREF	GetForeColour();
	//***********************************************************************
	// Name:		GetBkColour
	// Description:	Obtener el color de la estela.
	// Parameters:	None.
	// Return:		COLORREF col.	
	// Notes:		None.
	//***********************************************************************
	COLORREF	GetBkColour();
	//***********************************************************************
	// Name:		GetTextForeColour
	// Description:	Obtener el color de la estela.
	// Parameters:	COLORREF col.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	COLORREF	GetTextForeColour();
	//***********************************************************************
	// Name:		GetTextBkColour
	// Description:	Obtener el color del texto de fondo.
	// Parameters:	None.
	// Return:		COLORREF col.	
	// Notes:		None.
	//***********************************************************************
	COLORREF	GetTextBkColour();
	//***********************************************************************
	// Name:		SetShowText
	// Description:	Hacer el texto visible.
	// Parameters:	BOOL.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
    void		SetShowText(BOOL bShow);
	//***********************************************************************
	// Name:		SetFormato
	// Description:	Establece el formato del texto de la barra de progreso.
	// Parameters:	formato.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	void		SetFormato(DWORD formato=DT_CENTER | DT_VCENTER | DT_SINGLELINE);

protected:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg LRESULT OnSetText(UINT, LPARAM szText);
    afx_msg LRESULT OnGetText(UINT cchTextMax, LPARAM szText);
	afx_msg void	OnSysColorChange();
DECLARE_MESSAGE_MAP()
};

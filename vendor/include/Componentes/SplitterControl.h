#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define CW_LEFTALIGN 1
#define CW_RIGHTALIGN 2
#define CW_TOPALIGN 3
#define CW_BOTTOMALIGN 4
#define SPS_VERTICAL 1
#define SPS_HORIZONTAL 2
typedef struct SPC_NMHDR
{
	NMHDR hdr;
	int delta;
} SPC_NMHDR;
//***************************************************************************
//
// AUTHOR:  JUHU
//
// DESCRIPTION: Spliter (Separador de paginas verticales)
//
// ORIGINAL: 
//
//***************************************************************************
class   __COMPONENTES_EXPORT__ CSplitterControl : public CStatic
{
// Construction
public:
	//***********************************************************************
	// Name:		CSplitterControl
	// Description:	Constructor.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************	
	CSplitterControl();
	//***********************************************************************
	// Name:		CSplitterControl
	// Description:	Destructor.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************	
	virtual	~CSplitterControl();

// Attributes
public:
protected:
	BOOL		m_bIsPressed;
	int			m_nType;
	int			m_nX, m_nY;
	int			m_nMin, m_nMax;
	int			m_nSavePos;		// Save point on the lbutton down 
								// message
// Operations
public:
	//***********************************************************************
	// Name:		ChangePos
	// Description:	Cambiar la posicion.
	// Parameters:	CWnd* pWnd, int dx, int dy.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	static void ChangePos(CWnd* pWnd, int dx, int dy);
	//***********************************************************************
	// Name:		ChangeWidth
	// Description:	Cambiar el ancho.
	// Parameters:	CWnd* pWnd, int dx, DWORD dwFlag = CW_LEFTALIGN.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	static void ChangeWidth(CWnd* pWnd, int dx, DWORD dwFlag = CW_LEFTALIGN);
	//***********************************************************************
	// Name:		ChangeHeight
	// Description:	Cambiar el alto.
	// Parameters:	CWnd* pWnd, int dy, DWORD dwFlag = CW_TOPALIGN.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	static void ChangeHeight(CWnd* pWnd, int dy, DWORD dwFlag = CW_TOPALIGN);
	//***********************************************************************
	// Name:		SetRange
	// Description:	Cambiar el rango.
	// Parameters:	int nMin, int nMax.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	void		SetRange(int nMin, int nMax);
	//***********************************************************************
	// Name:		SetRange
	// Description:	Cambiar la rango con respecto a la pocicion actual.
	// Parameters:	int nSubtraction, int nAddition, int nRoot.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	void		SetRange(int nSubtraction, int nAddition, int nRoot);
	//***********************************************************************
	// Name:		GetStyle
	// Description:	Obtener el estilo definido.
	// Parameters:	CWnd* pWnd, int dx, int dy.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	int			GetStyle();
	//***********************************************************************
	// Name:		SetStyle
	// Description:	Cambiar el estilo.
	// Parameters:	int nStyle = SPS_VERTICAL.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	int			SetStyle(int nStyle = SPS_VERTICAL);
	//***********************************************************************
	// Name:		Create
	// Description:	Crear el objeto.
	// Parameters:	DWORD dwStyle, const CRect& rect, CWnd* pParent, UINT nID.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	void		Create(DWORD dwStyle, const CRect& rect, CWnd* pParent, UINT nID);

protected:
	virtual void	DrawLine(CDC* pDC, int x, int y);
	void			MoveWindowTo(CPoint pt);

	afx_msg void	OnPaint();
	afx_msg void	OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL	OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void	OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void	OnLButtonUp(UINT nFlags, CPoint point);

DECLARE_MESSAGE_MAP()
};

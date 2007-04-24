#pragma once
# include <afxtempl.h>
//***************************************************************************
//
// AUTHOR:  JUHU
//
// DESCRIPTION: This tooltip control implements:
//
//		o An immediate popup tip window
//		o Normal control tooltips
//
//		The tip window can display an icon and multi-line
//		tip text.  Seperate multiple text lines with a '\n'.
//		The tip window's colors are derrived from
//		system window, window text, and scroll bar colors.
//
//		Create the control by calling the Create() method.
//
//		To display immediate tips, call the Show() method.
//		Call SetIcon() to set the icon used for immediate tips.
//
//		Use AddTool() to add tool windows.  RelayEvent() must
//		be called from the parent windows's PreTranslateMessage().
//
//		Call SetShowDelay() to adjust the tip popup delay.
//
//		Call SetFont() to change the tooltip text font.  The default
//		is the system default GUI font.
//
//
// ORIGINAL: Mark Bozeman		09-16-2001
//
//***************************************************************************
class __COMPONENTES_EXPORT__ CXInfoTip : public CToolTipCtrl
{
// Construction
public:
	//***********************************************************************
	// Name:		CVListCtrl
	// Description:	Constructor.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	CXInfoTip();
	//***********************************************************************
	// Name:		CVListCtrl
	// Description:	Destructor.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	virtual ~CXInfoTip();

// Attributes
protected:
	///////////////////////////////////////////////////////////////////////////
	// Tool information structure
	///////////////////////////////////////////////////////////////////////////
	// Timer identifiers
	enum
	{
		timerShow			= 100,								// Show timer
		timerHide			= 101								// Hide timer
	};
	int			m_nShowDelay;									// Show delay
	CPoint		m_ptOrigin;										// Popup origin
	CString		m_szText;										// Tip text
	UINT		m_nTimer;										// Show/hide timer
	CWnd		*m_pParent;
	bool		m_bVisible;
// Operations
public:
	//***********************************************************************
	// Name:		Create
	// Description:	Asociacion con la clase "padre".
	// Parameters:	CWnd *parent.
	// Return:		BOOL.	
	// Notes:		None.
	//***********************************************************************
	BOOL Create(CWnd *parent, bool globo=true);
	//***********************************************************************
	// Name:		Show
	// Description:	Mostrar.
	// Parameters:	CString szText, CPoint *pt = NULL, bool titulo=false.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	void Show(CString szText, CPoint *pt = NULL);
	//***********************************************************************
	// Name:		Show
	// Description:	Mostrar.
	// Parameters:	CString szTitulo, CString szText,CPoint *pt = NULL.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	void Show(CString szText, CPoint *pt,CString szTitulo, int icono=0);
	//***********************************************************************
	// Name:		Hide
	// Description:	Esconder.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	void Hide();
	//***********************************************************************
	// Name:		SetShowDelay
	// Description:	Sets the delay for the tooltip.
	// Parameters:	int nDelay.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	void SetShowDelay(int nDelay) { m_nShowDelay = nDelay; };
	//***********************************************************************
	// Name:		RelayEvent
	// Description:	.
	// Parameters:	LPMSG lpMsg.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	void RelayEvent(LPMSG lpMsg);

protected:
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();

	DECLARE_MESSAGE_MAP()
};

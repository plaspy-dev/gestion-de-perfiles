#pragma once

#define CSS_FADEIN		0x0001
#define CSS_FADEOUT		0x0002
#define CSS_FADE		CSS_FADEIN | CSS_FADEOUT
#define CSS_SHADOW		0x0004
#define CSS_CENTERSCREEN	0x0008
#define CSS_CENTERAPP		0x0010
#define CSS_HIDEONCLICK		0x0020

#define CSS_TEXT_NORMAL		0x0000
#define CSS_TEXT_BOLD		0x0001
#define CSS_TEXT_ITALIC		0x0002
#define CSS_TEXT_UNDERLINE	0x0004

typedef BOOL (WINAPI* FN_ANIMATE_WINDOW)(HWND,DWORD,DWORD);

//***************************************************************************
//
// AUTHOR:  JUHU
//
// DESCRIPTION: Mensaje Splash (Ayuda y bienvenida)
//
// ORIGINAL: SplashScreenEx.cpp : header file by John O'Byrne 01/10/2002
//
//***************************************************************************
class   __COMPONENTES_EXPORT__ CSplashScreenEx : public CWnd
{
	DECLARE_DYNAMIC(CSplashScreenEx)
	// Construction
public:
	//***********************************************************************
	// Name:		CSplashScreenEx
	// Description:	Constructor.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	CSplashScreenEx();
	//***********************************************************************
	// Name:		CSplashScreenEx
	// Description:	Destructor.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	virtual ~CSplashScreenEx();

// Attributes
public:
	int m_nxPos;
	int m_nyPos;

protected:
	FN_ANIMATE_WINDOW m_fnAnimateWindow;
	CWnd *m_pWndParent;
	CBitmap m_bitmap;
	HRGN m_hRegion;

	DWORD m_dwStyle;
	DWORD m_dwTimeout;

	int m_nBitmapWidth;
	int m_nBitmapHeight;

	// texto
	CFont m_myFont;
	CString m_strText;
	CRect m_rcText;
	UINT m_uTextFormat;
	COLORREF m_crTextColor;

	// texto 2
	CFont m_myFont2;
	CString m_strText2;
	CRect m_rcText2;
	UINT m_uTextFormat2;
	COLORREF m_crTextColor2;

// Operations
public:
	//***********************************************************************
	// Name:		Create
	// Description:	Crea un splash.
	// Parameters:	CWnd *pWndParent,LPCTSTR szText=NULL,DWORD dwTimeout=2000,DWORD dwStyle=CSS_FADE | CSS_CENTERSCREEN | CSS_SHADOW.
	// Return:		None.	
	// Notes:		CSS_FADEIN
	//				CSS_FADEOUT
	//				CSS_FADE		CSS_FADEIN | CSS_FADEOUT
	//				CSS_SHADOW
	//				CSS_CENTERSCREEN
	//				CSS_CENTERAPP		
	//				CSS_HIDEONCLICK		
	//				CSS_TEXT_NORMAL
	//				CSS_TEXT_BOLD
	//				CSS_TEXT_ITALIC
	//				CSS_TEXT_UNDERLINE
	//***********************************************************************
	BOOL Create(CWnd *pWndParent,LPCTSTR szText=NULL,DWORD dwTimeout=2000,DWORD dwStyle=CSS_FADE | CSS_CENTERSCREEN | CSS_SHADOW);
	//***********************************************************************
	// Name:		SetBitmap
	// Description:	Fija el fondo.
	// Parameters:	UINT nBitmapID,short red=-1,short green=-1,short blue=-1.
	// Return:		BOOL.	
	// Notes:		None.
	//***********************************************************************
	BOOL SetBitmap(UINT nBitmapID,short red=-1,short green=-1,short blue=-1);
	//***********************************************************************
	// Name:		SetBitmap
	// Description:	Fija el fondo.
	// Parameters:	LPCTSTR szFileName,short red=-1,short green=-1,short blue=-1.
	// Return:		BOOL.	
	// Notes:		None.
	//***********************************************************************
	BOOL SetBitmap(LPCTSTR szFileName,short red=-1,short green=-1,short blue=-1);
	//***********************************************************************
	// Name:		Show
	// Description:	Muestra el splash.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	void Show();
	//***********************************************************************
	// Name:		Hide
	// Description:	Esconde el splash.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	void Hide();
	//***********************************************************************
	// Name:		SetText
	// Description:	Fija el texto.
	// Parameters:	LPCTSTR szText.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	void SetText(LPCTSTR szText);
	//***********************************************************************
	// Name:		SetTextFont
	// Description:	Fija el tipo de fuente.
	// Parameters:	LPCTSTR szFont,int nSize,int nStyle.
	// Return:		None.	
	// Notes:		see SetTextDefaultFont.
	//***********************************************************************
	void SetTextFont(LPCTSTR szFont,int nSize,int nStyle);
	//***********************************************************************
	// Name:		SetTextDefaultFont
	// Description:	Reset a fuente por defecto.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		See SetTextFont2.
	//***********************************************************************
	void SetTextDefaultFont();
	//***********************************************************************
	// Name:		SetTextFont
	// Description:	Fija el texto.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		See SetTextFormat.
	//***********************************************************************
	void SetTextColor(COLORREF crTextColor);
	//***********************************************************************
	// Name:		SetTextRect
	// Description:	Recibe un CRect con texto dentro.
	// Parameters:	CRect& rcText.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	void SetTextRect(CRect& rcText);
	//***********************************************************************
	// Name:		SetTextFormat
	// Description:	Fija el formato del texto.
	// Parameters:	UINT uTextFormat.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	void SetTextFormat(UINT uTextFormat);
	//***********************************************************************
	// Name:		SetTextFont2
	// Description:	Segunta fuente.
	// Parameters:	LPCTSTR szFont,int nSize,int nStyle.
	// Return:		None.	
	// Notes:		see SetTextFont.
	//***********************************************************************
	void SetTextFont2(LPCTSTR szFont,int nSize,int nStyle);
	//***********************************************************************
	// Name:		SetTextDefaultFont2
	// Description:	Segunda fuente x defecto.
	// Parameters:	LPCTSTR szFont,int nSize,int nStyle.
	// Return:		None.	
	// Notes:		see SetTextFont.
	//***********************************************************************
	void SetTextDefaultFont2();
	//***********************************************************************
	// Name:		SetText2
	// Description:	Segundo texto.
	// Parameters:	LPCTSTR szFont,int nSize,int nStyle.
	// Return:		None.	
	// Notes:		see SetTextFont.
	//***********************************************************************
	void SetText2(LPCTSTR szText);
	//***********************************************************************
	// Name:		SetTextRect2
	// Description:	Segundo rectangulo con texto.
	// Parameters:	LPCTSTR szFont,int nSize,int nStyle.
	// Return:		None.	
	// Notes:		see SetTextFont.
	//***********************************************************************
	void SetTextRect2(CRect& rcText);
	//***********************************************************************
	// Name:		SetTextFormat2
	// Description:	Segunto texto con formato.
	// Parameters:	LPCTSTR szFont,int nSize,int nStyle.
	// Return:		None.	
	// Notes:		see SetTextFont.
	//***********************************************************************
	void SetTextFormat2(UINT uTextFormat);
	//***********************************************************************
	// Name:		SetTextColor2
	// Description:	Segundo color de texto.
	// Parameters:	LPCTSTR szFont,int nSize,int nStyle.
	// Return:		None.	
	// Notes:		see SetTextFont.
	//***********************************************************************
	void SetTextColor2(COLORREF crTextColor);


	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	LRESULT OnPrintClient(WPARAM wParam, LPARAM lParam);

protected:
	HRGN CreateRgnFromBitmap(HBITMAP hBmp, COLORREF color);
	void DrawWindow(CDC *pDC);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void PostNcDestroy();

DECLARE_MESSAGE_MAP()
};



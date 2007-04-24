//***************************************************************************
//
// AUTHOR:  JUHU
//
// DESCRIPTION: Panel LCD
//
// ORIGINAL: Copyright (C) 2002 by Nic Wilson  http://www.nicsoft.com.au
//
//***************************************************************************

#pragma once
class __COMPONENTES_EXPORT__ CMatrixStatic : public CStatic
{
	// Construction
public:
	//***********************************************************************
	// Name:		CMatrixStatic
	// Description:	Constructor.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	CMatrixStatic();
	//***********************************************************************
	// Name:		CMatrixStatic
	// Description:	Destructor.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	virtual ~CMatrixStatic();

// Attributes
public:
	enum {SMALL = 0, LARGE, TINY};
	enum {LEFT = 0, RIGHT, UP, DOWN};

private:

	bool GetCharBmpOffset(RECT *rc, char ch);
	char		m_cPadChar;
	CString		m_csStrText;
	bool		m_bModified, m_bImmediateUpdate, m_btimer, m_bAutoPad;	
	COLORREF	m_crOffColor,	m_crOnColor, m_crBackColor;
	UINT		m_iImagematrix, m_iTimer;
	int			m_icharwidth, m_icharheight, m_ixspacing, m_iyspacing;
	int			m_iMaxYChars, m_iMaxXChars, m_iDirection;


// Operations
public:
	//***********************************************************************
	// Name:		SetAutoPadding
	// Description:	Utilize un caracter entre caracteres, por defecto es un 
	//				espacio.
	// Parameters:	bool pad, char ch = ' '.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	void SetAutoPadding(bool pad, char ch = ' ');
	//***********************************************************************
	// Name:		GetText
	// Description:	Obtiene el texto que se esta mostrando.
	// Parameters:	LPTSTR string,  int size.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	int  GetText(LPTSTR string,  int size);
	//***********************************************************************
	// Name:		SetDisplayColors
	// Description:	Define los colores que se van ha usar( fondo, encendido
	//				y apagado.
	// Parameters:	COLORREF bk,  COLORREF on,  COLORREF off.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	void SetDisplayColors(COLORREF bk,  COLORREF on,  COLORREF off);
	//***********************************************************************
	// Name:		SetBkColor
	// Description:	Solo define el color de fondo.
	// Parameters:	COLORREF bk.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	void SetBkColor(COLORREF bk);
	//***********************************************************************
	// Name:		SetColor
	// Description:	Define los colores que se usan para indica encendido y 
	//				apagado.
	// Parameters:	COLORREF off,  COLORREF on.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	void SetColor(COLORREF off,  COLORREF on);
	//***********************************************************************
	// Name:		SetBitmapResource
	// Description:	Define el la imagen LCD de fondo (Azul, Roja, Verde).
	// Parameters:	UINT bitmap.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	void SetBitmapResource(UINT bitmap);
	//***********************************************************************
	// Name:		SetSize
	// Description:	Define el tamaño de el display.
	// Parameters:	int size = SMALL.
	// Return:		None.	
	// Notes:		SMALL = 0, LARGE, TINY.
	//***********************************************************************
	void SetSize(int size = SMALL);
	//***********************************************************************
	// Name:		SetText
	// Description:	Inserta el texto que se desea mostrar en el display.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	void SetText(LPCTSTR lpszText);
	//***********************************************************************
	// Name:		StopScroll
	// Description:	Detiene el scroll text (movimiento del texto).
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	void StopScroll();
	//***********************************************************************
	// Name:		DoScroll
	// Description:	Inicia el scroll text (movimiento del texto).
	// Parameters:	int speed, BOOL dir.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	void DoScroll(int speed, BOOL dir);
	//***********************************************************************
	// Name:		SetCustomCharSizes
	// Description:	Tamaño del caracter.
	// Parameters:	int width, int height, int xspace,  int yspace.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	void SetCustomCharSizes(int width, int height, int xspace,  int yspace);
	//***********************************************************************
	// Name:		AdjustClientYToSize
	// Description:	Ajusta las imagenes de fondo y texto en espacio que se
	//				le indique (coordenada Y).
	// Parameters:	int size.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	void AdjustClientYToSize(int size);
	//***********************************************************************
	// Name:		AdjustClientXToSize
	// Description:	Ajusta las imagenes de fondo y texto en espacio que se
	//				le indique (coordenada X).
	// Parameters:	int size.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	void AdjustClientXToSize(int size);
	//***********************************************************************
	// Name:		SetNumberOfLines
	// Description:	Define el numero maximo de lineas para el display.
	// Parameters:	int max = 0.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	void SetNumberOfLines(int max = 0);
	//***********************************************************************
	// Name:		SetXCharsPerLine
	// Description:	Define el numero maximo de caracteres por linea.
	// Parameters:	int max = 0.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	void SetXCharsPerLine(int max = 0);

protected:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT nIDEvent);

DECLARE_MESSAGE_MAP()
};


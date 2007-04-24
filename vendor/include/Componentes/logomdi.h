//***************************************************************************
//
// AUTHOR:  JUHU
//
// DESCRIPTION: Logotipo de el espacio de trabajo (imagen fondo, flash, texto)
//
// ORIGINAL: 
//
//***************************************************************************
# pragma once

# include "shockwaveflash1.h"

class  __COMPONENTES_EXPORT__  CLogoMdi : public CWnd
{
// Construction
public:
	//***********************************************************************
	// Name:		CLogoMdi
	// Description:	Default constructor.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
    CLogoMdi();
	//***********************************************************************
	// Name:		CLogoMdi
	// Description:	Destructor.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	virtual ~CLogoMdi();

// Attributes
protected:
	CString				m_szArchivo;
	int					m_nAncho;
	int					m_nAlto;
	CShockwaveflash1	*m_pFlash;
	HBITMAP	m_hBitmap;
	BITMAP		m_bmInfo;
    CSize	m_sizeClient;
	CFont	m_LogoFont;
	CString sLogoString;
	DWORD	m_dwBkColor;
	int		m_nLineas;

// Operaciones
public:
	//***********************************************************************
	// Name:		SetFlash
	// Description:	Carga el objeto Flash a partir de un archivo.
	// Parameters:	CString archivo, int ancho=0, int alto=0, DWORD bkColor=-1.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	void SetFlash(CString archivo, int ancho=0, int alto=0, DWORD bkColor=-1);
	//***********************************************************************
	// Name:		Pause
	// Description:	Pausar la animacion Flash
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	void Pause();
	//***********************************************************************
	// Name:		Start
	// Description:	Inicioar la animacion Flash
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	void Start();
	//***********************************************************************
	// Name:		BuildFlash
	// Description:	Inicia la pelicula
	// Parameters:	bool.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	void BuildFlash(bool load=false);

protected:
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnSize(UINT nType, int cx, int cy);

DECLARE_MESSAGE_MAP()
};

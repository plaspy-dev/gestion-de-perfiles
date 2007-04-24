//***************************************************************************
//
// AUTHOR:  JUHU
//
// DESCRIPTION: Este es un objeto CStatic que recibe imagenes JPG, GIF, 
// recursos, memoria.
//
// ORIGINAL: 
//
//***************************************************************************

#pragma once

class  __COMPONENTES_EXPORT__ CBitmapPicture : public CStatic
{
// Construction
public:
	//***********************************************************************
	// Name:		CBitmapPicture
	// Description:	Default constructor.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
    CBitmapPicture();
	//***********************************************************************
	// Name:		CBitmapPicture
	// Description:	Destructor.
	// Parameters:	None.
	// Return:		None.		
	// Notes:		None.
	//***********************************************************************
	virtual ~CBitmapPicture();
    CByteArray		m_pImage;

// Atributos
public:
	HBITMAP  m_hBitmap;

protected:
    bool		m_bHideInfo;
    BITMAP		m_bmInfo;

private:
    int     m_nResourceID;
    CString m_strResourceName;

// Operations
// Propias
public:
	//***********************************************************************
	// Name:		SetBitmap
	// Description:	Recibe un objeto imagen del tipo de visual.
	// Parameters:	HBITMAP
	// Return:		BOOL 
	// Notes:		None.
	//***********************************************************************
    BOOL SetBitmap(HBITMAP hBitmap);
	//***********************************************************************
	// Name:		SetBitmap
	// Description:	Identificador del recurso que apunta a la imagen.
	// Parameters:	UINT
	// Return:		BOOL 
	// Notes:		None.
	//***********************************************************************
	BOOL SetBitmap(UINT nIDResource);
	//***********************************************************************
	// Name:		SetBitmap
	// Description:	Ruta sobre el sistema de archivos donde se encuentra la imagen.
	// Parameters:	LPCTSTR
	// Return:		BOOL 
	// Notes:		Ejemplo: "C://Windows/..."
	//***********************************************************************
    BOOL SetBitmap(LPCTSTR lpszResourceName);
	//***********************************************************************
	// Name:		ReloadBitmap
	// Description:	Puntero dirigido a la ubicacion de la imagen en la memoria.
	// Parameters:	void
	// Return:		BOOL 
	// Notes:		none
	//***********************************************************************
    BOOL ReloadBitmap();
	//***********************************************************************
	// Name:		SetImage
	// Description:	Puntero dirigido a la ubicacion de la imagen en la memoria.
	// Parameters:	CByteArray
	// Return:		void 
	// Notes:		none
	//***********************************************************************
    void SetImage(CByteArray *array);
	//***********************************************************************
	// Name:		HideInfo
	// Description:	Inserta informacion adicional sobre la imagen.
	// Parameters:	bool
	// Return:		void 
	// Notes:		none
	//***********************************************************************
	void HideInfo(bool hide=true);

// Visual
protected:
    virtual void PreSubclassWindow();
    virtual void DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct );

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnSysColorChange();
DECLARE_MESSAGE_MAP()
};


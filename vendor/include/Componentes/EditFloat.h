//***************************************************************************
//
// AUTHOR:  JUHU
//
// DESCRIPTION: Deciende de CEditXP y contiene funciones especiales para 
//				tratamiento de num flotantes.
//
// ORIGINAL: 
//
//***************************************************************************
#pragma once
#include "editxp.h"
# include "XInfoTip.h"

class __COMPONENTES_EXPORT__ CEditFloat : public CEditXP
{
// Construction
public:
	//***********************************************************************
	// Name:		CEditFloat
	// Description:	Default constructor.
	// Parameters:	void.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	CEditFloat(void);
	//***********************************************************************
	// Name:		CEditFloat
	// Description:	Destructor.
	// Parameters:	void.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	~CEditFloat(void);

// Attributes
protected:
	bool			m_bEnableMinus;
	double			m_dNumero;
	char			m_szMiles[4];
	char			m_szDecimal[4];
	void			SetNumeroEx(const CString &numero);
	void			SetNumeroEx();
	bool			m_bFocus;

// Operaciones
public:
	//***********************************************************************
	// Name:		TrimDecimales
	// Description:	Le quita los decimales a la configuracion de Win.
	// Parameters:	double.
	// Return:		CString.	
	// Notes:		None.
	//***********************************************************************
	static CString	TrimDecimales(double numero);
	//***********************************************************************
	// Name:		CleanZero
	// Description:	Quita ceros a la derecha que no son significativos.
	// Parameters:	CString.
	// Return:		CString.	
	// Notes:		None.
	//***********************************************************************
	static CString	CleanZero(const CString &cadena, bool force=false);
	//***********************************************************************
	// Name:		GetLocale
	// Description:	Obtiene los signos de miles y decimal dependiendo de la 
	//				conf de Wind.
	// Parameters:	char *decimal, char *miles.
	// Return:		void.	
	// Notes:		None.
	//***********************************************************************
	static void		GetLocale(char *decimal, char *miles);
	//***********************************************************************
	// Name:		GetNormal
	// Description:	Obtiene la configuracion de pocicion . , que entieden +
	//				ciertas funciones y programas.
	// Parameters:	void.
	// Return:		None.	
	// Notes:		Por ejemplo mysql y atoi crearian problemas con 1.032,212 
	//				esta funcion soluciona esto convirtiendo en 1032.212, see
	//				GetFormatNumber para proceso inverso.
	//***********************************************************************
	static CString	GetNormal(const CString &numero);
	//***********************************************************************
	// Name:		GetFormatNumber
	// Description:	Devuelve el numero con formato.
	// Parameters:	CString.
	// Return:		CString.	
	// Notes:		Inversa a GetNormal.
	//***********************************************************************
	static CString  GetFormatCurrency(const CString &numero);
	static CString  GetFormatNumber(const CString &numero);
	//***********************************************************************
	// Name:		GetFormatNumber
	// Description:	Convierte un double en el formato utilizado por el usuario.
	// Parameters:	double.
	// Return:		CString.	
	// Notes:		None.
	//***********************************************************************
	static CString  GetFormatCurrency(double numero);
	static CString  GetFormatNumber(double numero);
	//***********************************************************************
	// Name:		SetNumero
	// Description:	Permite ingresar un numero dentro del Edit.
	// Parameters:	CString.
	// Return:		void.	
	// Notes:		None.
	//***********************************************************************
	void SetNumero(const CString &numero);
	//***********************************************************************
	// Name:		SetNumero
	// Description:	Permite ingresar un numero dentro del Edit.
	// Parameters:	double.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	void SetNumero(double numero);
	//***********************************************************************
	// Name:		SetFormatNumero
	// Description:	Coloca el formato dentro del edit.
	// Parameters:	CString.
	// Return:		void.	
	// Notes:		None.
	//***********************************************************************
	void SetFormatNumero(const CString &numero);
	//***********************************************************************
	// Name:		GetNumero
	// Description:	Obtiene el numero que contiene el edit.
	// Parameters:	void.
	// Return:		CString.	
	// Notes:		None.
	//***********************************************************************
	CString GetNumero();
	//***********************************************************************
	// Name:		GetCleanNumero
	// Description:	Obtiene el numero sin formato y sin ceros.
	// Parameters:	void.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	CString GetCleanNumero();

	void EnableMinus(bool enable=true);

public:
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	void SetWindowText(LPCTSTR lpszString);

	__declspec(property(get=GetText,put=SetText))		CString	Text;
	__declspec(property(get=GetNumero,put=SetNumero))		CString	Number;
	void	SetText(const CString &texto);
DECLARE_MESSAGE_MAP()
};

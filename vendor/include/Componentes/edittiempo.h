//***************************************************************************
//
// AUTHOR:  JUHU
//
// DESCRIPTION: Permite manejar fecha de varios formatos a varios formatos.
//
// ORIGINAL: 
//
//***************************************************************************
#pragma once


// CEditTiempo
# define	POS_DIA		1
# define	POS_MES		2
# define	POS_ANO		3
# define	POS_HORA	4
# define	POS_MIN		5
#include "EditXP.h"

class  __COMPONENTES_EXPORT__ CEditTiempo : public CEditXP
{
	DECLARE_DYNAMIC(CEditTiempo)

// Construction
public:
	//***********************************************************************
	// Name:		CEditTiempo
	// Description:	Default constructor.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	CEditTiempo();
	//***********************************************************************
	// Name:		CEditTiempo
	// Description:	Destructor.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	virtual ~CEditTiempo();

// Attributes
protected:
	COleDateTime *m_DTMaximo;
	COleDateTime *m_DTMinimo;

	void	ValidateFecha();
	bool	m_bLocked;
	bool	m_bHora;
	int		m_nCurrentChar;
	int		m_nMaxChar;
	CString m_szBase;

// Operaciones
public:
	//***********************************************************************
	// Name:		SetDateTime
	// Description:	Inserta la fecha en el edit partiendo de un obj COleDateTime
	// Parameters:	COleDateTime *tiempo=NULL
	// Return:		void
	// Notes:		Fecha y hora
	//***********************************************************************
	void SetDateTime(COleDateTime *tiempo=NULL);
	//***********************************************************************
	// Name:		SetDateTime
	// Description:	Inserta la fecha en el edit partiendo de un obj CTime
	// Parameters:	CTime *tiempo
	// Return:		void
	// Notes:		Fecha y hora
	//***********************************************************************
	void SetDateTime(CTime *tiempo);
	//***********************************************************************
	// Name:		SetDate
	// Description:	Inserta la fecha en el edit partiendo de un obj COleDateTime
	// Parameters:	COleDateTime *tiempo=NULL
	// Return:		void
	// Notes:		Fecha unicamente
	//***********************************************************************
	void SetDate(COleDateTime *tiempo=NULL);
	//***********************************************************************
	// Name:		SetDate
	// Description:	Inserta la fecha en el edit partiendo de un obj CTime
	// Parameters:	CTime *tiempo
	// Return:		void
	// Notes:		Fecha unicamente
	//***********************************************************************
	void SetDate(CTime *tiempo);
	//***********************************************************************
	// Name:		SetTimeHoraMysql
	// Description:	Permite insertar una fecha, hora directamente de mysql
	// Parameters:	CString fecha, bool hora= false, bool lock=true
	// Return:		void
	// Notes:		
	//***********************************************************************
	void SetTimeHoraMysql( CString fecha, bool hora= false, bool lock=true);
	//***********************************************************************
	// Name:		GetFormatoMySQL
	// Description:	Obtiene el formato de mysql
	// Parameters:	void
	// Return:		CString
	// Notes:		
	//***********************************************************************
	CString GetFormatoMySQL();
	//***********************************************************************
	// Name:		SetMinimo
	// Description:	Fecha minima que permite el edit
	// Parameters:	COleDateTime *tiempo=NULL
	// Return:		void
	// Notes:		
	//***********************************************************************
	void  SetMinimo(COleDateTime *tiempo=NULL);
	//***********************************************************************
	// Name:		SetMinimo
	// Description:	Fecha minima que permite el edit
	// Parameters:	CTime *tiempo
	// Return:		void
	// Notes:		
	//***********************************************************************
	void  SetMinimo(CTime *tiempo);
	//***********************************************************************
	// Name:		SetMaximo
	// Description:	Fecha maxima que permite el edit
	// Parameters:	COleDateTime *tiempo=NULL
	// Return:		void
	// Notes:		
	//***********************************************************************
	void  SetMaximo(COleDateTime *tiempo=NULL);
	//***********************************************************************
	// Name:		SetMaximo
	// Description:	Fecha maxima que permite el edit
	// Parameters:	CTime *tiempo
	// Return:		void
	// Notes:		
	//***********************************************************************
	void  SetMaximo(CTime *tiempo);
	//***********************************************************************
	// Name:		GetDateTime
	// Description:	Obtiene la fecha que contiene el edit
	// Parameters:	COleDateTime &tiempo
	// Return:		bool
	// Notes:		
	//***********************************************************************
	bool GetDateTime(COleDateTime &tiempo);
	//***********************************************************************
	// Name:		GetDateTime
	// Description:	Obtiene la fecha que contiene el edit
	// Parameters:	CTime &tiempo
	// Return:		bool
	// Notes:		
	//***********************************************************************
	bool GetDateTime(CTime &tiempo);
	//***********************************************************************
	// Name:		GetDateTime
	// Description:	Devuelve true si tiene una fecha valida.
	// Parameters:	void
	// Return:		bool
	// Notes:		
	//***********************************************************************
	bool GetDateTime();
//***********************************************************************
	// Name:		GetMaximo
	// Description:	Obtiene la fecha maxima permitida
	// Parameters:	COleDateTime &tiempo	
	// Return:		bool
	// Notes:		
	//***********************************************************************
	bool GetMaximo(COleDateTime &tiempo);
	//***********************************************************************
	// Name:		GetMaximo
	// Description:	Obtiene la fecha maxima permitida
	// Parameters:	CTime &tiempo	
	// Return:		bool
	// Notes:		
	//***********************************************************************
	bool GetMaximo(CTime &tiempo);
	//***********************************************************************
	// Name:		GetMinimo
	// Description:	Obtiene la fecha minima permitida
	// Parameters:	COleDateTime &tiempo	
	// Return:		bool
	// Notes:		
	//***********************************************************************
	bool GetMinimo(COleDateTime &tiempo);
	//***********************************************************************
	// Name:		GetMinimo
	// Description:	Obtiene la fecha minima permitida
	// Parameters:	CTime &tiempo	
	// Return:		bool
	// Notes:		
	//***********************************************************************
	bool GetMinimo(CTime &tiempo);	

protected:
	void	ValidPos(bool forward=true);

public:
	afx_msg void OnEnSetfocus();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnEnChange();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg LRESULT OnCut(WPARAM Wparam, LPARAM LParam);
DECLARE_MESSAGE_MAP()
};

__COMPONENTES_EXPORT__ int GetMaxDay( int mes, int ano);


#pragma once
#include "atltime.h"
//***************************************************************************
//
// AUTHOR:  JUHU
//
// DESCRIPTION: Clase para el manejo de datos tipo date o time.
//
// ORIGINAL: 
//
//***************************************************************************
class __LAPPC_EXPORT__ CMyTime : public COleDateTime
{
	// Construction
public:
	CMyTime(COleDateTime tiempo);
	CMyTime(CTime tiempo);
	//***********************************************************************
	// Name:		CMyTime
	// Description:	Constructor.
	// Parameters:	bool currenttime=false.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	CMyTime(bool currenttime=false);
	//***********************************************************************
	// Name:		CMyTime
	// Description:	Constructor.
	// Parameters:	CString tiempo.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	CMyTime(const CString &tiempo);
	//***********************************************************************
	// Name:		CMyTime
	// Description:	Destructor.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	~CMyTime(void);
// Operations
public:
	//***********************************************************************
	// Name:		GetFechaHora
	// Description:	Obtiene la fecha y hora.
	// Parameters:	bool larga= false.
	// Return:		CString.	
	// Notes:		None.
	//***********************************************************************
	CString		GetFechaHora(bool larga= false) const;
	//***********************************************************************
	// Name:		GetFecha
	// Description:	Obtiene solo la fecha.
	// Parameters:	bool larga=false.
	// Return:		CString.	
	// Notes:		None.
	//***********************************************************************
	CString		GetFecha(bool larga=false) const;
	//***********************************************************************
	// Name:		GetHora
	// Description:	Obtiene unicamente la hora.
	// Parameters:	None.
	// Return:		CString.	
	// Notes:		None.
	//***********************************************************************
	CString		GetHora() const;
	//***********************************************************************
	// Name:		SetDateTime
	// Description:	Permite definir una fecha a partir de una cadena de texto
	//				para el objeto MyTime.
	// Parameters:	CString tiempo.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	void		SetStringDateTime(const CString &tiempo);
	//***********************************************************************
	// Name:		GetFormatoMysql
	// Description:	Obtiene la fecha en formato mysql.
	// Parameters:	bool hora=false.
	// Return:		CString.	
	// Notes:		None.
	//***********************************************************************
	CString		GetFormatoMysql(bool hora=false, bool segundos=false) const;
	//***********************************************************************
	// Name:		GetCustomFormat
	// Description:	Formato de la configuracion de Windows (see Excel).
	// Parameters:	CString formato.
	// Return:		CString.	
	// Notes:		None.
	//***********************************************************************
	CString		GetCustomFormat(CString formato) const;
	//***********************************************************************
	// Name:		EsValido
	// Description:	Es una fecha valida en un intervalo definido.
	// Parameters:	None.
	// Return:		bool.	
	// Notes:		None.
	//***********************************************************************
	bool		EsValido() const;

	// miembros staticos
	static CString	GetFechaLargaFormat();
	static CString	GetFechaCortaFormat();
	static CString	GetHoraFormat();

	// operadores 
	CMyTime &operator = (const CString &tiempo);
	CMyTime &operator = (const CMyTime &tiempo);
	CMyTime &operator = (const CTime &tiempo);
	CMyTime &operator = (const COleDateTime &tiempo);

	// otros
	operator CTime() const;
	operator COleDateTime() const;
};

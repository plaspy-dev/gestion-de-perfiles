#pragma once
//***************************************************************************
//
// AUTHOR:  JUHU
//
// DESCRIPTION: CConector conector para Mysql.
//
// ORIGINAL: 
//
//***************************************************************************
class __CONECTOR_EXPORT___ CQuery
{
	// Construction
public:
	//***********************************************************************
	// Name:		CQuery
	// Description:	Default constructor.
	// Parameters:	const CString &tabla.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	CQuery(const CString &tabla);
	//***********************************************************************
	// Name:		CQuery
	// Description:	Destructor.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	~CQuery(void);

// Attributes
protected:
	CString			m_szTabla;
	CStringArray	m_saCampos;
	CStringArray	m_saValores;
	CByteArray		m_baQuoted;
	CByteArray		m_baBinario;
	CString			m_szBinario;

// Operations
public:
	//***********************************************************************
	// Name:		SetTabla
	// Description:	Fije la tabla ha usar.
	// Parameters:	const CString &tabla.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	void	SetTabla(const CString &tabla);
	//***********************************************************************
	// Name:		GetTabla
	// Description:	Otiene el nombre de la tabla usada.
	// Parameters:	None.
	// Return:		CString.	
	// Notes:		None.
	//***********************************************************************
	CString GetTabla();
	//***********************************************************************
	// Name:		Clean
	// Description:	Resetear los valores (ejem: tabla).
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	void Clean();
	//***********************************************************************
	// Name:		AddCampo
	// Description:	Añadir un nuevo campo dentro de la sentencia.
	// Parameters:	const CString &campo, const CString &valor, bool quotes=true.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	void AddCampo(const CString &campo, const CString &valor, bool quotes=true);
	//***********************************************************************
	// Name:		GetText
	// Description:	Obtiene el valor del campo.
	// Parameters:	const CString &campo.
	// Return:		CString.	
	// Notes:		valores.
	//***********************************************************************
	CString GetText(const CString &campo);
	//***********************************************************************
	// Name:		SetText
	// Description:	Fija el nombre y valor del campo.
	// Parameters:	const CString &campo, const CString &valor.
	// Return:		None.	
	// Notes:		valores.
	//***********************************************************************
	void SetText(const CString &campo, const CString &valor);
	//***********************************************************************
	// Name:		CleanQuote
	// Description:	Limpiar comillas.
	// Parameters:	const CString &campo.
	// Return:		None.	
	// Notes:		valores.
	//***********************************************************************
	void CleanQuote(const CString &campo);
	//***********************************************************************
	// Name:		GetUpdate
	// Description:	Devuelve la construccion del la consulta.
	// Parameters:	const CString condicion.
	// Return:		CString.	
	// Notes:		salidas.
	//***********************************************************************
	CString GetUpdate(const CString condicion);
	//***********************************************************************
	// Name:		GetInsert
	// Description:	Devuelve la construccion del la consulta.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		salidas.
	//***********************************************************************
	CString GetInsert();
	//***********************************************************************
	// Name:		GetSelect
	// Description:	Devuelve la construccion del la consulta.
	// Parameters:	const CString condicion="", const CString join="".
	// Return:		CString.	
	// Notes:		salidas.
	//***********************************************************************
	CString	GetSelect(const CString condicion="", const CString join="");
	void SetBinario(const CString &campo, const CByteArray &arreglo);


	void operator+=(const CString &campo);
	CString& operator[]( const CString &campo);
	static CString GetValor(const CString &valor);
	__declspec(property(get=GetText,put=SetText))	CString	Campos[];

// no usadas // definiciones por compatibilidad con las versiones anteriores
	static CString CreateUpdate(CString tabla, CStringArray &Campos, CStringArray &Valores, CString condicion);
	static CString CreateInsert(CString tabla, CStringArray &Valores);
	static CString CreateInsert(CString tabla, CStringArray &valores, CString condicionselect);
	static CString CreateInsert(CString tabla, CStringArray &campos, CStringArray &valores);
};


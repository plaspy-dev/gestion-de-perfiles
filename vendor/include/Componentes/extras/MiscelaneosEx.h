//***************************************************************************
//
// AUTHOR:  JUHU
//
// DESCRIPTION: Una clase para adjuntar informacion a diversos controles (lista)
//
// ORIGINAL: 
//
//***************************************************************************
#pragma once

class __COMPONENTES_EXPORT__ CMiscelaneosEx
{
// Construction
public:
	//***********************************************************************
	// Name:		CMiscelaneosEx
	// Description:	Constructor.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	CMiscelaneosEx(void);
	//***********************************************************************
	// Name:		CMiscelaneosEx
	// Description:	Constructor.
	// Parameters:	const CString &nombre, DWORD data, CString szExtra="".
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	CMiscelaneosEx(const CString &nombre, DWORD data, CString szExtra="");
	//***********************************************************************
	// Name:		CMiscelaneosEx
	// Description:	Destructor.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	~CMiscelaneosEx(void);

// Attributes
protected:
	CString			m_szNombre;
	CString			m_szExtra;
	DWORD			m_dwData;
	CMiscelaneosEx	*m_pSig;

// Operations
public:
	//***********************************************************************
	// Name:		DeleteLast		
	// Description:	Borre la ultima entrada de la lista.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		
	//***********************************************************************
	void	DeleteLast();
	//***********************************************************************
	// Name:		GetNombre
	// Description:	Obtenga el atribulto nombre.
	// Parameters:	None.
	// Return:		CString.	
	// Notes:		
	//***********************************************************************
	CString GetNombre() const;
	//***********************************************************************
	// Name:		GetData
	// Description:	Obtenga la data en un paketico.
	// Parameters:	None.
	// Return:		DWORD.	
	// Notes:		
	//***********************************************************************
	DWORD	GetData() const;
	//***********************************************************************
	// Name:		GetExtra		
	// Description:	Obtiene el contenido del campo extra.
	// Parameters:	None.
	// Return:		CString.	
	// Notes:		
	//***********************************************************************
	CString GetExtra() const;
	//***********************************************************************
	// Name:		SetData		
	// Description:	Ingresar el pakete data.
	// Parameters:	DWORD data.
	// Return:		None.	
	// Notes:		
	//***********************************************************************
	void	SetData(DWORD data);
	//***********************************************************************
	// Name:		SetExtra		
	// Description:	Ingresa el campo extra.
	// Parameters:	CString extra.
	// Return:		None.	
	// Notes:		
	//***********************************************************************
	void	SetExtra(CString extra);
	//***********************************************************************
	// Name:		SetNombre		
	// Description:	Ingresa el campo nombre.
	// Parameters:	CString nombre.
	// Return:		None.	
	// Notes:		
	//***********************************************************************
	void	SetNombre(CString nombre);
	//***********************************************************************
	// Name:		DeleteAll		
	// Description:	Borra todos los registros.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		
	//***********************************************************************
	void	DeleteAll();
	//***********************************************************************
	// Name:		RemoveAt		
	// Description:	Remueve el contenido de "objeto" ubicado en la pos....
	// Parameters:	int index.
	// Return:		None.	
	// Notes:		
	//***********************************************************************
	void	RemoveAt(int index);
	//***********************************************************************
	// Name:		GetCount
	// Description:	Obtiene el total de registros existentes.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		
	//***********************************************************************
	int		GetCount() const;

	//Operadores
	CMiscelaneosEx &operator = (CMiscelaneosEx &item);
	CMiscelaneosEx *operator[]( int nIndex ) const;
	CMiscelaneosEx *Add(const CString &nombre, DWORD data, CString szExtra="");
	CMiscelaneosEx *GetAt(int index) const;
	CMiscelaneosEx *GetNext() const;
	CMiscelaneosEx *FindNext(DWORD data) const;
	CMiscelaneosEx *Find(CString nombre) const;
	CMiscelaneosEx *Find(DWORD data) const;
	CMiscelaneosEx *FindNoCase(CString nombre) const;
	CMiscelaneosEx *FindExtraNoCase(CString nombre) const;
};

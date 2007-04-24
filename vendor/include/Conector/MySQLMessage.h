#pragma once

typedef class CMySQLMessage MySQLMessage;
typedef class CMySQLMessage MySQLMessageB;
//***************************************************************************
//
// AUTHOR:  JUHU
//
// DESCRIPTION: CConector conector para Mysql.
//
// ORIGINAL: 
//
//***************************************************************************
class __CONECTOR_EXPORT___ CMySQLMessage
{
// Construction
public:
	//***********************************************************************
	// Name:		CMySQLMessage
	// Description:	Default constructor.
	// Parameters:	LPARAM lParam.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	CMySQLMessage(LPARAM lParam);
	//***********************************************************************
	// Name:		CMySQLMessage
	// Description:	Default constructor.
	// Parameters:	DWORD owner=0.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	CMySQLMessage(DWORD id=0, CWnd *parent=NULL);
	//***********************************************************************
	// Name:		CMySQLMessage
	// Description:	Destructor.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************	
	~CMySQLMessage(void);

// Attributes
protected:
	int				m_nID;
	CStringArray	m_pMessage;
	CString			m_szVacio;
	CStringArray	m_saCampos;
	CByteArray		m_pMessageB;
	int				m_nCount;
	CWnd			*m_pParent;
	// temporalmente publicas para compatibilidad anterior

// Operations
public:
	//***********************************************************************
	// Name:		GetID
	// Description:	Id del componente que lo usa.
	// Parameters:	None 
	// Return:		DWORD.	
	// Notes:		None.
	//***********************************************************************
	DWORD GetID() const;
	//***********************************************************************
	// Name:		GetCampo
	// Description:	Indice del campo.
	// Parameters:	DWORD.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	int		GetCampo(const CString &campo);
	//***********************************************************************
	// Name:		*GetBinaryData
	// Description:	Obtiene el resultado de una consulta a un dato binario.
	// Parameters:	CByteArray.
	// Return:		None.	
	// Notes:		Note que solo se permite un dato binario por consulta.
	//***********************************************************************
	CByteArray *GetBinaryData();
	//***********************************************************************
	// Name:		GetAt
	// Description:	Obtiene un dato a partir del id.
	// Parameters:	int index.
	// Return:		CString.	
	// Notes:		.
	//***********************************************************************
	CString	GetAt(int index);
	//***********************************************************************
	// Name:		GetAt
	// Description:	Obtiene un dato a partir del id.
	// Parameters:	const CString &campo.
	// Return:		CString. 	
	// Notes:		.
	//***********************************************************************
	CString GetAt(const CString &campo);
	//***********************************************************************
	// Name:		Clean
	// Description:	Limpia el contenido del objeto.
	// Parameters:	CString.
	// Return:		CString. 	
	// Notes:		.
	//***********************************************************************
	void Clean();
	//***********************************************************************
	// Name:		AddColumn
	// Description:	Agregar columna.
	// Parameters:	const CString &columna.
	// Return:		int. 	
	// Notes:		operaciones de columna.
	//***********************************************************************
	int AddColumn(const CString &columna);
	//***********************************************************************
	// Name:		GetColumna
	// Description:	Obtener coluna.
	// Parameters:	int index.
	// Return:		CString. 	
	// Notes:		operaciones de columna.
	//***********************************************************************
	CString GetColumna(int index);
	//***********************************************************************
	// Name:		GetColumnCount
	// Description:	O.
	// Parameters:	None.
	// Return:		int. 	
	// Notes:		operaciones de columna.
	//***********************************************************************
	int GetColumnCount();
	//***********************************************************************
	// Name:		GetCount
	// Description:	Obtiene el numero de registros.
	// Parameters:	CString.
	// Return:		int. 	
	// Notes:		operaciones de columna.
	//***********************************************************************
	int GetCount() const;
	//***********************************************************************
	// Name:		SetCount
	// Description:	.
	// Parameters:	int count.
	// Return:		None. 	
	// Notes:		For friends.
	//***********************************************************************
	void SetCount(int count);
	//***********************************************************************
	// Name:		Add
	// Description:	Agrega una cadena al mensaje
	// Parameters:	
	//		campo: cadena a agregar.
	// Return:		Ninguna.
	// Notes:		For friends.
	//***********************************************************************
	void Add(const CString &campo);
	//***********************************************************************
	// Name:		GetMensaje
	// Description:	Agrega una cadena al mensaje
	// Parameters:	Ninguno
	// Return:		Arreglo de cadenas con el mensaje.
	// Notes:		For friends.
	//***********************************************************************
	CStringArray *GetMensaje();
	CWnd *GetParent();
	void SetParent(CWnd *parent);

	CMySQLMessage &operator=(CMySQLMessage &mensaje);
	CString& operator[]( const CString &campo);
	CString& operator[]( const int index);
};

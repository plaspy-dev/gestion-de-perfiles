#pragma once

# include "ArrayDB.h"
# include "threadmysql.h"

//***************************************************************************
//
// AUTHOR:  JUHU
//
// DESCRIPTION: CConector conector para Mysql.
//
// ORIGINAL: 
//
//***************************************************************************
class __CONECTOR_EXPORT___ CConector {
	// Construction
public:
	//***********************************************************************
	// Name:		CConector
	// Description:	Default constructor.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	CConector();
	//***********************************************************************
	// Name:		CConector
	// Description:	Destructor.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	~CConector();

// Attributes
public:
	bool									m_bVerbose;

private:
	CMutex									m_pMutex;
	CArrayDB								m_pArrayDB;
	CArrayDB								m_pArrayDBModal;
	CStringArray							m_pQuerys;
	CPtrArray								m_pArrayCWnd;
	CUIntArray								m_pIDs;
	CWnd									*pMainFrame;
	CThreadMySQL							*m_pThread;
	CThreadMySQL							*m_pThreadOld;
	CByteArray								m_pBinary;
	bool									m_bRunning;
	bool									m_bModal;
	int										m_nReintentos;
	int										m_nLastState;
	CUIntArray								m_nDatabase;

// Operations
public:
	//***********************************************************************
	// Name:		CopyState
	// Description:	Copia el estado.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	void		CopyState();
	//***********************************************************************
	// Name:		ResetState
	// Description:	Resetea el estado (eje: cuando intenta conectarse).
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	void		ResetState();
	//***********************************************************************
	// Name:		SetMainFrame
	// Description:	A quien debe enviarse el mensaje.
	// Parameters:	CWnd *parent.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	void		SetMainFrame( CWnd *parent);
	//***********************************************************************
	// Name:		GetUser
	// Description:	Obtiene el usuario con quien se esta conectando.
	// Parameters:	None.
	// Return:		CString.	
	// Notes:		None.
	//***********************************************************************
	CString		GetUser();
	//***********************************************************************
	// Name:		SetUser
	// Description:	Fija el usuario con quien se esta conectando.
	// Parameters:	CString user.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	void		SetUser(CString user);
	//***********************************************************************
	// Name:		SetUser
	// Description:	Fija el password de usuario con quien se esta conectando.
	// Parameters:	CString passwd.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	void		SetPasswd(CString passwd);
	//***********************************************************************
	// Name:		CloseConections
	// Description:	Cierra las conexiones actuales.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	void		CloseConections();
	//***********************************************************************
	// Name:		KillQuery
	// Description:	Elimina alguna consulta realizandose en la BD.
	// Parameters:	DWORD id, CWnd *parent.
	// Return:		bool.	
	// Notes:		None.
	//***********************************************************************
	bool		KillQuery(DWORD id, CWnd *parent);
	//***********************************************************************
	// Name:		GetNombreDatabase
	// Description:	Es el nombre de la base de datos actual.
	// Parameters:	None.
	// Return:		CString.	
	// Notes:		None.
	//***********************************************************************
	CString		GetNombreDatabase();
	//***********************************************************************
	// Name:		GetDatabasesCount
	// Description:	Es una sentencia directa a alguna tabla en Mysql.
	// Parameters:	None.
	// Return:		int.	
	// Notes:		None.
	//***********************************************************************
	int			GetDatabasesCount();
	//***********************************************************************
	// Name:		Query
	// Description:	Sentencia, id de quien envio el mensaje, padre, una sola
	//				base de datos.
	// Parameters:	CString query, int id, CWnd *parent, int usedatabase=0.
	// Return:		int.	
	// Notes:		see QueryBinary for binarios.
	//***********************************************************************
	int			Query( CString query, int id=0, CWnd *parent=NULL, int usedatabase=0);
	//***********************************************************************
	// Name:		QueryBinary
	// Description:	Comun Query, el primer dato es binario.
	// Parameters:	CString query, int id, CWnd *parent, int usedatabase=0.
	// Return:		int.	
	// Notes:		see Query.
	//***********************************************************************
	int			QueryBinary( CString query, int id=0, CWnd *parent=NULL, int usedatabase=0);
	//***********************************************************************
	// Name:		QueryBinary
	// Description:	Comun Query, indepente de la BD.
	// Parameters:	CString query, int id, CWnd *parent.
	// Return:		int.	
	// Notes:		see Query.
	//***********************************************************************
	int			QueryEx( CString query, int id=0, CWnd *parent=NULL);
	//***********************************************************************
	// Name:		Find
	// Description:	Comun Query, no modal.
	// Parameters:	CString query, int id, CWnd *parent.
	// Return:		ULONG.	//Total encontrados
	// Notes:		see Query.
	//***********************************************************************
	ULONG		Find( CString query, CWnd *parent=NULL, CStringArray *valores=NULL, CStringArray *columnas=NULL);
	//***********************************************************************
	// Name:		AddDataBase
	// Description:	Añadir una nueva conf de BD al sistema.
	// Parameters:	CString nombreDB, CString user, CString passwd, CString host, int port= MYSQL_PORT.
	// Return:		bool.
	// Notes:		see Query.
	//***********************************************************************
	bool		AddDataBase(CString nombreDB, CString user, CString passwd, CString host, int port= MYSQL_PORT, bool incrustado=false);
	//***********************************************************************
	// Name:		Guardar
	// Description:	Guardar una nueva conf de BD al sistema.
	// Parameters:	CString nombreDB, CString user, CString passwd, CString host, int port= MYSQL_PORT.
	// Return:		bool.
	// Notes:		see AddDataBase.
	//***********************************************************************
	bool		Guardar(HKEY hKey= HKEY_CURRENT_USER);
	//***********************************************************************
	// Name:		Cargar
	// Description:	Cargar una nueva conf de BD al sistema.
	// Parameters:	CString nombreDB, CString user, CString passwd, CString host, int port= MYSQL_PORT.
	// Return:		bool.
	// Notes:		see AddDataBase.
	//***********************************************************************
	bool		Cargar(HKEY hKey= HKEY_CURRENT_USER);
	//***********************************************************************
	// Name:		Ping
	// Description:	Ping a las bases de datos.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		See TestConection.
	//***********************************************************************
	bool		Ping();
	//***********************************************************************
	// Name:		TestConection
	// Description:	Añadir una nueva conf de BD al sistema.
	// Parameters:	CWnd *parent.
	// Return:		bool.
	// Notes:		see Ping.
	//***********************************************************************
	bool		TestConection(CWnd *parent);
	bool		EsIncrustado() const;
	void		OnDestroy();

	
	CArrayDB	*GetDatabaseAt(int index);
	CArrayDB	*GetDatabaseModalAt(int index);
	CConector &operator=(CConector &conector);
	bool		RealDeleteThread();

private:
	void									RealQuery();
};
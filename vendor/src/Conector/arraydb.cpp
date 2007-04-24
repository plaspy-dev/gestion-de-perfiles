#include "StdAfx.h"
# include "Conector.h"
#include "arraydb.h"
# include "ThreadMySQL.h"
# include "include/errmsg.h"


CArrayDB::CArrayDB()
{
	m_nPort= MYSQL_PORT;
	m_pMySQL= NULL;
	m_pSig= NULL;
	m_pParent= NULL;
	m_nReintentos=0;
	m_nEstado=0;
	m_bTryConect=false;
	m_pMainFrame= NULL;
	m_bInscrutado=false;
}

CArrayDB::~CArrayDB()
{
	RemoveAll();
}

void CArrayDB::SetHost(CString host) {

	m_szHost= host;
}

CString	CArrayDB::GetHost() {

	return m_szHost;
}

void CArrayDB::SetDB(CString db) {
	
	m_szNombreDB= db;
}

void CArrayDB::SetUser(CString usuario) {
	
	m_szUser= usuario;
}
void CArrayDB::SetPassword(CString passwd) {
	m_szPasswd= passwd;
}
void CArrayDB::SetPort(int port) {
	m_nPort= port;
}

CString CArrayDB::GetDB() {
	return m_szNombreDB;
}
CString CArrayDB::GetUser() {
	return m_szUser;
}

CString CArrayDB::GetPassword() {
	return m_szPasswd;
}

int	CArrayDB::GetPort() {
	return m_nPort;
}

bool CArrayDB::EsIncrustado() const  {

	return m_bInscrutado;
}

void CArrayDB::SetIncrustado(bool incrustado/*=false*/) {

	m_bInscrutado=incrustado;
}

void CArrayDB::SetMainFrame(CWnd* mainframe) {
	
	m_pMainFrame= mainframe;
	if(m_pSig) 
		m_pSig->SetMainFrame(mainframe);
}

void CArrayDB::SetEstado(int estado) {

	m_nEstado=0;
	if( m_pSig) 
		m_pSig->SetEstado(estado);
}

int	CArrayDB::GetEstado() {

	if( m_pSig)
		return m_pSig->GetEstado()+m_nEstado;
	else return m_nEstado;
}

void CArrayDB::SetRetry(unsigned int reintentos) {

	m_nReintentos= reintentos;
	if( m_pSig)
		m_pSig->SetRetry(reintentos);
}

void CArrayDB::ResetState() {

	m_bTryConect=false;
	if( m_pSig)
		m_pSig->ResetState();
}

void CArrayDB::SetPadre(CWnd* parent) {
	m_pParent= parent;
	if( m_pSig)
		m_pSig->SetPadre(parent);
}

bool CArrayDB::CanConnect() {

	return !(m_pMySQL==NULL && m_bTryConect);
}

void CArrayDB::SetCanConnect(bool intentar) {

	m_bTryConect=!intentar;
}

/* = operator definition */
CArrayDB& CArrayDB::operator=(const CArrayDB &database)
{
	if (&database != this)
	{
		m_nPort= database.m_nPort;
		m_pParent= database.m_pParent;
		m_szHost= database.m_szHost;
		m_szNombreDB= database.m_szNombreDB;
		m_szPasswd= database.m_szPasswd;
		m_szUser= database.m_szUser;
		m_pMainFrame= database.m_pMainFrame;
		m_bInscrutado= database.m_bInscrutado;
		m_bTryConect= false;
	}
	return *this;
}

bool CArrayDB::Ping() {

	ASSERT(IsLoaded());
	if(!IsLoaded())
		return false;
	FUNCTION_STACK_TRACE("FFFFC000");
	for( int i=0; i<GetCount(); i++) {
		if( GetAt(i)->m_pMySQL)  {
			if(mysql_ping( GetAt(i)->m_pMySQL)==0) {
				return true;
			}
		}
	}
	return false;
}


CArrayDB *CArrayDB::GetAt(int index) {

	FUNCTION_STACK_TRACE("FFFFC001");
	if( index==0)
		return this;
	if( m_pSig) 
		return m_pSig->GetAt(index-1);
	return NULL;
}

int	CArrayDB::GetCount() {

	FUNCTION_STACK_TRACE("FFFFC002");
	if( m_szHost.IsEmpty())
		return 0;
	if( m_pSig) return m_pSig->GetCount()+1;
	return 1;
}

void CArrayDB::Add( CArrayDB *info) {

	FUNCTION_STACK_TRACE("FFFFC003");
	if( m_szHost.IsEmpty()) {
		
		m_nPort= info->m_nPort;
		m_pParent= info->m_pParent;
		m_szHost= info->m_szHost;
		m_szNombreDB= info->m_szNombreDB;
		m_szPasswd= info->m_szPasswd;
		m_szUser= info->m_szUser;
		m_bInscrutado= info->m_bInscrutado;
		return;
	}
	if( !m_pSig) m_pSig= new CArrayDB();
	m_pSig->Add( info);
}

void CArrayDB::RemoveAll() {

	FUNCTION_STACK_TRACE("FFFFC004");
	m_szHost.Empty();
	m_bTryConect=false;
	if( m_pMySQL && IsLoaded())
		mysql_close( m_pMySQL);
	m_pMySQL= NULL;
	if(m_bInscrutado && IsLoaded()) 
		mysql_library_end();
	if(!m_bInscrutado)
		m_hDLL=NULL;
	SAFE_DELETE(m_pSig);
}

void CArrayDB::CloseConections() {
	
	if( m_pMySQL && IsLoaded())
		mysql_close( m_pMySQL);
	m_pMySQL= NULL;
	m_bTryConect= false;
	if(m_bInscrutado && IsLoaded()) {
		mysql_library_end();
	}
	if( m_pSig)
		m_pSig->CloseConections();
}

CString CArrayDB::GetLastError() {

	if(!IsLoaded()) {
		return m_bInscrutado ?
			"No se puede encontrar libmysqld.dll, por favor consulte a su administrador." : 
			"No se puede encontrar libmysql.dll, por favor consulte a su administrador.";
	}
	FUNCTION_STACK_TRACE("FFFFC005");
	if( m_pMySQL)
		return mysql_error(m_pMySQL);
	return "No se ha establecido una conexión";
}

bool CArrayDB::Conectar(int id, bool seleccionardb) {

	FUNCTION_STACK_TRACE("FFFFC006");

	CMySQLMessage	message((DWORD)id, m_pParent);

	if(!IsLoaded()) { // intentamos cargar el dll
		Init(m_bInscrutado ? "libmysqld.dll" : "libmysql.dll");
	}
	if(!IsLoaded()) {

		message.Add(GetLastError());
		message.Add("");
		if( m_pParent && ::IsWindow(m_pParent->GetSafeHwnd()))
			m_pParent->SendMessage( WM_MYSQL, QUERY_ERROR, (LPARAM) &message);
		if( m_pMainFrame!=m_pParent && m_pMainFrame && ::IsWindow(m_pMainFrame->GetSafeHwnd()))
			m_pMainFrame->SendMessage(WM_MYSQL, QUERY_ERROR, (LPARAM) &message);
		return false;
	}

	if( m_pMySQL) {
		return true;
	}
	if( m_bTryConect)
		return false;
	m_bTryConect= seleccionardb;
	bool m_bError= false;

	if(m_bInscrutado) {

		// opciones por defecto de mysql.
		CString configuracion;
		configuracion= "--defaults-file=" + CApplicacion::GetAppPath("mysql.ini");
		configuracion.Replace("\\", "/");
		char *server_groups[] = {"mysqld", "client" };
		const char **server_options= new const char* [3];
		server_options[0]= CApplicacion::GetAppName();
		server_options[1]= configuracion;

		// creamos el directorio donde se van a guardar los datos por defecto.
		TCHAR szPath[MAX_PATH];
		SHGetFolderPath(NULL, CSIDL_COMMON_APPDATA | CSIDL_FLAG_CREATE,NULL, 0, szPath);
		CString ruta= szPath;
		ruta+="\\";
		ruta+= CApplicacion::GetCompany() + "\\";
		CreateDirectory(ruta, NULL);
		ruta+= CApplicacion::GetAppName() + "\\";
		CreateDirectory(ruta, NULL);
		ruta+="data\\";
		CreateDirectory(ruta, NULL);

		// verificamos que no exista ningun directorio configurado
		CMyRegKey key(HKEY_LOCAL_MACHINE, CApplicacion::GetAppKey());
		ruta= key.GetString("control", "database", ruta);

		// reemplazamos los backslash por slash.
		ruta.Replace("\\", "/");
		ruta= "--datadir=" + ruta;
		server_options[2]= ruta;

		// establecemos la ruta del archivo.
		SetCurrentDirectory(CApplicacion::GetAppPath());
		if(mysql_library_init(3,(char **) server_options, server_groups)!=0) { // error
			
			message.Add("No se puede crear instancia de la base de datos.");
			message.Add("");
			if( m_pParent && ::IsWindow(m_pParent->GetSafeHwnd()))
				m_pParent->SendMessage(WM_MYSQL, QUERY_ERROR, (LPARAM) &message);
			if( m_pMainFrame!=m_pParent && m_pMainFrame && ::IsWindow(m_pMainFrame->GetSafeHwnd()))
				m_pMainFrame->SendMessage(WM_MYSQL, QUERY_ERROR, (LPARAM) &message);
			delete []server_options;
			return false;
		}
		delete []server_options;
	}

	if((m_pMySQL= mysql_init((MYSQL*) 0))==NULL) {
		message.Add(GetLastError());
		message.Add("");
		if( m_pParent && ::IsWindow(m_pParent->GetSafeHwnd()))
			m_pParent->SendMessage(WM_MYSQL, QUERY_ERROR, (LPARAM) &message);
		if( m_pMainFrame!=m_pParent && m_pMainFrame && ::IsWindow(m_pMainFrame->GetSafeHwnd()))
			m_pMainFrame->SendMessage(WM_MYSQL, QUERY_ERROR, (LPARAM) &message);
		return false;
	}

	if(m_bInscrutado) {
		mysql_options(m_pMySQL, MYSQL_READ_DEFAULT_GROUP, "libmysqld_client");
		mysql_options(m_pMySQL, MYSQL_OPT_USE_EMBEDDED_CONNECTION, NULL);
	}

	if ( mysql_real_connect( m_pMySQL, m_szHost, m_szUser, m_szPasswd, m_szNombreDB, m_nPort ,NULL, 0 ) )
    {
		return true;
	} else {
		if( (mysql_errno( m_pMySQL)==CR_SERVER_GONE_ERROR || mysql_errno( m_pMySQL)==CR_SERVER_LOST) && m_nReintentos) {
				m_nReintentos--;
				return Conectar(id, seleccionardb);
		}
		message.Add(GetLastError());
		message.Add("");
		if( m_pParent && ::IsWindow(m_pParent->GetSafeHwnd()))
			m_pParent->SendMessage(WM_MYSQL, QUERY_ERROR, (LPARAM) &message);
		if( m_pMainFrame!=m_pParent && m_pMainFrame && ::IsWindow(m_pMainFrame->GetSafeHwnd()))
			m_pMainFrame->SendMessage(WM_MYSQL, QUERY_ERROR, (LPARAM) &message);
		
		if(m_pMySQL)
			mysql_close(m_pMySQL);
		m_pMySQL= NULL;
		return false;
	}
	return true;
}

ULONG CArrayDB::Find( CString query, CStringArray *datos, CStringArray *columnas) {

	FUNCTION_STACK_TRACE("FFFFC007");
	MYSQL_RES	*res=NULL;
	MYSQL_ROW	row;

	ULONGLONG i=0;

	CMySQLMessage	message(0, m_pParent);

	if( datos) datos->RemoveAll();

	if( !m_pMySQL) 
		if( !Conectar(0)) 
			return -1;

	if( !m_pMySQL)
		return -1;

	ASSERT(IsLoaded());
	if(!IsLoaded())
		return -1;

	if( mysql_query(m_pMySQL, query)) {
		if( (mysql_errno( m_pMySQL)==CR_SERVER_GONE_ERROR || mysql_errno( m_pMySQL)==CR_SERVER_LOST) && m_nReintentos) {
				m_nReintentos--;
				CloseConections();
				return Find( query, datos, columnas);
		}
		message.Add( GetLastError());
		message.Add(query);
		if(datos)
			datos->Add(GetLastError());
		if( mysql_errno( m_pMySQL)==CR_CONN_HOST_ERROR) {
			if( m_pMySQL)
				mysql_close( m_pMySQL);
			m_pMySQL= NULL;
		}
		if( m_pParent && ::IsWindow(m_pParent->GetSafeHwnd()))
				m_pParent->SendMessage( WM_MYSQL, QUERY_ERROR, (LPARAM) &message);
		if(  m_pMainFrame!=m_pParent && m_pMainFrame && ::IsWindow(m_pMainFrame->GetSafeHwnd()))
			m_pMainFrame->SendMessage(WM_MYSQL, QUERY_ERROR, (LPARAM) &message);
		return -1;
	} else {

		if( query.Find("INSERT INTO ")!=-1) {
			long lastid= mysql_insert_id(m_pMySQL);
			if( lastid) {
				query.Format("%ld", (long) lastid);
				if( datos) {
					datos->Add(query);
					return datos->GetCount();
				}
			}
		}
		res = mysql_store_result( m_pMySQL);
		if( ! res )
			return 0;
		i = (ULONGLONG) mysql_num_rows(res);

		if( columnas) {
			MYSQL_FIELD	*fd;
			while( fd = mysql_fetch_field( res ))
				columnas->Add(fd->name);
		}

		int l = 0;
		if( datos) {
		
			while ( row = mysql_fetch_row( res ) ) {
	
				int j = mysql_num_fields( res ) ;
				l++;
				for ( int k = 0 ; k < j ; k++ ) 
				
					datos->Add((((row[k]==NULL)||(!strlen(row[k])))?"":row[k]));
			}
		}
		mysql_free_result( res ) ;
	}
	return i;
}

int	CArrayDB::QueryBinary(CString query, int id, CThreadAuto *hilo) {

	FUNCTION_STACK_TRACE("FFFFC008");
	ASSERT(hilo);
	
	MYSQL_ROW	row;

	ULONGLONG i=0;
	CMySQLMessage	message((DWORD)id, m_pParent);

	ASSERT(m_pParent);

	if(!hilo->IsRunning())
		return 0;
	if( !m_pMySQL) 
		if( !Conectar(id)) 
			return -1;
	if( !m_pMySQL)
		return -1;

	ASSERT(IsLoaded());
	if(!IsLoaded())
		return -1;

	if(!hilo->IsRunning())
		return 0;
	if( mysql_query(m_pMySQL, query)) {

		if(!hilo->IsRunning())
			return 0;
		if( (mysql_errno( m_pMySQL)==CR_SERVER_GONE_ERROR || mysql_errno( m_pMySQL)==CR_SERVER_LOST) && m_nReintentos) {
				m_nReintentos--;
				if(!hilo->IsRunning())
					return 0;
				CloseConections();
				if(!hilo->IsRunning())
					return 0;
				return QueryBinary(query, id, hilo);
		}
		message.Add(GetLastError());
		if( mysql_errno( m_pMySQL)==CR_CONN_HOST_ERROR) {
			if( m_pMySQL)
				mysql_close( m_pMySQL);
			m_pMySQL= NULL;
		}
		if(!hilo->IsRunning())
			return 0;
		if( m_pParent && ::IsWindow(m_pParent->GetSafeHwnd()))
			m_pParent->SendMessage(WM_MYSQL, QUERY_ERROR, (LPARAM) &message);
		if( m_pMainFrame!=m_pParent && m_pMainFrame  && ::IsWindow(m_pMainFrame->GetSafeHwnd()))
			m_pMainFrame->SendMessage(WM_MYSQL, QUERY_ERROR, (LPARAM) &message);
		return -1;
	} else {
	  
		if(!hilo->IsRunning())
			return 0;
		if( query.Find("INSERT INTO ")!=-1) {
			long lastid= mysql_insert_id(m_pMySQL);
			if( lastid) {
				CString temp;
				temp.Format("%d", lastid);
				message.Add(temp);
				if(!hilo->IsRunning())
					return 0;
				if( m_pParent && ::IsWindow(m_pParent->GetSafeHwnd()))
					return m_pParent->SendMessage(WM_MYSQL, QUERY_RESULT, (LPARAM) &message);
				return 0;
			}
		}

		if(!hilo->IsRunning())
			return 0;
		MYSQL_RES *mysqlres = mysql_store_result( m_pMySQL);
		if(!mysqlres) 
			return i;
		i = (ULONGLONG) mysql_num_rows(mysqlres);
		if(!hilo->IsRunning())
			return 0;
		message.SetCount(i);
		if( m_pParent && ::IsWindow(m_pParent->GetSafeHwnd())) 
			m_pParent->SendMessage(WM_MYSQL, QUERY_COUNT2, (LPARAM)&message);
		if( m_pMainFrame!=m_pParent && m_pMainFrame  && ::IsWindow(m_pMainFrame->GetSafeHwnd())) 
			m_pMainFrame->SendMessage(WM_MYSQL, QUERY_COUNT2, (LPARAM)&message);
		if(!hilo->IsRunning())
			return 0;
		// guardado por compatibilidad
		if(i==0 && m_pParent && ::IsWindow(m_pParent->GetSafeHwnd()))
			m_pParent->SendMessage(WM_MYSQL, QUERY_EMPTY, (LPARAM) &message);

		int l = 1;
		int j;

		while ( (row = mysql_fetch_row( mysqlres )) ) {
			
			if(!hilo->IsRunning())
				break;
			message.Clean();

			j = mysql_num_fields( mysqlres );
			unsigned long *length= mysql_fetch_lengths(mysqlres);
			l++;
			for ( int k = 0 ; k < j ; k++ ) {
				if(k==0)
					for( int l=0; l<length[k]; l++)
						message.GetBinaryData()->Add(row[k][l]);
				else message.Add((((row[k]==NULL)||(!strlen(row[k])))?"":row[k]));
			}
			if(!hilo->IsRunning())
				break;
			if( m_pParent && ::IsWindow(m_pParent->GetSafeHwnd()))
				m_pParent->SendMessage(WM_MYSQL, QUERY_RESULT, (LPARAM) &message);
			else break;
			if( m_pMainFrame!=m_pParent && m_pMainFrame  && ::IsWindow(m_pMainFrame->GetSafeHwnd()))
				m_pMainFrame->SendMessage(WM_MYSQL, QUERY_RESULT, (LPARAM) &message);
			if(!hilo->IsRunning())
				break;
		}
		mysql_free_result( mysqlres );
		mysqlres= NULL;
	}
	return i;
}

int CArrayDB::Query(CString query, int id, CThreadAuto *hilo) {

	FUNCTION_STACK_TRACE("FFFFC008");
	ASSERT(hilo);

	MYSQL_ROW	row;

	ULONGLONG i=0;
	CMySQLMessage	message((DWORD)id,m_pParent);

	// ASSERT(m_pParent);

	if( !m_pMySQL) 
		if( !Conectar(id)) 
			return -1;
	if( !m_pMySQL)
		return -1;

	ASSERT(IsLoaded());
	if(!IsLoaded())
		return -1;
	if(!hilo->IsRunning())
		return 0;

	if( mysql_query(m_pMySQL, query)) {

		if(!hilo->IsRunning())
			return 0;
		if( (mysql_errno( m_pMySQL)==CR_SERVER_GONE_ERROR || mysql_errno( m_pMySQL)==CR_SERVER_LOST) && m_nReintentos) {
				m_nReintentos--;
				if(!hilo->IsRunning())
					return 0;
				CloseConections();
				if(!hilo->IsRunning())
					return 0;
				return Query(query, id, hilo);
		}
		if(!hilo->IsRunning())
			return 0;
		message.Add( GetLastError());
		message.Add(query);
		if( mysql_errno( m_pMySQL)==CR_CONN_HOST_ERROR) {
			if( m_pMySQL)
				mysql_close( m_pMySQL);
			m_pMySQL= NULL;
		}
		if(!hilo->IsRunning())
			return 0;
		if( m_pParent && ::IsWindow(m_pParent->GetSafeHwnd()))
			m_pParent->SendMessage(WM_MYSQL, QUERY_ERROR, (LPARAM) &message);
		if( m_pMainFrame!=m_pParent && m_pMainFrame  && ::IsWindow(m_pMainFrame->GetSafeHwnd()))
			m_pMainFrame->SendMessage(WM_MYSQL, QUERY_ERROR, (LPARAM) &message);
		return -1;
	} else {
	  
		if(!hilo->IsRunning())
			return 0;
		if( query.Find("INSERT INTO ")!=-1) {
			long lastid= mysql_insert_id(m_pMySQL);
			if( lastid) {
				query.Format("%ld", (long) lastid);
				message.Add(query);
				if(!hilo->IsRunning())
					return 0;
				if( m_pParent && ::IsWindow(m_pParent->GetSafeHwnd()))
					return m_pParent->SendMessage(WM_MYSQL, QUERY_RESULT, (LPARAM) &message);
				return 0;
			}
		}
		if(!hilo->IsRunning())
			return 0;
		MYSQL_RES *mysqlres = mysql_store_result( m_pMySQL);
		if(!mysqlres) 
			return i;
		if(!hilo->IsRunning())
			return 0;
		i = (ULONGLONG) mysql_num_rows(mysqlres);
		message.SetCount(i);
		if( m_pParent && ::IsWindow(m_pParent->GetSafeHwnd())) 
			m_pParent->SendMessage(WM_MYSQL, QUERY_COUNT2, (LPARAM)&message);
		if( m_pMainFrame!=m_pParent && m_pMainFrame  && ::IsWindow(m_pMainFrame->GetSafeHwnd())) 
			m_pMainFrame->SendMessage( WM_MYSQL, QUERY_COUNT2, (LPARAM)&message);
		if(!hilo->IsRunning())
			return 0;
		// guardado por compatibilidad
		if(i==0 && m_pParent && ::IsWindow(m_pParent->GetSafeHwnd()))
			m_pParent->SendMessage(WM_MYSQL, QUERY_EMPTY, (LPARAM) &message);
		if(!hilo->IsRunning())
			return 0;
		MYSQL_FIELD	*fd;
		while( fd = mysql_fetch_field( mysqlres )) {
            
			message.Add( fd->name);
			message.AddColumn(fd->name);
		}
		if(!hilo->IsRunning())
			return 0;
		if( m_pParent)
			if( ::IsWindow(m_pParent->GetSafeHwnd()))
				m_pParent->SendMessage(WM_MYSQL, QUERY_TITLE, (LPARAM) &message);

		int l = 1;
		int j;

		while ( (row = mysql_fetch_row( mysqlres )) ) {

			if(!hilo->IsRunning())
				break;
			message.Clean();
			j = mysql_num_fields( mysqlres ) ;
			l++;
			for ( int k = 0 ; k < j ; k++ ) 
				message.Add((((row[k]==NULL)||(!strlen(row[k])))?"":row[k]));

			if(!hilo->IsRunning())
				break;
			if( m_pParent && ::IsWindow(m_pParent->GetSafeHwnd()))
					m_pParent->SendMessage(WM_MYSQL, QUERY_RESULT, (LPARAM) &message);
			else break;
			if( m_pMainFrame!=m_pParent && m_pMainFrame  && ::IsWindow(m_pMainFrame->GetSafeHwnd()))
				m_pMainFrame->SendMessage(WM_MYSQL, QUERY_RESULT, (LPARAM) &message);
			if(!hilo->IsRunning())
				break;
		}
		mysql_free_result( mysqlres );
		mysqlres= NULL;
	}
	return i;
}
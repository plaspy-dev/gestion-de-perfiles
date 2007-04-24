#pragma once
# include "MySQLMessage.h"
# include "include/mysql.h"
# include <componentes/componentes.h>
# include <DLLModule.h>


# define	QUERY_RESULT	1
# define	QUERY_EMPTY		2
# define	QUERY_ERROR		3
# define	QUERY_END		4
# define	QUERY_TITLE		5
# define	QUERY_ESTADO	7
# define	QUERY_COUNT2	9

# define	THREAD_END		QUERY_END

# define SAFE_DELETE(p) if(p) { delete p; p=NULL; };
# define ESTADO_INICIANDO				0
# define ESTADO_CONECTANDO				1
# define ESTADO_CONECTADO				2
# define ESTADO_REALIZANDO_CONSULTA		3
# define ESTADO_ERROR_EN_CONSULTA		4
# define ESTADO_ALMACENANDO_CONSULTA	5
# define ESTADO_ALMACENANDO_RESULTADO	6
# define ESTADO_ALMACENANDO_COLUMNA		7
# define ESTADO_ALMACENANDO_FILAS		8
# define ESTADO_CONTANDO_CAMPOS			9
# define ESTADO_ALMACENANDO_CAMPOS		10
# define ESTADO_CAMPOS_ALMACENADOS		11
# define ESTADO_LIBERANDO_MEMORIA		12
# define ESTADO_TERMINADO				13

class CArrayDB : public CDLLModule
{
public:
	CArrayDB();
	~CArrayDB();
private:
	CArrayDB	*m_pSig;
	MYSQL		*m_pMySQL;
	int			m_nReintentos;
	CWnd*		m_pParent;
	int			m_nEstado;
	bool		m_bTryConect;
	CWnd*		m_pMainFrame;
protected:
	CString		m_szHost;
	CString m_szNombreDB;
	CString m_szUser;
	CString m_szPasswd;
	int		m_nPort;
	bool	m_bInscrutado;
public:
	void	SetHost(CString host);
	void	SetDB(CString db);
	void	SetUser(CString usuario);
	void	SetPassword(CString passwd);
	void	SetPort(int port);
	void	SetMainFrame(CWnd* mainframe);
	void	SetIncrustado(bool incrustado/*=false*/);

	CString	GetHost();
	CString GetDB();
	CString GetUser();
	CString GetPassword();
	int		GetPort();
	bool	EsIncrustado() const ;
public:
	void	ResetState();
	void	CloseConections();
	void	SetRetry(unsigned int reintentos);
	void	SetEstado(int estado=0);
	int		GetEstado();
	bool	Ping();
	bool	CanConnect();
	void	SetCanConnect(bool intentar=true);
	CArrayDB &operator=(const CArrayDB &database);
	CArrayDB *GetAt(int index);
	int		GetCount();
	void	Add( CArrayDB *info);
	void	RemoveAll();
	CString GetLastError();
	int		Query(CString query, int id, CThreadAuto *hilo);
	int		QueryBinary(CString query, int id, CThreadAuto *hilo);
	bool	Conectar(int id=0, bool seleccionardb=true);
	ULONG	Find( CString query, CStringArray *datos, CStringArray *columnas=NULL);
	void	SetPadre(CWnd* parent);

	// necesario para cargar las funciones del dll.
	// declaramos las funciones y luego si las usamos
	DECLARE_DLL_FUNCTION(int, STDCALL,mysql_ping, (MYSQL *))
	DECLARE_DLL_FUNCTION(void, STDCALL,mysql_free_result, (MYSQL_RES *))
	DECLARE_DLL_FUNCTION(void, STDCALL,mysql_close, (MYSQL *))
	DECLARE_DLL_FUNCTION(const char *, STDCALL,mysql_error, (MYSQL *))
	DECLARE_DLL_FUNCTION(MYSQL *, STDCALL,mysql_init, (MYSQL *))
	DECLARE_DLL_FUNCTION(MYSQL *, STDCALL,mysql_real_connect, (MYSQL *, const char *,const char *,const char *,const char *,unsigned int,const char *,unsigned long ))
	DECLARE_DLL_FUNCTION(unsigned int , STDCALL,mysql_errno, (MYSQL *))
	DECLARE_DLL_FUNCTION(int , STDCALL,mysql_query, (MYSQL *, const char *))
	DECLARE_DLL_FUNCTION(my_ulonglong, STDCALL,mysql_insert_id, (MYSQL *))
	DECLARE_DLL_FUNCTION(MYSQL_RES *, STDCALL,mysql_store_result, (MYSQL *))
	DECLARE_DLL_FUNCTION(my_ulonglong , STDCALL,mysql_num_rows, (MYSQL_RES *))
	DECLARE_DLL_FUNCTION(MYSQL_FIELD *, STDCALL,mysql_fetch_field, (MYSQL_RES *))
	DECLARE_DLL_FUNCTION(MYSQL_ROW, STDCALL,mysql_fetch_row, (MYSQL_RES *))
	DECLARE_DLL_FUNCTION(unsigned int, STDCALL,mysql_num_fields, (MYSQL_RES *))
	DECLARE_DLL_FUNCTION(unsigned long *, STDCALL,mysql_fetch_lengths, (MYSQL_RES *))
	DECLARE_DLL_FUNCTION(int , STDCALL,mysql_server_init, (int, char **, char **))
	DECLARE_DLL_FUNCTION(void, STDCALL,mysql_server_end, (void))
	DECLARE_DLL_FUNCTION(int, STDCALL,mysql_options, (MYSQL *,enum mysql_option,const char *))

	BEGIN_DLL_INIT()
		INIT_DLL_FUNCTION(int, STDCALL,mysql_ping, (MYSQL *), "mysql_ping")
		INIT_DLL_FUNCTION(void, STDCALL,mysql_free_result, (MYSQL_RES *), "mysql_free_result")
		INIT_DLL_FUNCTION(void, STDCALL,mysql_close, (MYSQL *), "mysql_close")
		INIT_DLL_FUNCTION(const char *, STDCALL,mysql_error, (MYSQL *), "mysql_error")
		INIT_DLL_FUNCTION(MYSQL *, STDCALL,mysql_init, (MYSQL *), "mysql_init")
		INIT_DLL_FUNCTION(MYSQL *, STDCALL,mysql_real_connect, (MYSQL *, const char *,const char *,const char *,const char *,unsigned int,const char *,unsigned long ), "mysql_real_connect")
		INIT_DLL_FUNCTION(unsigned int , STDCALL,mysql_errno, (MYSQL *), "mysql_errno")
		INIT_DLL_FUNCTION(int , STDCALL,mysql_query, (MYSQL *, const char *), "mysql_query")
		INIT_DLL_FUNCTION(my_ulonglong, STDCALL,mysql_insert_id, (MYSQL *),"mysql_insert_id")
		INIT_DLL_FUNCTION(MYSQL_RES *, STDCALL,mysql_store_result, (MYSQL *),"mysql_store_result")
		INIT_DLL_FUNCTION(my_ulonglong, STDCALL,mysql_num_rows, (MYSQL_RES *),"mysql_num_rows")
		INIT_DLL_FUNCTION(MYSQL_FIELD *, STDCALL,mysql_fetch_field, (MYSQL_RES *),"mysql_fetch_field")
		INIT_DLL_FUNCTION(MYSQL_ROW, STDCALL,mysql_fetch_row, (MYSQL_RES *),"mysql_fetch_row")
		INIT_DLL_FUNCTION(unsigned int, STDCALL,mysql_num_fields, (MYSQL_RES *),"mysql_num_fields")
		INIT_DLL_FUNCTION(unsigned long *, STDCALL,mysql_fetch_lengths, (MYSQL_RES *),"mysql_fetch_lengths")
		INIT_DLL_FUNCTION(int, STDCALL,mysql_server_init, (int , char **, char **),"mysql_server_init")
		INIT_DLL_FUNCTION(void, STDCALL,mysql_server_end, (void),"mysql_server_end")
		INIT_DLL_FUNCTION(int, STDCALL,mysql_options, (MYSQL *,enum mysql_option,const char *),"mysql_options")
	END_DLL_INIT()
};

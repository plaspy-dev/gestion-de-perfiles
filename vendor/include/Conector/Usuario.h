#pragma once

class __CONECTOR_EXPORT___ CUsuario
{
protected:
	CString		m_szNombre;
	DWORD		m_dwID;
	DWORD		m_dwGrupo;
	CString		m_szDescripcion;
	CString		m_szCodificado;
	DWORD		m_dwData;
public:
	CUsuario(void);
	CUsuario(CString Nombre, DWORD id, DWORD grupo);
	virtual ~CUsuario(void);
	// todos los grupos a los cuales pertenece

	void		SetGrupo(DWORD grupos);
	void		SetUser(CString usuario);
	void		SetDescripcion(CString descripcion);
	void		SetData(DWORD data);
	DWORD		GetData();
	CString		GetDescripcion();
	void		SetId(DWORD id);
	bool		Pertenece(DWORD idGrupo);
	DWORD		GetID();
	CString		GetNombre();
	DWORD		GetGrupo();
	CString		GetGrupos();
	CString		GetCodificado();
	static	CString	GetGrupos(DWORD grupo);
	static	DWORD	GetGrupos(CString grupo);
	static int GetNombresGrupos(CStringArray *nombres, CUIntArray *Codigos, CStringArray *metodo=NULL, CStringArray *descripciones=NULL);
	static CString GetCodificado(CString Usuario);
};

#pragma once
# include <Componentes/Componentes.h>
#include "usuario.h"

class CHorario {
public:
	CHorario() {
		m_nDia=0;
	}
	CHorario(int dia, int entrada, int salida) {

		m_nDia=dia;
		m_nEntrada= entrada;
		m_nSalida= salida;
	}
public:
	int m_nDia;
	int m_nEntrada;
	int m_nSalida;
};

class __CONECTOR_EXPORT___ CUsuario2 : public CUsuario
{
protected:
	CString			m_szNombreCompleto;
	CString			m_szContrasena;
	CMyTime			m_dtCreacion;
	CMyTime			m_dtVigencia;
	CMyTime			m_dtUCambio;
	CMyTime			m_dtAcceso;
	CPtrArray		m_pHorario;
	int				m_nExpira;
	int				m_nAutonumerico;
public:
	CUsuario2(void);
	CUsuario2(CString Nombre, DWORD id, DWORD grupo);
public:
	~CUsuario2(void);
	void SetContrasena(const CString &contrasena);
	CString GetContrasena() const;
	void SetNombreCompleto(const CString &nombre);
	CString GetNombreCompleto() const;
	void SetCreacion(CMyTime tiempo);
	CMyTime GetCreacion() const;
	void SetVigencia(CMyTime tiempo);
	CMyTime GetVigencia() const;
	void SetUCambio(CMyTime tiempo);
	CMyTime GetUCambio() const;
	void SetAcceso(CMyTime tiempo);
	CMyTime GetAcceso() const;
	void AddHorario(int dia, int entrada, int salida);
	CPtrArray *GetHorario();
	bool CanEntry(COleDateTime actual) const;
	void CleanHorario();
	void SetExpiracion(int expira);
	int GetExpiracion() const;
	void SetAutonumerico(int autonumerico);
	int GetAutonumerico() const;
	CUsuario2 &operator=(const CUsuario2 &user);
};

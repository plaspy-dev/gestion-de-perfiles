#pragma once
#include "usuario.h"
# include <Componentes/Componentes.h>

class __CONECTOR_EXPORT___ CUsuario3 : public CUsuario
{
protected:
	CMiscelaneosEx	m_pPermisos;
	CString m_szNombreCompleto;
	CString m_szContrasena;
public:
	CUsuario3(void);
	CUsuario3(CString Nombre, DWORD id, DWORD grupo);
	~CUsuario3(void);
	CUsuario3 &operator=(const CUsuario3 &user);
	void SetContrasena(const CString &contrasena);
	CString GetContrasena() const;
	void SetNombreCompleto(const CString &nombre);
	CString GetNombreCompleto() const;
	CMiscelaneosEx *GetPermisos();
	void AddPermisos(const CString &clase, DWORD permiso);
	DWORD GetPermiso(const CString &clase) const;
	DWORD GetPermiso(CFormView *view) const;
	void SetPermisos(const CMiscelaneosEx &info);
	static DWORD GetPermiso(const CMiscelaneosEx &permisos,const CString &clase);
	bool CanRead(CFormView *view);
	bool CanPrint(CFormView *view);
	bool CanInsert(CFormView *view);
	bool CanUpdate(CFormView *view);
	bool CanDelete(CFormView *view);
	bool IsAdmin(CFormView *view);

	bool CanRead(const CString &clase);
	bool CanPrint(const CString &clase);
	bool CanInsert(const CString &clase);
	bool CanUpdate(const CString &clase);
	bool CanDelete(const CString &clase);
	bool IsAdmin(const CString &clase);
};

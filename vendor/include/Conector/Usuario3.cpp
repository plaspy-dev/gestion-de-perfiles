# include "stdafx.h"
#include "Usuario3.h"


CUsuario3::CUsuario3(void)
{
}

CUsuario3::CUsuario3(CString Nombre, DWORD id, DWORD grupo) {

	m_szNombre= Nombre;
	m_dwID= id;
	m_dwGrupo=grupo;
}

CUsuario3::~CUsuario3(void)
{
}

CUsuario3 &CUsuario3::operator=(const CUsuario3 &user) {
	
	if (&user!= this)
	{
		m_szNombre= user.m_szNombre;
		m_dwID= user.m_dwID;
		m_dwGrupo= user.m_dwGrupo;
		m_szDescripcion= user.m_szDescripcion;
		m_szCodificado= user.m_szCodificado;
		m_dwData= user.m_dwData;
		m_szNombreCompleto= user.m_szNombreCompleto;
		m_szContrasena= user.m_szContrasena;
		m_pPermisos.DeleteAll();
		for(int i=0; i<user.m_pPermisos.GetCount(); i++) {
			CMiscelaneosEx *info= user.m_pPermisos[i];
			m_pPermisos.Add(info->GetNombre(), info->GetData());
		}
	}
	return *this;
}

void CUsuario3::SetContrasena(const CString &contrasena) {
	m_szContrasena= contrasena;
}

CString CUsuario3::GetContrasena() const {
	return m_szContrasena;
}

void CUsuario3::SetNombreCompleto(const CString &nombre) {
	m_szNombreCompleto= nombre;
}

CString CUsuario3::GetNombreCompleto() const {
	return m_szNombreCompleto;
}

CMiscelaneosEx *CUsuario3::GetPermisos() {

	return &m_pPermisos;
}

void CUsuario3::AddPermisos(const CString &clase, DWORD permiso) {

	m_pPermisos.Add(clase, permiso);
}

DWORD CUsuario3::GetPermiso(const CMiscelaneosEx &permisos,const CString &clase) {

	DWORD permiso=0;
	CMiscelaneosEx *info =permisos.Find(clase);
	if(info)
		permiso= info->GetData();
	return permiso;
}

bool CUsuario3::CanRead(const CString &clase) {

	return GetPermiso(clase)>0;
}

bool CUsuario3::CanPrint(const CString &clase) {

	return GetPermiso(clase)>1;
}

bool CUsuario3::CanInsert(const CString &clase) {

	return GetPermiso(clase)>3;
}

bool CUsuario3::CanUpdate(const CString &clase) {

	return GetPermiso(clase)>7;
}

bool CUsuario3::CanDelete(const CString &clase) {

	return GetPermiso(clase)>15;
}

bool CUsuario3::IsAdmin(const CString &clase) {

	return GetPermiso(clase)>31;
}

bool CUsuario3::CanRead(CFormView *view) {

	return GetPermiso(view)>0;
}

bool CUsuario3::CanPrint(CFormView *view) {

	return GetPermiso(view)>1;
}

bool CUsuario3::CanInsert(CFormView *view) {

	return GetPermiso(view)>3;
}

bool CUsuario3::CanUpdate(CFormView *view) {

	return GetPermiso(view)>7;
}

bool CUsuario3::CanDelete(CFormView *view) {

	return GetPermiso(view)>15;
}

bool CUsuario3::IsAdmin(CFormView *view) {

	return GetPermiso(view)>31;
}

DWORD CUsuario3::GetPermiso(CFormView *view) const {

	ASSERT(view);
	if(view==NULL)
		return 0;
	return GetPermiso(view->GetRuntimeClass()->m_lpszClassName);
}

DWORD CUsuario3::GetPermiso(const CString &clase) const {

	if(!m_dwGrupo)
		return 63;
	DWORD permiso=0;
	CMiscelaneosEx *info =m_pPermisos.Find(clase);
	if(info)
		permiso= info->GetData();
	return permiso;
}

void CUsuario3::SetPermisos(const CMiscelaneosEx &info) {

	m_pPermisos.DeleteAll();
	for(int i=0; i<info.GetCount(); i++) {
		CMiscelaneosEx *temp=info[i];
		m_pPermisos.Add(temp->GetNombre(), temp->GetData());
	}
}

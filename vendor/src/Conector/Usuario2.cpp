# include "stdafx.h"
#include "Usuario2.h"

CUsuario2::CUsuario2(void)
{
	m_nExpira=0;
	m_nAutonumerico=0;
}

CUsuario2::CUsuario2(CString Nombre, DWORD id, DWORD grupo) {

	m_szNombre= Nombre;
	m_dwID= id;
	m_dwGrupo=grupo;
	m_nExpira=0;
}

CUsuario2 &CUsuario2::operator=(const CUsuario2 &user) {

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
		m_dtCreacion= user.m_dtCreacion;
		m_dtVigencia= user.m_dtVigencia;
		m_dtUCambio= user.m_dtUCambio;
		m_dtAcceso= user.m_dtAcceso;
		m_nExpira= user.m_nExpira;
		m_nAutonumerico= user.m_nAutonumerico;
		CleanHorario();
		for(int i=0; i<user.m_pHorario.GetCount(); i++) {
			CHorario *hor=(CHorario *)user.m_pHorario[i];
			m_pHorario.Add(new CHorario(hor->m_nDia, hor->m_nEntrada, hor->m_nSalida));
		}
	}
	return *this;
}

void CUsuario2::SetExpiracion(int expira) {

	m_nExpira= expira;
}

int CUsuario2::GetExpiracion() const {

	return m_nExpira;
}

CUsuario2::~CUsuario2(void)
{
	CleanHorario();
}

void CUsuario2::SetContrasena(const CString &contrasena) {
	m_szContrasena= contrasena;
}
CString CUsuario2::GetContrasena() const {
	return m_szContrasena;
}

void CUsuario2::SetNombreCompleto(const CString &nombre) {
	m_szNombreCompleto= nombre;
}
CString CUsuario2::GetNombreCompleto() const {
	return m_szNombreCompleto;
}
void CUsuario2::SetCreacion(CMyTime tiempo) {
	m_dtCreacion= tiempo;
}
CMyTime CUsuario2::GetCreacion() const {
	return m_dtCreacion;
}
void CUsuario2::SetVigencia(CMyTime tiempo) {
	m_dtVigencia= tiempo;
}
CMyTime CUsuario2::GetVigencia() const {
	return m_dtVigencia;
}
void CUsuario2::SetUCambio(CMyTime tiempo) {
	m_dtUCambio= tiempo;
}
CMyTime CUsuario2::GetUCambio() const {
	return m_dtUCambio;
}
void CUsuario2::SetAcceso(CMyTime tiempo) {
	m_dtAcceso= tiempo;
}
CMyTime CUsuario2::GetAcceso() const {
	return m_dtAcceso;
}

void CUsuario2::AddHorario(int dia, int entrada, int salida) {
	
	m_pHorario.Add(new CHorario(dia, entrada, salida));
}
CPtrArray *CUsuario2::GetHorario() {

	return &m_pHorario;
}

bool CUsuario2::CanEntry(COleDateTime actual) const {

	if(m_pHorario.IsEmpty())
		return true;
	for(int i=0;i<m_pHorario.GetCount();i++) {
		CHorario *hor= (CHorario *)m_pHorario[i];
		// el lunes es el dia 2, y el domingo esta invertido
		if(hor->m_nDia+2==actual.GetDayOfWeek() || (actual.GetDayOfWeek()==0 && hor->m_nDia==6)) {

			COleDateTime entrada, salida;
			entrada.SetDateTime(actual.GetYear(), actual.GetMonth(), actual.GetDay(), hor->m_nEntrada==24 ? 23 : hor->m_nEntrada, hor->m_nEntrada==24 ? 59 : 0, hor->m_nEntrada==24 ? 59 : 0);
			salida.SetDateTime(actual.GetYear(), actual.GetMonth(), actual.GetDay(), hor->m_nSalida==24 ? 23 : hor->m_nSalida, hor->m_nSalida==24 ? 59 : 0, hor->m_nSalida==24 ? 59 : 0);

			if(actual>=entrada && actual<=salida)
				return true;
		}
	}
	return false;
}

void CUsuario2::CleanHorario() {

	for(int i=0;i<m_pHorario.GetCount();i++) {
		CHorario *hor= (CHorario *)m_pHorario[i];
		delete hor;
	}
	m_pHorario.RemoveAll();
}

void CUsuario2::SetAutonumerico(int autonumerico) {

	m_nAutonumerico= autonumerico;
}

int CUsuario2::GetAutonumerico() const {

	return m_nAutonumerico;
}
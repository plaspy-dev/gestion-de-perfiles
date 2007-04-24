#include "StdAfx.h"
#include "arrayusuarios.h"

CArrayUsuarios::CArrayUsuarios(void)
{
	m_pSig= NULL;
}

CArrayUsuarios::~CArrayUsuarios(void)
{
	if( m_pSig) {
		delete m_pSig;
		m_pSig= NULL;
	}
}

void CArrayUsuarios::DeleteAll() {

	m_szNombre.Empty();
	m_dwID=0;
	m_dwGrupo=0;
	if( m_pSig) {
		delete m_pSig;
		m_pSig= NULL;
	}
}

void CArrayUsuarios::AddUsuario( CString &nombre, DWORD id, DWORD grupo) {

	if( m_szNombre.IsEmpty()) {

		m_szNombre= nombre;
		m_dwID= id;
		m_dwGrupo= grupo;
		return;
	}
	if( !m_pSig) m_pSig= new CArrayUsuarios();
	m_pSig->AddUsuario(nombre, id, grupo);
}

int	CArrayUsuarios::GetCount() {

	CArrayUsuarios *info= this;
	if( info->m_szNombre.IsEmpty())
		return 0;
	int count=0;
	while( info) {
		count++;
		info= info->m_pSig;
	}
	return count;
}

CUsuario *CArrayUsuarios::GetAt(int index) {

	CArrayUsuarios *info= this;
	if( info->m_szNombre.IsEmpty())
		return NULL;
	int count=0;
	while( info) {
		if( count==index)
			return (CUsuario *) info;
		count++;
		info= info->m_pSig;
	}
	return NULL;
}

CUsuario *CArrayUsuarios::FindUsuario( CString nombre) {

	CArrayUsuarios *info= this;
	while( info) {

		if( info->m_szNombre==nombre)
			return (CUsuario *)info;
		info= info->m_pSig;
	}
	return NULL;
}

CUsuario *CArrayUsuarios::FindUsuario( DWORD id) {

	CArrayUsuarios *info= this;
	while( info) {

		if( info->m_dwID==id)
			return (CUsuario *)info;
		info= info->m_pSig;
	}
	return NULL;
}
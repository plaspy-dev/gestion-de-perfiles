#include "StdAfx.h"
#include "extras/MiscelaneosEx.h"

CMiscelaneosEx::CMiscelaneosEx(const CString &nombre, DWORD data, CString szExtra/*=""*/) {

	m_szNombre= nombre;
	m_dwData= data;
	m_szExtra= szExtra;
	m_pSig= NULL;
}

CMiscelaneosEx::CMiscelaneosEx(void)
{
	m_pSig= NULL;
}

CMiscelaneosEx::~CMiscelaneosEx(void)
{
	CMiscelaneosEx *info= m_pSig, *aux;
	while( info) {
		aux= info->m_pSig;
		info->m_pSig= NULL;
		delete info;
		info=aux;
	}
}

void CMiscelaneosEx::DeleteAll() {

	m_szNombre.Empty();
	if( m_pSig) {
		delete m_pSig;
		m_pSig= NULL;
	}
}

void CMiscelaneosEx::SetData(DWORD data) {
	m_dwData= data;
}

void CMiscelaneosEx::SetExtra(CString extra) {
	m_szExtra= extra;
}

void CMiscelaneosEx::SetNombre(CString nombre) {
	m_szNombre= nombre;
}

CMiscelaneosEx *CMiscelaneosEx::GetNext() const {

	return m_pSig;
}

CMiscelaneosEx *CMiscelaneosEx::FindNext(DWORD data) const {

	return m_pSig ? m_pSig->Find(data) : NULL;
}

CMiscelaneosEx *CMiscelaneosEx::Add(const CString &nombre, DWORD data, CString szExtra) {

	if( m_szNombre.IsEmpty()) {

		m_szNombre= nombre;
		m_dwData= data;
		m_szExtra= szExtra;
		return this;
	}
	CMiscelaneosEx **info= &m_pSig;
	while( *info)
		info= &(*info)->m_pSig;
	ASSERT(*info==NULL);
	*info= new CMiscelaneosEx();
	return (*info)->Add(nombre, data, szExtra);
}

CMiscelaneosEx *CMiscelaneosEx::Find(CString nombre) const {

	const CMiscelaneosEx *info= this;
	while( info) {

		if( info->m_szNombre==nombre)
			return (CMiscelaneosEx *)info;
		info= info->m_pSig;
	}
	return NULL;
}

void CMiscelaneosEx::DeleteLast() {
	if( m_pSig) {
		if( m_pSig->m_pSig) {
			m_pSig->DeleteLast();
		} else {
			delete m_pSig;
			m_pSig=NULL;
		}
	}
}

void CMiscelaneosEx::RemoveAt(int index) {
	
	CMiscelaneosEx *info=GetAt(index);
	if( info) {
		CMiscelaneosEx *update=info;
		info= info->m_pSig;
		while(info) {
			*update=*info;
			info=info->m_pSig;
			update=update->m_pSig;
		}
		if( update==this) {
			m_szNombre.Empty();
			m_dwData=0;
		} else if( update) {
			DeleteLast();
			update= NULL;
		}
	}
}

CMiscelaneosEx *CMiscelaneosEx::operator[]( int nIndex ) const {

	return GetAt(nIndex);
}

CMiscelaneosEx &CMiscelaneosEx::operator = (CMiscelaneosEx &item) {

	if( &item!=this) {

		m_szExtra=item.m_szExtra;
		m_szNombre= item.m_szNombre;
		m_dwData= item.m_dwData;
	}
	return *this;
}

CMiscelaneosEx *CMiscelaneosEx::Find(DWORD data) const {

	const CMiscelaneosEx *info= this;
	while( info) {

		if( info->m_dwData==data)
			return (CMiscelaneosEx *)info;
		info= info->m_pSig;
	}
	return NULL;
}

CMiscelaneosEx *CMiscelaneosEx::FindNoCase(CString nombre) const {

	const CMiscelaneosEx *info= this;
	while( info) {

		if( !info->m_szNombre.CompareNoCase(nombre))
			return (CMiscelaneosEx *)info;
		info= info->m_pSig;
	}
	return NULL;
}

CMiscelaneosEx *CMiscelaneosEx::FindExtraNoCase(CString nombre) const {

	const CMiscelaneosEx *info= this;
	while( info) {
		if( !info->m_szExtra.CompareNoCase(nombre))
			return (CMiscelaneosEx *)info;
		info= info->m_pSig;
	}
	return NULL;
}

int CMiscelaneosEx::GetCount() const {

	if( m_szNombre.IsEmpty())
		return 0;
	const CMiscelaneosEx *info= this;
	int i=0;
	while( info) {

		i++;
		info= info->m_pSig;
	}
	return i;
}

CMiscelaneosEx *CMiscelaneosEx::GetAt(int index) const {

	const CMiscelaneosEx *info= this;
	int i=0;
	while( info) {

		if( index==i)
			return (CMiscelaneosEx *)info;
		i++;
		info= info->m_pSig;
	}
	return NULL;
}

CString CMiscelaneosEx::GetNombre() const {
	return m_szNombre;
}

DWORD CMiscelaneosEx::GetData() const {

	return m_dwData;
}

CString CMiscelaneosEx::GetExtra() const {

	return m_szExtra;
}


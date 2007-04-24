#include "StdAfx.h"
#include "items.h"

CItems::CItems(void)
{
	m_pSig= NULL;
}

CItems::~CItems(void)
{
	if( m_pSig) {

		delete m_pSig;
		m_pSig= NULL;
	}
}

bool LoadDataFromXML(CString Archivo) {

	return true;
}

void AddItem( CString &nombre, DWORD &data, float &Precio, CString &Descripcion) {

}
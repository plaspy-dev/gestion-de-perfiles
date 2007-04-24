# include "stdafx.h"
#include "MySQLMessage.h"

CMySQLMessage::CMySQLMessage(LPARAM lParam) {

	CMySQLMessage *mess= (CMySQLMessage *)lParam;
	*this= *mess;
	m_pParent= NULL;
}

CMySQLMessage::CMySQLMessage(DWORD id/*=0*/, CWnd *parent/*=NULL*/)
{
	m_nCount= 0;
	m_nID= id;
	m_pParent= parent;
}

CMySQLMessage::~CMySQLMessage(void)
{
}

DWORD CMySQLMessage::GetID() const {
	return m_nID;
}

CMySQLMessage &CMySQLMessage::operator=(CMySQLMessage &mensaje) {

	if (&mensaje != this)
	{
		m_saCampos.RemoveAll();
		m_saCampos.Copy(mensaje.m_saCampos);
		m_pMessageB.RemoveAll();
		m_pMessageB.Copy(mensaje.m_pMessageB);
		m_pMessage.RemoveAll();
		m_pMessage.Copy(mensaje.m_pMessage);
		m_nCount= mensaje.m_nCount;
		m_nID= mensaje.m_nID;
		m_pParent= mensaje.m_pParent;
	}
	return *this;
}

CString& CMySQLMessage::operator[]( const CString &campo) {

	int index= GetCampo(campo);
	
	ASSERT(index>=0);
	ASSERT(index<m_pMessage.GetCount());

	if(index<0 || index>=m_pMessage.GetCount()) {

		m_szVacio.Empty();
		return m_szVacio;
	}
	return m_pMessage[index];
}

CString& CMySQLMessage::operator[]( const int index) {

	int pos= index;

	ASSERT(pos>=0);
	ASSERT(pos<m_pMessage.GetCount());

	if(pos<0 || pos>=m_pMessage.GetCount()) {

		m_szVacio.Empty();
		return m_szVacio;
	}
	return m_pMessage[pos];
}

int	CMySQLMessage::GetCampo(const CString &campo) {

	for(int i=0; i<m_saCampos.GetCount(); i++) {
		if(!m_saCampos[i].CompareNoCase(campo)) {
			if(!m_pMessageB.IsEmpty())
				i--;
			return i;
		}
	}
	ASSERT(false);
	return -1;
}

CString	CMySQLMessage::GetAt(int index) {

	return (*this)[index];
}

CString CMySQLMessage::GetAt(const CString &campo) {

	return (*this)[campo];
}

CByteArray *CMySQLMessage::GetBinaryData() {

	return &m_pMessageB;
}

void CMySQLMessage::Clean() {

	m_pMessageB.RemoveAll();
	m_pMessage.RemoveAll();
}

int CMySQLMessage::AddColumn(const CString &columna) {

	return m_saCampos.Add(columna);
}

CString CMySQLMessage::GetColumna(int index) {

	return m_saCampos[index];
}

int CMySQLMessage::GetColumnCount() {

	return m_saCampos.GetCount();
}

int CMySQLMessage::GetCount() const {

	return m_nCount;
}

void CMySQLMessage::SetCount(int count) {

	m_nCount= count;
}

void CMySQLMessage::Add(const CString &campo) {

	m_pMessage.Add(campo);
}

CStringArray *CMySQLMessage::GetMensaje() {

	return &m_pMessage;
}

CWnd *CMySQLMessage::GetParent() {

	return m_pParent;
}

void CMySQLMessage::SetParent(CWnd *parent) {

	m_pParent= parent;
}

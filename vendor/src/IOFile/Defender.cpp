#include "StdAfx.h"
# include "iofile.h"
#include "defender.h"
# include "math.h"


CDefender::CDefender() {
	m_nID=m_pDiskID.GetSerialNumber();
}

CDefender::CDefender(CString serial)
{
	m_nID=m_pDiskID.GetSerialNumber();
	m_szSerial=serial;
	m_szRealSerial="";
}

CDefender::~CDefender(void)
{
}


void CDefender::GetSerial() {

	m_szRealSerial= CApplicacion::GetAppName();
	
	if(m_szRealSerial.IsEmpty())
		return;

	double algo=m_nID;
	algo*=pow((double)CApplicacion::GetID(), -3.15);
	algo+=pow((double)m_nID, 0.1416);

	m_szRealSerial.Format("%0.lf", algo);

	char serial[MAX_PATH];
	strcpy(serial, m_szRealSerial);
	CMD5Checksum m_pCoding;
	m_szRealSerial= m_pCoding.GetMD5((BYTE *)serial, strlen(serial));
	m_szRealSerial.MakeUpper();
	m_szRealSerial= m_szRealSerial.Mid(5,25);
}

CDefender *CDefender::IsValidSerial() {

	if(m_szSerial.IsEmpty())
		return NULL;
	for( int i=0; i<m_pDiskID.GetDiscosDuros(); i++) {
		m_nID= m_pDiskID.GetSerialNumber(i);
		GetSerial();
		if( m_szRealSerial.IsEmpty())
			continue;
		if( m_szSerial==m_szRealSerial)
			return this;
	}
	return NULL;
}

CString CDefender::GetSerialNumber() {

	if(m_szRealSerial.IsEmpty())
		GetSerial();
	return m_szRealSerial;
}

void CDefender::SetID(unsigned int id) {

	m_nID=id;
	m_szRealSerial="";
}

CString CDefender::GetKernel() {

	CString texto;
	texto.Format("%u", m_nID);
	return texto;
}

CDiskID *CDefender::GetDiskID() {

	return &m_pDiskID;
}

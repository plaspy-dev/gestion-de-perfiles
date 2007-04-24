#include "StdAfx.h"
#include "mytime.h"

CMyTime::CMyTime(bool currenttime/*=false*/)
{
	if(currenttime) {
		COleDateTime tiempo= COleDateTime::GetCurrentTime();
		*this= tiempo;
	}
}

CMyTime::~CMyTime(void)
{
}

CMyTime::CMyTime(const CString &tiempo) {

	SetStringDateTime(tiempo);
}

CMyTime::CMyTime(COleDateTime tiempo) {

	*this= tiempo;
}

CMyTime::CMyTime(CTime tiempo) {

	*this= tiempo;
}

CMyTime &CMyTime::operator = (const CString &tiempo) {

	SetStringDateTime(tiempo);
	return *this;
}

CMyTime &CMyTime::operator = (const CMyTime &tiempo) {

	if(this!=&tiempo) {
		SYSTEMTIME systime;
		tiempo.GetAsSystemTime(systime);
		COleDateTime::SetDateTime(systime.wYear, systime.wMonth, systime.wDay, systime.wHour, systime.wMinute, systime.wSecond);
		if(tiempo.m_status!=COleDateTime::valid) 
			this->m_status=tiempo.m_status;
	}
	return *this;
}

CMyTime &CMyTime::operator = (const CTime &tiempo) {

	SYSTEMTIME systime;
	tiempo.GetAsSystemTime(systime);
	COleDateTime::SetDateTime(systime.wYear, systime.wMonth, systime.wDay, systime.wHour, systime.wMinute, systime.wSecond);
	return *this;
}

CMyTime &CMyTime::operator = (const COleDateTime &tiempo) {

	SYSTEMTIME systime;
	tiempo.GetAsSystemTime(systime);
	COleDateTime::SetDateTime(systime.wYear, systime.wMonth, systime.wDay, systime.wHour, systime.wMinute, systime.wSecond);
	if(tiempo.m_status!=COleDateTime::valid) 
		this->m_status=tiempo.m_status;
	return *this;
}

CMyTime::operator CTime() const {

	SYSTEMTIME systime;
	GetAsSystemTime(systime);
	if(m_status!=valid)
		return CTime(0);
	return CTime(systime);
}

CMyTime::operator COleDateTime() const {

	SYSTEMTIME systime;
	GetAsSystemTime(systime);
	return COleDateTime(systime);
}

bool CMyTime::EsValido() const {

	return GetStatus()==valid;
}

CString	CMyTime::GetFechaLargaFormat() {
	
	char formato[255];
	GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SLONGDATE, formato, 255);
	return formato;
}

CString	CMyTime::GetFechaCortaFormat() {
	
	char formato[255];
	GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SSHORTDATE, formato, 255);
	return formato;
}

CString	CMyTime::GetHoraFormat() {

	char formato[255];
	GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_STIMEFORMAT, formato, 255);
	return formato;
}

CString CMyTime::GetCustomFormat(CString formato) const {

	SYSTEMTIME systime;
	GetAsSystemTime(systime);
	char salida[255];
	GetDateFormat(LOCALE_USER_DEFAULT, 0, &systime, formato, salida, 255);
	return salida;
}

CString	CMyTime::GetFecha(bool larga) const {

	if(!EsValido())
		return NULL;
	SYSTEMTIME systime;
	GetAsSystemTime(systime);
	char salida[255];
	GetDateFormat(LOCALE_USER_DEFAULT, larga ? DATE_LONGDATE : DATE_SHORTDATE , &systime, NULL, salida, 255);
	return salida;
}

CString	CMyTime::GetHora() const {

	if(!EsValido())
		return NULL;
	SYSTEMTIME systime;
	GetAsSystemTime(systime);
	char salida[255];
	GetTimeFormat(LOCALE_USER_DEFAULT, LOCALE_NOUSEROVERRIDE, &systime, NULL, salida, 255);
	return salida;
}

CString	CMyTime::GetFechaHora(bool larga) const {

	CString temp= GetFecha(larga);
	if( larga)
		temp+=",";
	temp+= " ";
	return  temp + GetHora();
}

void CMyTime::SetStringDateTime(const CString &tiempo) {

	if( tiempo.IsEmpty() || tiempo.GetLength()<9 || tiempo=="0000-00-00" || tiempo=="0000-00-00 00:00:00") {
		SetDate(0,0,0);
		m_status= error;
		return;
	}

	CString ano,mes,dia,hora,min,sec;

	ano+= tiempo.GetAt(0);
	ano+= tiempo.GetAt(1);
	ano+= tiempo.GetAt(2);
	ano+= tiempo.GetAt(3);
	mes+= tiempo.GetAt(5);
	mes+= tiempo.GetAt(6);
	dia+= tiempo.GetAt(8);
	dia+= tiempo.GetAt(9);
	if( tiempo.GetLength()>10) {

		hora= tiempo.GetAt(11);
		hora+= tiempo.GetAt(12);
		// mes son los minutos
		min= tiempo.GetAt(14);
		min+= tiempo.GetAt(15);
		sec= tiempo.GetAt(17);
		sec+= tiempo.GetAt(18);
	}
	COleDateTime::SetDateTime(atol(ano), atol(mes), atol(dia), atol(hora), atol(min), atol(sec));
}

CString	CMyTime::GetFormatoMysql(bool hora, bool segundos/*=false*/) const {

	return EsValido() ? Format(hora ? ( segundos ? "%Y-%m-%d %H:%M:%S" : "%Y-%m-%d %H:%M:00") : "%Y-%m-%d") : "";
}

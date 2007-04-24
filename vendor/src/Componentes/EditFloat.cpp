#include "StdAfx.h"
#include "editfloat.h"

CEditFloat::CEditFloat(void)
{
	m_dNumero=0;
	m_bFocus=false;
	m_bEnableMinus=false;
	GetLocale(m_szDecimal, m_szMiles);
}

CEditFloat::~CEditFloat(void)
{
}

BEGIN_MESSAGE_MAP(CEditFloat, CEditXP)
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	ON_WM_CHAR()
END_MESSAGE_MAP()

CString CEditFloat::CleanZero(const CString &cadena, bool force) {

	CString szCadena= cadena;
	if(AfxGetApp()->GetProfileInt(AfxGetApp()->GetProfileString("almaceno", "clave", "almaceno"),"showzero",0))
		if( !force)
			return szCadena;
	int pos= szCadena.Find(".");
	if( pos!=-1) {
		for( int i=szCadena.GetLength()-1; i>pos; i--) {
			if( szCadena[i]=='0')
				szCadena.Delete(i);
			else break;
		}
		if( szCadena[szCadena.GetLength()-1]=='.')
			szCadena.Delete(szCadena.GetLength()-1);
	}
	return szCadena;
}

CString CEditFloat::TrimDecimales(double numero) {
	
	int decimales=AfxGetApp()->GetProfileInt(AfxGetApp()->GetProfileString("almaceno", "clave", "almaceno"), "precision", 2);
	
	if( AfxGetApp()->GetProfileInt(AfxGetApp()->GetProfileString("almaceno", "clave", "almaceno"), "windows", 1)) {
		char signod[4];
		signod[0]=0;
		GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_IDIGITS, signod, 4);
		decimales= atoi(signod);
	}
	CString formato,resultado;
	formato.Format("%%.%dlf", decimales);
	resultado.Format(formato, numero);
	return CleanZero(resultado);
}

void CEditFloat::GetLocale(char *decimal, char *miles) {

	char signod[4];
	strcpy(signod, AfxGetApp()->GetProfileString(AfxGetApp()->GetProfileString("almaceno", "clave", "almaceno"), "sepdec", ","));
	if( AfxGetApp()->GetProfileInt(AfxGetApp()->GetProfileString("almaceno", "clave", "almaceno"), "windows", 1)) 
		GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SDECIMAL, signod, 4);
	strcpy(decimal,signod);
    strcpy(signod,AfxGetApp()->GetProfileString(AfxGetApp()->GetProfileString("almaceno", "clave", "almaceno"), "sepmil", ","));
	if( AfxGetApp()->GetProfileInt(AfxGetApp()->GetProfileString("almaceno", "clave", "almaceno"), "windows", 1))
		GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_STHOUSAND, signod, 4);
	strcpy(miles, signod);
}

void CEditFloat::SetNumeroEx() {
	
	CString temp;
	GetWindowText(temp);
    SetNumeroEx(temp);
}

void CEditFloat::SetNumeroEx(const CString &numero) {

	CString szCadena=numero;
	m_dNumero= atof(szCadena);
}

CString	CEditFloat::GetNormal(const CString &numero) {

	CString szCadena=numero;

	char dec[4], mil[4];
	GetLocale(dec, mil);
	CString temp= AfxGetApp()->GetProfileString(AfxGetApp()->GetProfileString("almaceno", "clave", "almaceno"), "sepmill", "'");
	szCadena.Remove(temp[0]);
	szCadena.Remove(mil[0]);
	szCadena.Replace(dec, ".");
	return szCadena;
}

CString CEditFloat::GetFormatNumber(const CString &numero) {

	char temp[1024];
	GetNumberFormat(LOCALE_USER_DEFAULT, NULL,numero, NULL, temp, 1024);
	return temp;
}

CString  CEditFloat::GetFormatCurrency(const CString &numero) {
	
	char temp[1024];
	GetCurrencyFormat(LOCALE_USER_DEFAULT, NULL,numero, NULL, temp, 1024);
	return temp;
}

CString  CEditFloat::GetFormatCurrency(double numero) {

	CString temp;
	temp.Format("%.20lf", numero);
	return GetFormatCurrency(temp);
}

CString  CEditFloat::GetFormatNumber(double numero) {

	CString temp;
	temp.Format("%.20lf", numero);
	return GetFormatNumber(temp);
}

void CEditFloat::SetNumero(const CString &numero) {
	
	CString szCadena= numero;
	SetNumeroEx(szCadena);
	szCadena.Format("%lf", m_dNumero);
	CEditXP::SetWindowText(GetFormatNumber(szCadena));
}

void CEditFloat::SetNumero(double numero) {

	CString szNumero;
	szNumero.Format("%lf", numero);
	SetNumero(szNumero);
}

void CEditFloat::SetFormatNumero(const CString &numero) {
	
	SetNumero(GetNormal(numero));
}

CString CEditFloat::GetNumero() {
	
	CString szNumero;
	if( m_bFocus)
		SetNumeroEx();
	if(m_dNumero)
		szNumero.Format("%lf", m_dNumero);
	return szNumero;
}

void CEditFloat::OnKillFocus(CWnd* pNewWnd)
{
	m_bFocus=false;
	CString temp;
	GetWindowText(temp);
	SetNumero(GetNormal(temp));
	if(m_dNumero==0)
		Text="";
	CEditXP::OnKillFocus(pNewWnd);
}

CString CEditFloat::GetCleanNumero() {

	CString szNumero;
	if( m_bFocus)
		SetNumeroEx();
	if(m_dNumero) {
		szNumero.Format("%lf", m_dNumero);
		szNumero= CleanZero(szNumero,true);
	}
	szNumero.Replace(".", m_szDecimal);
	return szNumero;
}

void CEditFloat::OnSetFocus(CWnd* pOldWnd)
{
	SetWindowText(GetCleanNumero());
	SetSel(0, GetWindowTextLength());
	CEditXP::OnSetFocus(pOldWnd);
	m_bFocus=true;
}

void CEditFloat::EnableMinus(bool enable/*=true*/) {

	m_bEnableMinus=enable;
}

void CEditFloat::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	m_Tip.Hide();
	if( GetStyle() & ES_READONLY)
		return;
	if( (nChar>='0' && nChar<='9') || nChar==m_szDecimal[0] || nChar==VK_BACK || nChar==VK_RETURN || nChar==VK_TAB) {

		bool found=false;
		if( nChar==m_szDecimal[0]) {
			CString texto;
			GetWindowText(texto);
			if( texto.Find(m_szDecimal)!=-1) {
				found=true;
			}
		}
		if(!found) {
			CEditXP::OnChar(nChar, nRepCnt, nFlags);
			return;
		}
		//CEditXP::OnChar(nChar, nRepCnt, nFlags);
		//return;
	} else if(nChar=='.') {
		CString texto;
		GetWindowText(texto);
		if( texto.Find(m_szDecimal)==-1) {
			ReplaceSel(m_szDecimal, true);
			return;
		}
	} else if(m_bEnableMinus && nChar=='-') {

		CString texto;
		GetWindowText(texto);
		if(texto.Find('-')==-1) {
			int pos = GetSel();
			if(pos==0) {
				ReplaceSel("-", true);
				return;
			}
		}
	}
	SetToolTip("Sólo puede escribir un número aquí","Cáracter no adminitido");
}

void CEditFloat::SetWindowText(LPCTSTR lpszString) {
	
	CString texto= lpszString;
	SetNumeroEx(texto);
	CEditXP::SetWindowText(lpszString);
}

void CEditFloat::SetText(const CString &texto) {
	
	SetNumeroEx(texto);
	CEditXP::SetText(texto);
}


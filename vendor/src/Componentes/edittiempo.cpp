// EditTiempo.cpp : implementation file
//

#include "stdafx.h"
# include "Componentes.h"
#include "EditTiempo.h"

// CEditTiempo

IMPLEMENT_DYNAMIC(CEditTiempo, CEditXP)
CEditTiempo::CEditTiempo()
{
	FUNCTION_STACK_TRACE("FFFFE02C");

	m_DTMaximo= NULL;
	m_DTMinimo= NULL;

	m_bHora= false;
	m_bLocked= false;
	m_nMaxChar= 0;
	m_nCurrentChar= 0;
	m_szBase= _T("");
}

CEditTiempo::~CEditTiempo()
{
	if(m_DTMinimo)
		delete m_DTMinimo;
	if(m_DTMaximo)
		delete m_DTMaximo;
}


BEGIN_MESSAGE_MAP(CEditTiempo, CEditXP)
	ON_CONTROL_REFLECT(EN_SETFOCUS, OnEnSetfocus)
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_CONTROL_REFLECT(EN_CHANGE, OnEnChange)
	ON_WM_KILLFOCUS()
	ON_MESSAGE(WM_CUT, OnCut)
	ON_MESSAGE(WM_CLEAR, OnCut)
END_MESSAGE_MAP()

void  CEditTiempo::SetMinimo(COleDateTime *tiempo/*=NULL*/) {

	if(m_DTMinimo)
		delete m_DTMinimo;
	m_DTMinimo= NULL;
	if(!tiempo || tiempo->GetStatus()!=COleDateTime::valid)
		return;
	if(m_DTMaximo && *m_DTMaximo<*tiempo) {
		ASSERT(false);
		SetMinimo(m_DTMaximo);
		SetMaximo(tiempo);
		return;
	}
	m_DTMinimo= new COleDateTime();
	*m_DTMinimo=*tiempo;
	if(!m_bLocked)
		ValidateFecha();
}
void  CEditTiempo::SetMinimo(CTime *tiempo) {

	if(tiempo) {
		CMyTime oletime;
		oletime=*tiempo;
		SetMinimo(&oletime);
	} else SetMinimo();
}
void  CEditTiempo::SetMaximo(COleDateTime *tiempo/*=NULL*/) {

	if(m_DTMaximo)
		delete m_DTMaximo;
	m_DTMaximo= NULL;
	if(!tiempo || tiempo->GetStatus()!=COleDateTime::valid)
		return;
	if(m_DTMinimo && *m_DTMinimo>*tiempo) {
		ASSERT(false);
		SetMaximo(m_DTMinimo);
		SetMinimo(tiempo);
		return;
	}
	m_DTMaximo= new COleDateTime();
	*m_DTMaximo=*tiempo;
	if(!m_bLocked)
		ValidateFecha();
}

void  CEditTiempo::SetMaximo(CTime *tiempo) {

	if(tiempo) {
		CMyTime oletime;
		oletime=*tiempo;
		SetMaximo(&oletime);
	} else SetMaximo();
}

void CEditTiempo::SetDateTime(COleDateTime *tiempo) {
	
	CString temp;
	m_bHora=true;
	m_bLocked=false;
	if(tiempo) {
		temp= tiempo->Format("%d/%m/%y" );
		temp+=tiempo->Format(" %H:%M");
	} else {
		temp= "00/00/00";
		temp+= " 00:00";
	}
	m_nMaxChar= temp.GetLength();
	SetLimitText( temp.GetLength());
	SetWindowText( temp);
	m_szBase=temp;
}

void CEditTiempo::SetDateTime(CTime *tiempo) {

	if(tiempo) {
		CMyTime oletime;
		oletime=*tiempo;
		SetDateTime(&oletime);
	} else SetDateTime();
}

void CEditTiempo::SetDate(COleDateTime *tiempo) {
	
	CString temp;
	m_bHora=false;
	m_bLocked=false;

	if(tiempo) {
		temp= tiempo->Format("%d/%m/%y" );
	} else {
		temp= "00/00/00";
	}
	m_nMaxChar= temp.GetLength();
	SetLimitText( temp.GetLength());
	SetWindowText( temp);
	m_szBase=temp;
}

void CEditTiempo::SetDate(CTime *tiempo) {

	if(tiempo) {
		CMyTime oletime;
		oletime=*tiempo;
		SetDate(&oletime);
	} else SetDate();
}
// CEditTiempo message handlers

bool CEditTiempo::GetDateTime() {

	COleDateTime tiempo;
	return GetDateTime(tiempo);
}

CString CEditTiempo::GetFormatoMySQL() {
	
	COleDateTime tiempo;
	if(!GetDateTime(tiempo))
		return NULL;
	CMyTime mytiempo(tiempo);
	return mytiempo.GetFormatoMysql(m_bHora);
}

bool CEditTiempo::GetDateTime(COleDateTime &tiempo) {
	
	if(!m_bLocked)
		ValidateFecha();
	COleDateTime oletiempo;
	CString temp;
	GetWindowText(temp);
	if(!oletiempo.ParseDateTime(temp)) {
		return false;
	}
	tiempo= oletiempo;
	return true;
}

bool CEditTiempo::GetDateTime(CTime &tiempo) {
	COleDateTime oletiempo;
	if(!GetDateTime(oletiempo))
		return false;
	tiempo= CMyTime(oletiempo);
	return true;
}

void CEditTiempo::OnEnSetfocus()
{
	FUNCTION_STACK_TRACE("FFFFE02F");
	m_nCurrentChar=0;
	SetSel(m_nCurrentChar, m_nCurrentChar);
}

int GetMaxDay( int mes, int ano) {

	FUNCTION_STACK_TRACE("FFFFE030");
	switch( mes) {

		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
			return 31;
		case 4:
		case 6:
		case 9:
		case 11:
			return 30;
		case 2:
			if( (ano%4)==0)
				return 29;
			return 28;
	}
	return 0;
}

bool CEditTiempo::GetMaximo(COleDateTime &tiempo) {
	if(!m_DTMaximo)
		return false;
	tiempo= *m_DTMaximo;
	return true;
}

bool CEditTiempo::GetMaximo(CTime &tiempo) {

	if(!m_DTMaximo)
		return false;
	COleDateTime oletiempo;
	GetMaximo(oletiempo);
	tiempo= CMyTime(oletiempo);
	return true;
}

bool CEditTiempo::GetMinimo(COleDateTime &tiempo) {
	if(!m_DTMinimo)
		return false;
	tiempo= *m_DTMinimo;
	return true;
}

bool CEditTiempo::GetMinimo(CTime &tiempo) {

	if(!m_DTMinimo)
		return false;
	COleDateTime oletiempo;
	GetMinimo(oletiempo);
	tiempo= CMyTime(oletiempo);
	return true;
}

void CEditTiempo::ValidateFecha() {

	FUNCTION_STACK_TRACE("FFFFE031");
	if( m_bLocked)
		return;
	CString temp;
	CString convert;
	int hora=0, minutos=0;

	GetWindowText( temp);
	if(temp.IsEmpty())
		return;
	convert= temp.GetAt(0);
	convert+= temp.GetAt(1);
	int dia =atoi(convert);
	convert= temp.GetAt(3);
	convert+= temp.GetAt(4);
	int mes= atoi(convert);
	convert= temp.GetAt(6);
	convert+= temp.GetAt(7);
	int ano= atoi(convert);
	
	if( !dia && !mes && !ano) // nada que validar
		return;

	if( m_bHora) {

		convert= temp.GetAt(9);
		convert+= temp.GetAt(10);
		hora= atoi(convert);
		if( hora>23) hora=23;
		convert= temp.GetAt(12);
		convert+= temp.GetAt(13);
		minutos= atoi(convert);
		if( minutos>59) minutos=23;
	}
	if( mes>12) mes=12;
	int maxday= GetMaxDay( mes, ano);
	if( dia> maxday) dia=maxday;
	if( dia<1) dia=m_DTMaximo ? m_DTMaximo->GetDay() : 1;
	if( mes<1) mes=m_DTMaximo ? m_DTMaximo->GetMonth() : 1;
	if( ano<1) ano= m_DTMaximo ? m_DTMaximo->GetYear() : 06;

	COleDateTime final(ano+2000, mes, dia, hora, minutos, 0);
	if(m_DTMaximo && final>(*m_DTMaximo))
		final= *m_DTMaximo;
	if(m_DTMinimo && final<(*m_DTMinimo))
		final= *m_DTMinimo;
	temp= "%d/%m/%y";
	if( m_bHora) temp+=" %H:%M";
	SetWindowText(final.Format(temp));
}

void CEditTiempo::SetTimeHoraMysql( CString fecha, bool hora, bool lock) {

	FUNCTION_STACK_TRACE("FFFFE034");
	
	CString ano, mes, dia;
	CString myfecha;
	if( fecha.IsEmpty()) {

		myfecha= "00/00/00";
		if( hora) 
			myfecha+= " 00:00";
		
	} else {
	
		m_bLocked= lock;
		COleDateTime tiempo= CMyTime(fecha);
		myfecha= tiempo.Format("%d/%m/%y");

		if( hora) {
			// ano es la hora
			ano= fecha.GetAt(11);
			ano+= fecha.GetAt(12);
			// mes son los minutos
			mes= fecha.GetAt(14);
			mes+= fecha.GetAt(15);
			myfecha+= tiempo.Format(" %H:%M");
		}
	}
	SetWindowText( myfecha);
}

void CEditTiempo::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	FUNCTION_STACK_TRACE("FFFFE035");
	if( GetStyle() & ES_READONLY)
		return;
	CString szReplaceText;
	switch( nChar) {

		case VK_BACK:
			 if (m_nCurrentChar > 0)
             {
				 m_nCurrentChar--;
				 ValidPos(false);
				 SetSel( m_nCurrentChar, m_nCurrentChar+1);
				 szReplaceText += m_szBase.GetAt(m_nCurrentChar);
				 ReplaceSel(szReplaceText);
                 SetSel( m_nCurrentChar, m_nCurrentChar);
             }
             break;
		default:
			{
			  szReplaceText = "";
			  CString temp;
			  GetWindowText( temp);
			  int max, min;
			  max= '9';
			  min= '0';
			  switch( m_nCurrentChar) {

					case 0:
						max='3';
						break;
					case 1:
						if( temp.GetAt(0)=='3')
							max='1';
						break;
					case 3:
						max='1';
						break;
					case 4:
						if( temp.GetAt(3)=='1') 
							max='2';
						break;
					case 9:
						max='2';
						break;
					case 10:
						if( temp.GetAt(11)=='2')
							max='3';
						break;
					case 12:
						max='5';
						break;
			  }
			  if( m_nCurrentChar>= 0 && m_nCurrentChar<m_nMaxChar) {
                  szReplaceText="";
				  if( nChar<min || nChar>max) {

					  break;
				  } else 
					  szReplaceText.Format("%c", nChar);
				  SetSel( m_nCurrentChar, m_nCurrentChar+1);
				  ReplaceSel( szReplaceText);
				  m_nCurrentChar++;
				  ValidPos();
				  SetSel( m_nCurrentChar, m_nCurrentChar);
				  m_bLocked= false;
			  }
			}
	}
}

void CEditTiempo::ValidPos(bool forward) {

	FUNCTION_STACK_TRACE("FFFFE036");
	int paso=-1;
	if( forward) paso= 1;
	switch( m_nCurrentChar) {
		case 14:
			ValidateFecha();
			m_nCurrentChar+= paso;
			break;
		case 8:
			ValidateFecha();
		case 11:
		case 2:
		case 5:
			m_nCurrentChar+= paso;
			break;
	}
}

void CEditTiempo::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	FUNCTION_STACK_TRACE("FFFFE037");
	switch (nChar)
    {
    case VK_TAB:
		(((CWnd*)GetParent())->GetNextDlgTabItem(this, FALSE))->SetFocus();
        break;
    case VK_LEFT:
		if( m_nCurrentChar)
			m_nCurrentChar--;
		ValidPos(false);
		SetSel( m_nCurrentChar, m_nCurrentChar);
		break;
    case VK_RIGHT:
		if( m_nCurrentChar<m_nMaxChar)
			m_nCurrentChar++;
		ValidPos();
		SetSel( m_nCurrentChar, m_nCurrentChar);
		break;
    case VK_DELETE:
		if( GetStyle() & ES_READONLY)
			break;
		if( m_nCurrentChar<m_nMaxChar) {

			CString replace;
			replace+= m_szBase.GetAt(m_nCurrentChar);
			SetSel(m_nCurrentChar, m_nCurrentChar+ 1);
			ReplaceSel(replace);
			m_nCurrentChar++;
			ValidPos();
			SetSel(m_nCurrentChar, m_nCurrentChar);
		}
		break;
    default:
		break;
    }
}

void CEditTiempo::OnLButtonUp(UINT nFlags, CPoint point)
{
	FUNCTION_STACK_TRACE("FFFFE038");
	int x, y;
	GetSel( x, y);
	if( x==y) 
		m_nCurrentChar= x;
	ValidPos();
	SetSel(m_nCurrentChar, m_nCurrentChar);
	CEditXP::OnLButtonUp(nFlags, point);
}

void CEditTiempo::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	FUNCTION_STACK_TRACE("FFFFE039");
	ValidPos();
	SetSel(m_nCurrentChar, m_nCurrentChar);
}

void CEditTiempo::OnEnChange()
{
	FUNCTION_STACK_TRACE("FFFFE03A");
	CString temp;
	GetWindowText( temp);
	if( temp.GetLength()!=m_nMaxChar) {

		SetWindowText(m_szBase);
		ValidPos();
		SetSel( m_nCurrentChar, m_nCurrentChar);
	}
}

LRESULT CEditTiempo::OnCut(WPARAM Wparam, LPARAM LParam) {

	return 0;
}

void CEditTiempo::OnKillFocus(CWnd* pNewWnd)
{
	FUNCTION_STACK_TRACE("FFFFE03B");
	CEditXP::OnKillFocus(pNewWnd);
	if( GetStyle() & ES_READONLY)
		return;
	ValidateFecha();
}

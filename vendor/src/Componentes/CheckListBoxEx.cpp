// F:\Documents and Settings\JuHU\Escritorio\sit\src\SIT\Tools\CheckListBoxEx.cpp : implementation file
//

#include "stdafx.h"
# include "Componentes.h"
#include "CheckListBoxEx.h"
# include "xmlreader.h"


// CCheckListBoxEx

IMPLEMENT_DYNAMIC(CCheckListBoxEx, CCheckListBox)
CCheckListBoxEx::CCheckListBoxEx()
{
	m_bSaltoTab= false;
}

CCheckListBoxEx::~CCheckListBoxEx()
{
}


BEGIN_MESSAGE_MAP(CCheckListBoxEx, CCheckListBox)
	ON_WM_SETFOCUS()
	ON_WM_CHAR()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CCheckListBoxEx message handlers

void CCheckListBoxEx::LoadDataFromXML( CString Archivo) {

	FUNCTION_STACK_TRACE("FFFFE012");
	XmlReader r;
	int pos=-1;
# ifdef DESARROLLO
	r.ShowMsgBoxOnError();
# endif	
	r.Open(Archivo);
	while( r.ReadString()) {

		switch( r.GetNodeType()) {
			case NODETYPE_ATTRIB:
				{
				CString szName, szValue;
				r.GetAttrib(szName, szValue);
				szName = GetUnescapedXmlString(szName);
				szValue = GetUnescapedXmlString(szValue);

				CString szCurrentElement;
				r.GetNodeName(szCurrentElement);
				if( szCurrentElement.CompareNoCase( "item")==0) {

					if(szName.CompareNoCase("nombre")==0)

						pos= AddString( szValue);
					else if( szName.CompareNoCase( "data")==0) 

						SetItemData( pos, atol(szValue));
				}
				}
		}
	}
	r.Close();
}

void CCheckListBoxEx::OnSetFocus(CWnd* pOldWnd)
{
	CCheckListBox::OnSetFocus(pOldWnd);
	ValidateScroll(this);
}

BOOL CCheckListBoxEx::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->message==WM_KEYDOWN) {
		switch( pMsg->wParam) {

			case VK_TAB:
				if( m_bSaltoTab)
					if( GetKeyState(VK_SHIFT)>>8 == -1) {
					if( GetCurSel()!=0)
						pMsg->wParam=  VK_UP;
					} else if( GetCurSel()<GetCount()-1)
					pMsg->wParam=  VK_DOWN;
				break;
		}
	} else if(pMsg->message==WM_KEYUP && pMsg->wParam==VK_ESCAPE) {
		 GetParent()->GetNextDlgTabItem(this, true)->SetFocus();
	}
	return CCheckListBox::PreTranslateMessage(pMsg);
}

void CCheckListBoxEx::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	KillTimer(100);
	if( isalpha(nChar)) {

		m_szUsuario+=(char)nChar;
		int pos= FindString(GetCurSel(), m_szUsuario);
		if( pos!=LB_ERR) {
			SetTimer(100, 500, NULL);
			SetCurSel(pos);
			return;
		}
		else m_szUsuario.Empty();
	}
	CCheckListBox::OnChar(nChar, nRepCnt, nFlags);
}

void CCheckListBoxEx::OnTimer(UINT nIDEvent)
{
	if( nIDEvent==100) {

		m_szUsuario.Empty();
		KillTimer(100);
	}
	CCheckListBox::OnTimer(nIDEvent);
}

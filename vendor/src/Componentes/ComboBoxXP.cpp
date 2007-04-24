///////////////////////////////////////////////////////////////////////////////
//
// ComboBoxXP.cpp : implementation file
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
# include "Componentes.h"
#include "ComboBoxXP.h"
#include "Draw.h"
# include "xmlreader.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CComboBoxXP, CComboBox)

////////////////////////////////////////////////////////////////////////////////
CComboBoxXP::CComboBoxXP () :
    m_nVisibleLines (7), m_nDefaultHeight (-1), m_nEditHeight (-1)
{
	FUNCTION_STACK_TRACE("FFFFE013");
	m_bValidateScroll= true;
	m_bAutoComplete= true;
	m_bShowCombo= AfxGetApp()->GetProfileInt("combo", "showcombo",1)==1;
	m_bRealAutocomplete= true;
	m_bEnable= true;

	m_nSelOK=0;
	m_nMax=0;
	m_nCurrent=0;
	m_nStep=1;
}

void CComboBoxXP::SetAutoComplete(bool bAutocompletar) {

	FUNCTION_STACK_TRACE("FFFFE014");
	m_bRealAutocomplete= bAutocompletar;
}

void CComboBoxXP::ValidateScroll(bool validar) {

	m_bValidateScroll= validar;
}
///////////////////////////////////////////////////////////////////////////////
BOOL CComboBoxXP::Create (DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	FUNCTION_STACK_TRACE("FFFFE015");
	if( dwStyle&WS_DISABLED) {
		dwStyle&=~WS_DISABLED;
		m_bEnable=false;
	}
	dwStyle|=CBS_AUTOHSCROLL;
	// necesario para cuando se crea dinamcamente la clase el presubclassingwindow no genere error.
	m_pToolTip.m_hWnd= (HWND)1;
    if ( !CComboBox::Create (dwStyle, rect, pParentWnd, nID) )
    {
        return false;
    }
    SetFont (CFont::FromHandle ((HFONT)::GetStockObject (DEFAULT_GUI_FONT)));
    m_MouseMgr.Init (m_hWnd);

    HWND hEdit = ::GetWindow (m_hWnd, GW_CHILD);

    if ( hEdit != NULL )
    {
        m_Edit.SubclassWindow (hEdit);
		m_Edit.DrawBordes(false);
		m_Edit.SetReadOnly(!m_bEnable);
    }

	m_pToolTip.m_hWnd=NULL;
	m_pToolTip.Create(this, false);
    return true;
}

bool CComboBoxXP::SetSelData(DWORD data, bool first) {

	return SetSelDataEx(data,first)!=-1;
}

int	CComboBoxXP::GetCurSelEx() {
    
	return GetCurSel();
}

int	CComboBoxXP::SetSelDataEx(DWORD data, bool first) {

	FUNCTION_STACK_TRACE("FFFFE016"); 

	int real= -1;

	for( int i=0; i<GetCount(); i++) {

		if( GetItemData(i)==data) {
			if( first || real!=-1) {
				SetCurSel(i);
				GetParent()->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), CBN_SELCHANGE), (LPARAM) m_hWnd);
				GetParent()->SendMessage(WM_REALUPDATECOMBO,GetDlgCtrlID(), i);
				return i;
			} else real=i;
		}
	}
	if( real!=-1) {
		SetCurSel(real);
		GetParent()->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), CBN_SELCHANGE), (LPARAM) m_hWnd);
		GetParent()->SendMessage(WM_REALUPDATECOMBO,GetDlgCtrlID(), real);
		return real;
	}
	return -1;
}

void CComboBoxXP::LoadDataFromXML( CString Archivo, CString Atributo, CString Valor, bool esacto) {

	FUNCTION_STACK_TRACE("FFFFE017");
	XmlReader r;
	ResetContent();
	r.Open(Archivo);
# ifdef DESARROLLO
	r.ShowMsgBoxOnError();
# endif	
	CString nombre;
	DWORD data=0;

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

						nombre= szValue;
					else if( szName.CompareNoCase("data")==0)

						data= atol(szValue);
					else if( !Atributo.CompareNoCase(szName)) {

						if( !Valor.CompareNoCase(szValue) || szValue=="0") {

							int index= AddString(nombre);
							SetItemData( index, data);
						} else if( !esacto) 

							if( szValue.Find(","+Valor)!=-1 || szValue.Find(Valor+",")!=-1) {

								int index= AddString(nombre);
								SetItemData(index, data);
							}
					}
				}
				}
		}
	}
	r.Close();
}

void CComboBoxXP::LoadDataFromXMLEx( CString Archivo) {

	FUNCTION_STACK_TRACE("FFFFE018");

	XmlReader r;

	r.Open(Archivo);
	r.ShowMsgBoxOnError();
	DWORD data=0;

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

					if(!szValue.IsEmpty() && szName.CompareNoCase("att2")==0) 
                        SetItemData(AddString( szValue), data);
					else if( szName.CompareNoCase("data")==0)
						data= atol(szValue);
				}
				}
		}
	}
	r.Close();
}

void CComboBoxXP::LoadDataFromXML( CString Archivo, bool reset) {

	FUNCTION_STACK_TRACE("FFFFE019");
	XmlReader r;

	if(reset)
		ResetContent();
	r.Open(Archivo);
	r.ShowMsgBoxOnError();
	int index=0;

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

						index= AddString( szValue);
					else if( szName.CompareNoCase("data")==0)

						SetItemData( index, atol(szValue));
				}
				}
		}
	}
	r.Close();
}

////////////////////////////////////////////////////////////////////////////////
void CComboBoxXP::PreSubclassWindow ()
{
	FUNCTION_STACK_TRACE("FFFFE01A");
    m_MouseMgr.Init (m_hWnd);
    HWND hEdit = ::GetWindow (GetSafeHwnd(), GW_CHILD);
    if ( hEdit != NULL )
    {
        m_Edit.SubclassWindow (hEdit);
		m_Edit.DrawBordes(false);
    }
    CWindowRect rc (this);
    SetWindowPos (NULL, 0, 0, rc.Width(), 100, SWP_NOMOVE|SWP_NOZORDER);
	if(m_pToolTip.m_hWnd==NULL)
		m_pToolTip.Create(this, false);
}

///////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CComboBoxXP, CComboBox)
    ON_WM_GETDLGCODE()
    ON_WM_WINDOWPOSCHANGING()
    ON_WM_WINDOWPOSCHANGED()
    ON_WM_PAINT()
    ON_WM_SETFOCUS()
    ON_WM_KILLFOCUS()
    ON_WM_MOUSEMOVE()
    ON_WM_MOUSEOUT()
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_WM_TIMER()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////////////////
BOOL CComboBoxXP::PreTranslateMessage (MSG* pMsg)
{
	FUNCTION_STACK_TRACE("FFFFE01B");
	m_pToolTip.RelayEvent(pMsg);
	static bool escape=false;
    if ( pMsg->message == WM_KEYDOWN )
    {
		m_bAutoComplete = true;
        switch ( pMsg->wParam )
        {
        case VK_RETURN:
			if( ChildOfDockedToolbar (this))
			GetParent()->SendMessage (WM_COMMAND, GetDlgCtrlID());
            break;

        case VK_ESCAPE:
			escape= GetDroppedState()==TRUE;
			if( ChildOfDockedToolbar (this)) {
				if ( GetDroppedState() ) {
					ShowDropDown(false);
				} else
					GetParentFrame()->GetActiveFrame()->SetFocus();
			}
            break;
		case VK_DELETE:
		case VK_BACK:
			{
				m_bAutoComplete = false;
			}
			break;
		case VK_DOWN:
		case VK_UP:
		case VK_PRIOR:
		case VK_NEXT:
			if( !m_bEnable)
				return true;
			break;
        }
	} else if( pMsg->message==WM_KEYUP) {
		if( pMsg->wParam==VK_ESCAPE) {
			if( escape)
				escape=false;
			else {
				CWnd *elemento = GetParent()->GetNextDlgTabItem(this, true);
				elemento->SetFocus();
				if( elemento->IsKindOf(RUNTIME_CLASS(CEdit)))
					((CEdit *)elemento)->SetSel(0, ((CEdit *)elemento)->GetWindowTextLength());
			}
		}
	} else if( pMsg->message==WM_MOUSEWHEEL && !m_bEnable)
		return true;
	if( !m_bEnable) {
		if( pMsg->message==WM_LBUTTONDOWN || pMsg->message==WM_LBUTTONDBLCLK) {
			if(::IsWindow(m_Edit))
				m_Edit.SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
			return true;
		}
	}
    return CComboBox::PreTranslateMessage(pMsg);
}

///////////////////////////////////////////////////////////////////////////////
void CComboBoxXP::OnWindowPosChanging (WINDOWPOS* pWndPos)
{
	FUNCTION_STACK_TRACE("FFFFE01C");
    if ( !(pWndPos->flags & SWP_NOSIZE) && m_nEditHeight == -1 )
    {
        if ( m_nDefaultHeight == -1 )
        {
            m_nDefaultHeight = GetItemHeight (-1);
        }
        m_nEditHeight = pWndPos->cy;

        pWndPos->cy = m_nDefaultHeight + GetItemHeight(0)*m_nVisibleLines + GetSystemMetrics(SM_CYBORDER)*7;
    }
	if(::IsWindow(m_wndTextProgress)) 
		m_wndTextProgress.MoveWindow(1,1, pWndPos->cx-2, pWndPos->cy-2,false);
    CComboBox::OnWindowPosChanging (pWndPos);
}

///////////////////////////////////////////////////////////////////////////////
UINT CComboBoxXP::OnGetDlgCode ()
{
	FUNCTION_STACK_TRACE("FFFFE01D");
    if ( KEYDOWN (VK_RETURN) )
    {
        return DLGC_WANTALLKEYS|CComboBox::OnGetDlgCode();
    }
    return CComboBox::OnGetDlgCode();
}

BOOL CComboBoxXP::IsWindowEnabled() {

	return CComboBox::IsWindowEnabled() && m_bEnable;
}

BOOL CComboBoxXP::EnableWindow(BOOL bEnable) {

	// conservar asi para compatibilidad no usar bEnable==TRUE por que devuelve false
	m_bEnable= bEnable;
	Invalidate();
	if(::IsWindow(m_Edit)) 
		return m_Edit.SetReadOnly(!bEnable);
	return bEnable;
}
///////////////////////////////////////////////////////////////////////////////
void CComboBoxXP::OnWindowPosChanged (WINDOWPOS* pWndPos)
{
	FUNCTION_STACK_TRACE("FFFFE01E");
    CComboBox::OnWindowPosChanged (pWndPos);

    if ( !(pWndPos->flags & SWP_NOSIZE) && m_nEditHeight != -1 )
    {
        SetItemHeight (-1, m_nEditHeight-6);
        m_nEditHeight = -1;
    }
    HWND hEdit = ::GetWindow (m_hWnd, GW_CHILD);

    // Del the selection
    if ( hEdit != NULL && hEdit != ::GetFocus() )
    {
        ::SendMessage (hEdit, EM_SETSEL, (WPARAM)-1, 0);
    }
}

///////////////////////////////////////////////////////////////////////////////
void CComboBoxXP::OnPaint ()
{
	Default();
	if(m_WVersion.IsVistaorLater()) 
		return;

	CClientRect rc (this);
	CRect rcBtn (rc.right-::GetSystemMetrics (SM_CXHTHUMB), rc.top, rc.right, rc.bottom);

	ValidateRect (rcBtn);

	
	CClientDC cDC (this);
	if(!cDC.m_hDC)
		return;
	DWORD wStyle = DB_FLAT|DB_DOWN|(m_bEnable? DB_ENABLED : DB_DISABLED);
	bool bInToolbar = ::ChildOfDockedToolbar (this);
	bool bDropped = DefWindowProc (CB_GETDROPPEDSTATE, 0, 0L) != 0;
	bool bFocus = (::GetFocus() == m_hWnd || ::GetParent (::GetFocus()) == m_hWnd); // && bInToolbar;

	if ( !bInToolbar )
	{
		wStyle |= DB_BORDER;
	}
	if ( bFocus || m_MouseMgr.MouseOver() )
	{
		wStyle |= DB_OVER;
	}
	if ( bDropped )
	{
		wStyle |= DB_OVER|DB_PRESSED;
	}
	CDrawButton (wStyle, rcBtn).Draw (&cDC);
	CPenDC pen (cDC, ::GetSysColor ((( bFocus || bDropped || m_MouseMgr.MouseOver() ) && m_bEnable)
		? COLOR_HIGHLIGHT
		: (bInToolbar ? COLOR_WINDOW : COLOR_3DSHADOW)));

	cDC.SelectObject (::GetStockObject (NULL_BRUSH));
	cDC.Rectangle (rc);

	pen.Color (::GetSysColor (m_bEnable ? COLOR_WINDOW : COLOR_3DFACE));
	rc.InflateRect (-1, -1);
	rc.right -= ::GetSystemMetrics (SM_CXHTHUMB);
	cDC.Rectangle (rc);
	cDC.MoveTo (rc.right, rc.top);
	cDC.LineTo (rc.right, rc.bottom);
	if( !m_bEnable && bFocus) {
		rc.InflateRect(1,1);
		pen.Color(::GetSysColor(COLOR_HIGHLIGHT));
		cDC.Rectangle(rc);
	}
}

///////////////////////////////////////////////////////////////////////////////
void CComboBoxXP::OnSetFocus (CWnd* pOldWnd)
{
	FUNCTION_STACK_TRACE("FFFFE020");
    CComboBox::OnSetFocus (pOldWnd);

	if( m_bValidateScroll)
		::ValidateScroll(this);
    if ( ::ChildOfDockedToolbar (this) )
    {
        Invalidate();
    }
}

///////////////////////////////////////////////////////////////////////////////
void CComboBoxXP::OnKillFocus (CWnd* pNewWnd)
{
	FUNCTION_STACK_TRACE("FFFFE021");
    CComboBox::OnKillFocus (pNewWnd);
    Invalidate();
}

///////////////////////////////////////////////////////////////////////////////
void CComboBoxXP::OnMouseMove (UINT nFlags, CPoint point)
{
	FUNCTION_STACK_TRACE("FFFFE022");

	CRect rectClient;
	GetClientRect(&rectClient);
	int nComboButtonWidth = ::GetSystemMetrics(SM_CXHTHUMB) + 2;
	rectClient.right = rectClient.right - nComboButtonWidth;
	if (rectClient.PtInRect(point))
	{
		ClientToScreen(&rectClient);

		CString strText;
		GetWindowText(strText);

		CDC *pDC= GetDC();
		CFont *oldFont = pDC->SelectObject(GetFont());

		CSize size= pDC->GetTextExtent(strText);
		pDC->SelectObject(oldFont);
		ReleaseDC(pDC);

		if (size.cx > (rectClient.Width() - 6))
		{
			CPoint punto(rectClient.left, rectClient.bottom);
			m_pToolTip.Show(strText, &punto);
			SetTimer(1, 80, NULL);
		}
		else
		{
			m_pToolTip.Hide();
		}
	}
	else
	{
		m_pToolTip.Hide();
	}
	CComboBox::OnMouseMove(nFlags, point);
    m_MouseMgr.OnMouseMove();

}

///////////////////////////////////////////////////////////////////////////////
void CComboBoxXP::OnMouseOut ()
{
	FUNCTION_STACK_TRACE("FFFFE023");
	m_pToolTip.Hide();
    m_MouseMgr.OnMouseOut();
}

///////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CComboEditXP, CEditXP)
    ON_WM_KILLFOCUS()
    ON_WM_MOUSEMOVE()
    ON_WM_MOUSEOUT()
	ON_CONTROL_REFLECT(EN_CHANGE, OnEnChange)
	ON_WM_SETFOCUS()
	ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
void CComboEditXP::OnKillFocus (CWnd* pNewWnd)
{
	FUNCTION_STACK_TRACE("FFFFE024");
    CEditXP::OnKillFocus (pNewWnd);
	GetComboBox().Invalidate();
}

///////////////////////////////////////////////////////////////////////////////
void CComboEditXP::OnMouseMove (UINT, CPoint)
{
	FUNCTION_STACK_TRACE("FFFFE025");
    Default();
    GetComboBox().m_MouseMgr.OnMouseMove (m_hWnd);
}

///////////////////////////////////////////////////////////////////////////////
void CComboEditXP::OnMouseOut ()
{
	FUNCTION_STACK_TRACE("FFFFE026");
    GetComboBox().m_MouseMgr.OnMouseOut (m_hWnd);
}

BOOL CComboBoxXP::OnCommand(WPARAM wParam, LPARAM lParam)
{
	if(HIWORD(wParam)==CBN_SELCHANGE) 
		GetParent()->SendMessage(WM_REALUPDATECOMBO,GetDlgCtrlID(), GetCurSel());
	return CComboBox::OnCommand(wParam, lParam);
}

void CComboBoxXP::OnCbnEditupdate()
{
	FUNCTION_STACK_TRACE("FFFFE028");
	if( !m_bRealAutocomplete)
		return;
	if (!m_bAutoComplete) {

		bool autoc= m_bRealAutocomplete;
		m_bRealAutocomplete=false;

		CString str=Text;

		// Currently selected range
		DWORD dwCurSel = GetEditSel();
		WORD dStart = LOWORD(dwCurSel);
		WORD dEnd   = HIWORD(dwCurSel);

		int seleccion= FindStringExact(-1, str);
//#ifdef  _DEBUG
//		CString temporal;
//		static int contador=0;
//		if(seleccion==-1) { 
//			temporal.Format("Sin autocompletado seleccion actual %d, %d\nCadena '%s' %d\n",seleccion,contador++,str, GetCurSel());
//		} else {
//			CString texto;
//			GetLBText(seleccion,texto);
//			temporal.Format("Sin autocompletado seleccion actual %d, %s\nCadena '%s' %d\n",seleccion,texto,str, GetCurSel());
//		}
//		TRACE(temporal);
//#endif
		SetCurSel(seleccion);
		GetParent()->PostMessage(WM_REALUPDATECOMBO,GetDlgCtrlID(), seleccion);
		GetParent()->PostMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), CBN_SELCHANGE), (LPARAM) m_hWnd);
		if(seleccion==-1) 
			Text= str;
		SetEditSel(dStart, dEnd);
		m_bRealAutocomplete=autoc;
		return;
	}

  // Get the text in the edit box
  CString str;
  GetWindowText(str);
  int nLength = str.GetLength();
  int seleccion;
  
  // Currently selected range
  DWORD dwCurSel = GetEditSel();
  WORD dStart = LOWORD(dwCurSel);
  WORD dEnd   = HIWORD(dwCurSel);

  // Search for, and select in, and string in the combo box that is prefixed
  // by the text in the edit box
  seleccion= SelectString(-1, str);

  GetParent()->PostMessage(WM_REALUPDATECOMBO,GetDlgCtrlID(), seleccion);
  GetParent()->PostMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), CBN_SELCHANGE), (LPARAM) m_hWnd);

//#ifdef  _DEBUG
//  CString temporal;
//  static int contador=0;
//  if(seleccion==-1) { 
//	  temporal.Format("seleccion actual %d, %d\nCadena '%s' %d\n",seleccion,contador++,str, GetCurSel());
//  } else {
//	  CString texto;
//	  GetLBText(seleccion,texto);
//	  temporal.Format("seleccion actual %d, %s\nCadena '%s' %d\n",seleccion,texto,str, GetCurSel());
//  }
//  TRACE(temporal);
//#endif
  
  if( m_bShowCombo) {
	  ShowDropDown(seleccion!=CB_ERR);
	  SetCursor(LoadCursor(NULL, IDC_ARROW));
  }
  if (seleccion== CB_ERR)
  {
	  SetCurSel(seleccion);
	  CComboBox::SetWindowText(str);		// No text selected, so restore what was there before
      if (dwCurSel != CB_ERR)
        SetEditSel(dStart, dEnd);	//restore cursor postion
  }
  if (dEnd < nLength && dwCurSel != CB_ERR)
	  SetEditSel(dStart, dEnd);
  else
	  SetEditSel(nLength, -1);
//#ifdef  _DEBUG
//  if(seleccion==-1) { 
//	  temporal.Format("despues seleccion actual %d, %d\nCadena '%s' %d\n",seleccion,contador++,str, GetCurSel());
//  } else {
//	  CString texto;
//	  GetLBText(seleccion,texto);
//	  temporal.Format("despues seleccion actual %d, %s\nCadena '%s' %d\n",seleccion,texto,str, GetCurSel());
//  }
//  TRACE(temporal);
//#endif
}

void CComboEditXP::OnEnChange()
{
	FUNCTION_STACK_TRACE("FFFFE029");
	GetComboBox().OnCbnEditupdate();
}

BOOL CComboEditXP::PreTranslateMessage(MSG* pMsg)
{
	GetComboBox().m_pToolTip.RelayEvent(pMsg);
	if( pMsg->message==WM_KEYDOWN) {
		switch( pMsg->wParam) {
			case VK_LEFT:
				{
					int x, y;
					GetSel(x, y);
					if( x==y && x==0) 
						GetComboBox().GetParent()->GetNextDlgTabItem(this, true)->SetFocus();
				}
				break;
			case VK_RIGHT:
				{
					int x, y;
					GetSel(x, y);
					if( x==y && x==GetWindowTextLength()) 
						GetComboBox().GetParent()->GetNextDlgTabItem(this, false)->SetFocus();
				}
				break;
		}
	}
	return CEditXP::PreTranslateMessage(pMsg);
}

void CComboEditXP::OnSetFocus(CWnd* pOldWnd)
{
	CEditXP::OnSetFocus(pOldWnd);
	GetComboBox().Invalidate();
}

HBRUSH CComboEditXP::CtlColor(CDC* pDC, UINT nCtlColor)
{
	if(GetComboBox().m_bEnable)
		return CEditXP::CtlColor(pDC, nCtlColor);
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(::GetSysColor(COLOR_BTNSHADOW));
	return ::GetSysColorBrush(COLOR_3DFACE);
}

void CComboBoxXP::SetWindowText(LPCTSTR lpszString) {

	CComboBox::SetWindowText(lpszString);
	UpdateSeleccion();
	CString temp= lpszString;
	if(temp.IsEmpty())
		Mensaje= Mensaje;
}
// Text
void CComboBoxXP::SetText(const CString &texto) {

	SetWindowText(texto);
}

CString	CComboBoxXP::GetText() {

	CString texto;
	GetWindowText(texto);
	if(texto==Mensaje)
		texto.Empty();
	return texto;
}

void CComboBoxXP::SetMensaje(const CString &mensaje) {

	m_Edit.Mensaje= mensaje;
}

CString  CComboBoxXP::GetMensaje() {

	return m_Edit.Mensaje;
}

void CComboBoxXP::SetSelectedIndex(int index) {

	SetCurSel(index);
	GetParent()->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), CBN_SELCHANGE), (LPARAM) m_hWnd);
	GetParent()->SendMessage(WM_REALUPDATECOMBO,GetDlgCtrlID(), index);
}

int CComboBoxXP::GetSelectedIndex() {

	return GetCurSel();
}

void CComboBoxXP::SetSelectedData(DWORD data) {

	SetSelData(data);
}

DWORD CComboBoxXP::GetSelectedData() {

	int index= SelectedIndex;
	ASSERT(index!=-1);
	return index==-1 ? 0 : GetItemData(index);
}

void CComboBoxXP::SetRange(int minimo, int maximo) {

	if(!::IsWindow(m_wndTextProgress)) {

		CRect rect;
		GetWindowRect(rect);
		ScreenToClient(rect);
		VERIFY(m_wndTextProgress.Create(WS_VISIBLE | WS_CHILD | PBS_SMOOTH, rect, this, 10234));
		m_wndTextProgress.SetWindowPos(&wndTop, 1, 1, rect.Width()-2, rect.Height()-2,SWP_NOACTIVATE|SWP_NOSIZE|SWP_NOMOVE|SWP_NOOWNERZORDER);
		m_wndTextProgress.SetFormato(DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	}
	m_wndTextProgress.SetRange(minimo, maximo);
	m_wndTextProgress.SetWindowText(Text);
	m_nMax= maximo;
	SetPos(minimo);
}

void CComboBoxXP::SetPos(int pos) {

	m_nCurrent= pos;
	m_wndTextProgress.SetPos(pos);
	if(m_nCurrent>=m_nMax) {
		if(::IsWindow(m_wndTextProgress))
			m_wndTextProgress.ShowWindow(SW_HIDE);
	} else if(pos==0) {
		if(GetDroppedState()) 
			ShowDropDown(false);
		m_wndTextProgress.ShowWindow(SW_SHOW);
	}
}

int	CComboBoxXP::GetPos() const {

	return m_nCurrent;
}

int CComboBoxXP::Step() {

	SetPos(m_nCurrent+ m_nStep);
	return m_nCurrent;
}

void CComboBoxXP::SetStep(int step) {

	m_wndTextProgress.SetStep(step);
	m_nStep= step;
}

int CComboBoxXP::GetMaximo() const {

	return m_nMax;
}
void CComboBoxXP::SetProgressText(const CString &mensaje) {

	m_wndTextProgress.SetWindowText(mensaje);
}

CString CComboBoxXP::GetProgressText() {

	CString texto;
	m_wndTextProgress.GetWindowText(texto);
	return texto;
}

CTextProgressCtrl *CComboBoxXP::GetProgressCtrl() {

	return &m_wndTextProgress;
}

void CComboBoxXP::HideProgress() {

	SetPos(m_nMax);
}

void CComboBoxXP::ResetContent(bool cleantext/*=true*/) {

	CString texto= Text;
	DWORD dwCurSel = GetEditSel();
	CComboBox::ResetContent();
	if(!cleantext) {
		Text= texto;
		SetEditSel(LOWORD(dwCurSel), HIWORD(dwCurSel));
	}
}

void CComboBoxXP::UpdateSeleccion() {

	bool old=m_bShowCombo;
	m_bShowCombo=false;
	DWORD dwCurSel = GetEditSel();
#ifdef _DEBUG
	WORD dStart = LOWORD(dwCurSel);
	WORD dEnd   = HIWORD(dwCurSel);
#endif
	OnCbnEditupdate();
	m_bShowCombo= old;
	if(dwCurSel!=CB_ERR)
		SetEditSel(LOWORD(dwCurSel), HIWORD(dwCurSel));
}

HBRUSH CComboBoxXP::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_LISTBOX)
	{
		if (m_listbox.GetSafeHwnd() == NULL)
		{
			m_listbox.SubclassWindow(pWnd->GetSafeHwnd());
		}
	}
	HBRUSH hbr = CComboBox::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr;
}

void CComboBoxXP::OnDestroy()
{
	if (m_listbox.GetSafeHwnd() != NULL) {
		m_listbox.UnsubclassWindow();
		m_listbox.OnDestroy();
	}
	m_pToolTip.DestroyWindow();
	CComboBox::OnDestroy();
}

void CComboBoxXP::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent==1) {
		CPoint point;
		GetCursorPos(&point);
		ScreenToClient(&point);

		CRect rectClient;
		GetClientRect(&rectClient);
		int nComboButtonWidth = ::GetSystemMetrics(SM_CXHTHUMB) + 2;
		rectClient.right = rectClient.right - nComboButtonWidth;
		if (!rectClient.PtInRect(point))
		{
			KillTimer(nIDEvent);
			m_pToolTip.Hide();
		}
	}
	CComboBox::OnTimer(nIDEvent);
}

BOOL CComboBoxXP::OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult)
{
	if(message==WM_COMMAND) {
		if(HIWORD(wParam)==CBN_SELENDCANCEL) {
			m_nSelOK=0;
		} else if(HIWORD(wParam)==CBN_SELENDOK) {
			m_nSelOK++;
		} else if(HIWORD(wParam)==CBN_CLOSEUP) {
			if(m_nSelOK==0) {
				UpdateSeleccion();
			} else m_nSelOK--;
		}
	}
	return CComboBox::OnChildNotify(message, wParam, lParam, pLResult);
	// return notify;
}

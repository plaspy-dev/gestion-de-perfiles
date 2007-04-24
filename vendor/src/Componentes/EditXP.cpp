// EditXP.cpp : implementation file
//

#include "stdafx.h"
# include "Componentes.h"
#include "EditXP.h"
# include "Draw.h"
# include "newmenu/newmenu.h"

// CEditXP

IMPLEMENT_DYNAMIC(CEditXP, CEdit)
CEditXP::CEditXP()
{	
	FUNCTION_STACK_TRACE("FFFFE03C");
	m_crOver= ::GetSysColor(COLOR_INACTIVECAPTION);
	m_crFocus=  ::GetSysColor( COLOR_HIGHLIGHT);
	m_crDefault=  ::GetSysColor(COLOR_3DSHADOW);
	m_bSalto= AfxGetApp()->GetProfileInt("editctrl", "sfinal", 0)==1;
	m_bSaltoEx= AfxGetApp()->GetProfileInt("editctrl", "sflechas", 1)==1;
	m_hlFont= m_hAlto= 0;
	m_bDrawBordes= AfxGetApp()->GetProfileInt("editctrl", "bordes", 1)==1;
	m_nMax=0;
	m_nCurrent=0;
	m_nStep=1;
	m_nAncho= 0;
	m_hIcon=NULL;
	m_bDerecha=true;
}

CEditXP::~CEditXP()
{
}

BEGIN_MESSAGE_MAP(CEditXP, CEdit)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
    ON_WM_MOUSEOUT()
	ON_WM_SYSCOLORCHANGE()
	ON_MESSAGE(WM_PASTE, OnPaste)
	ON_WM_CONTEXTMENU()	
	ON_WM_SETFOCUS()
	ON_WM_DESTROY()
	ON_WM_KILLFOCUS()
	ON_WM_CHAR()
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_WINDOWPOSCHANGING()
	ON_MESSAGE(WM_SETFONT, OnSetFont)
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CEditXP message handlers

void CEditXP::OnSysColorChange() 
{
	FUNCTION_STACK_TRACE("FFFFE03D");
	CEdit::OnSysColorChange();

	m_crOver= ::GetSysColor(COLOR_INACTIVECAPTION);
	m_crFocus=  ::GetSysColor( COLOR_HIGHLIGHT);
	m_crDefault=  ::GetSysColor(COLOR_3DSHADOW);
}

void CEditXP::DrawBordes(bool draw) {
	m_bDrawBordes= draw;
}

void CEditXP::PreSubclassWindow()
{	
	FUNCTION_STACK_TRACE("FFFFE03E");
	m_MouseMgr.Init(m_hWnd);
	UpdateIcon();
	// m_Tip.Create(this);
}

void CEditXP::DrawBordes() {

	if(m_WVersion.IsVistaorLater())
		return;
	FUNCTION_STACK_TRACE("FFFFE03F");
	CRect rect;
	GetWindowRect(&rect);
	ScreenToClient(&rect);
	DWORD dwExStyle=GetStyle();
	COLORREF crDefault= m_crDefault;

	CClientDC cDC (this);
	if(!cDC.m_hDC)
		return;
	CPenDC pen( cDC, GetSysColor(COLOR_3DFACE));
	cDC.SelectObject(::GetStockObject (NULL_BRUSH));

	// borramos los bordes

	int nBorderWidth = 2;

	if (dwExStyle & WS_EX_DLGMODALFRAME) 
		nBorderWidth += 3;
	if (dwExStyle & WS_EX_CLIENTEDGE) 
		nBorderWidth += 2;
	if (dwExStyle & WS_EX_STATICEDGE && !(dwExStyle & WS_EX_DLGMODALFRAME)) 
		nBorderWidth ++;
	rect.InflateRect(1,1);
	for (int nLoop = 0; nLoop <nBorderWidth; nLoop++) {

		cDC.Rectangle(rect);
		rect.DeflateRect(1, 1);
	}
	rect.DeflateRect(1,1);

	if( ::GetFocus()==m_hWnd)
		crDefault= m_crFocus;
	else if( m_MouseMgr.MouseOver())
		crDefault= m_crOver;
	pen.Color( crDefault);
	rect.DeflateRect(-1, -1);
	cDC.Rectangle(rect);
}

void CEditXP::OnPaint()
{
	Default();
	if(m_bDrawBordes)
		DrawBordes();
	if(m_hIcon) {

		CClientDC dc(this);

		int width = GetSystemMetrics( SM_CXSMICON );
		int height = GetSystemMetrics( SM_CYSMICON );

		CRect rect;
		GetClientRect( &rect );
		rect.left+=2;
		rect.top+=2;
		rect.right-=2;
		rect.bottom-=2;
		
		if(m_bDerecha)
			rect.left= rect.right-(width+3);
		else rect.right= rect.left+width +3;

		// Clearing the background
		dc.FillSolidRect( rect, GetSysColor(COLOR_WINDOW) );

		// Drawing the icon
		::DrawIconEx( dc.m_hDC, rect.left, rect.top, m_hIcon, width, height, 0, NULL, DI_NORMAL );
	}
}

////////////////////////////////////////////////////////////////////////////////
void CEditXP::OnMouseMove (UINT, CPoint)
{
	FUNCTION_STACK_TRACE("FFFFE040");
    Default();
    m_MouseMgr.OnMouseMove();
}

///////////////////////////////////////////////////////////////////////////////
void CEditXP::OnMouseOut ()
{
	FUNCTION_STACK_TRACE("FFFFE041");
    m_MouseMgr.OnMouseOut();
}

LRESULT CEditXP::OnPaste(WPARAM Wparam, LPARAM LParam) 
{
	FUNCTION_STACK_TRACE("FFFFE042");
	if( GetStyle() & ES_READONLY)
		return 0;

	CString buffer;
	COleDataObject	obj;

	if (obj.AttachClipboard()) 
      {
		if (obj.IsDataAvailable(CF_TEXT)) 
         {
			HGLOBAL hmem = obj.GetGlobalData(CF_TEXT);
			CMemFile sf((BYTE*) ::GlobalLock(hmem), ::GlobalSize(hmem));
			CString buffer;

			LPSTR str = buffer.GetBufferSetLength(::GlobalSize(hmem));
			sf.Read(str, ::GlobalSize(hmem));
			::GlobalUnlock(hmem);

			buffer.ReleaseBuffer();
			buffer.FreeExtra();

			if( GetStyle() & ES_NUMBER) {

				CString out;
				for( int i=0; i<buffer.GetLength(); i++)
					if( buffer[i]>='0' && buffer[i]<='9')
						out+=buffer[i];
					else break;
				if(!out.IsEmpty())
					ReplaceSel(out, true);
				return 0;
			}
			Default();
         }
      }
   return 0;
}

void CEditXP::OnContextMenu(CWnd* pWnd, CPoint point)
{
	FUNCTION_STACK_TRACE("FFFFE044");
	UNREFERENCED_PARAMETER(pWnd);

  if(point.x==-1 && point.y==-1)
  {
    CRect rect;
    GetWindowRect(rect);

    point = rect.TopLeft()+CPoint(10,10);
  }
  SetFocus();

  CNewMenu menu;
  //CMenu menu;
  menu.CreatePopupMenu();
  menu.InsertMenu(0, MF_BYPOSITION , ID_EDIT_UNDO, GetShortMenu(ID_EDIT_UNDO));
  menu.InsertMenu(1, MF_BYPOSITION|MF_SEPARATOR);
  menu.InsertMenu(2, MF_BYPOSITION , ID_EDIT_COPY, GetShortMenu(ID_EDIT_COPY));
  menu.InsertMenu(2, MF_BYPOSITION , ID_EDIT_CUT, GetShortMenu(ID_EDIT_CUT));
  menu.InsertMenu(4, MF_BYPOSITION , ID_EDIT_CLEAR, GetShortMenu(ID_EDIT_CLEAR));
  menu.InsertMenu(5, MF_BYPOSITION , ID_EDIT_PASTE,  GetShortMenu(ID_EDIT_PASTE));
  menu.InsertMenu(6, MF_BYPOSITION|MF_SEPARATOR);
  menu.InsertMenu(7, MF_BYPOSITION , ID_EDIT_SELECT_ALL,GetShortMenu(ID_EDIT_SELECT_ALL));
  menu.TrackPopupMenu( TPM_RIGHTBUTTON, point.x, point.y, AfxGetMainWnd());
}

void CEditXP::InitMultiline() {

	FUNCTION_STACK_TRACE("FFFFE045");
	CRect rect;
	GetWindowRect(rect);
	m_hAlto= rect.Height();
	CFont *font= GetFont();
	ASSERT(font);
	LOGFONT fuente;
	font->GetLogFont(&fuente);
	m_hlFont= abs(fuente.lfHeight)+2;
	ASSERT(m_hAlto);
}

void CEditXP::ValidarTamano() {

	FUNCTION_STACK_TRACE("FFFFE046");
	if(m_hAlto) {
		CRect rectEdit;
		GetWindowRect(rectEdit);
		int h= (GetLineCount()-1)*m_hlFont+m_hAlto;
		if( rectEdit.Height()!= h) {

			GetParent()->ScreenToClient(rectEdit);
			int delta=rectEdit.bottom;
			rectEdit.bottom= rectEdit.top+ h;
			delta-=rectEdit.bottom;

			// movemos el resto de la ventana.
			CRect rectthis, rect;

			CWnd *parent= GetParent();
			CWnd *item= parent->GetWindow(GW_CHILD);
			GetWindowRect(&rectthis);
			parent->ScreenToClient(&rectthis);

			while(item) {

				if( item==this) {
					item= item->GetWindow(GW_HWNDNEXT);
					continue;
				}
				
				item->GetWindowRect(rect);
				parent->ScreenToClient(rect);

				if(rect.PtInRect(rectthis.BottomRight()) && rect.PtInRect(rectthis.TopLeft())) { // el componente esta dentro de este

					rect.bottom-= delta;
					item->MoveWindow(rect);
					item= item->GetWindow(GW_HWNDNEXT);
					continue;
				}
				if( rect.top < rectthis.bottom) {
					item= item->GetWindow(GW_HWNDNEXT);
					continue;
				}
				rect.MoveToY(rect.top-delta);
				item->MoveWindow(rect);
				item= item->GetWindow(GW_HWNDNEXT);
			}

			// movemos el cuadro de dialogo
			MoveWindow(rectEdit);
			parent->RedrawWindow();
			if(parent->IsKindOf(RUNTIME_CLASS(CFormView))) {
				RecalculateScroll((CFormView *)parent);
			} else if(parent->IsKindOf(RUNTIME_CLASS(CDialog))) {
				parent->GetWindowRect(rect);
				rect.bottom-=delta;
				parent->MoveWindow(rect);
			}
		}
	}
}

BOOL CEditXP::PreTranslateMessage(MSG* pMsg)
{
	if(::IsWindow(m_Tip))
		m_Tip.RelayEvent(pMsg);
	if( pMsg->message==WM_KEYUP) {

		if( pMsg->wParam==VK_ESCAPE) 
			if( !(GetParent()->IsKindOf(RUNTIME_CLASS(CComboBox)) || GetParent()->IsKindOf(RUNTIME_CLASS(CListCtrl)))) {
				CWnd *elemento = GetParent()->GetNextDlgTabItem(this, true);
				elemento->SetFocus();
				if( elemento->IsKindOf(RUNTIME_CLASS(CEdit)))
					((CEdit *)elemento)->SetSel(0, ((CEdit *)elemento)->GetWindowTextLength());
			}
	}
	if( m_bSalto && pMsg->message==WM_KEYUP) {
		int x,y;
		GetSel(x, y);
		if( x==y) {
			y= GetWindowTextLength();
			if( x==y && x==GetLimitText())
				GetParent()->GetNextDlgTabItem(this, FALSE)->SetFocus();
		}
	}
	if(m_bSaltoEx && pMsg->message==WM_KEYDOWN) {

		bool up=false;
		int x,y;
		GetSel(x, y);
		switch( pMsg->wParam) {
			case VK_LEFT:
				if(x || x!=y)
					x=-1;
				up=true;
				break;
			case VK_RIGHT:
				if( x!=GetWindowTextLength() || x!=y)
					x=-1;
				break;
			case VK_UP:
				up=true;
				break;
			case VK_DOWN:
				break;
			default:
				x=-1;
		}
		if( x!=-1) {
			GetParent()->GetNextDlgTabItem(this, up)->SetFocus();
			if( m_hAlto) {
				if( (pMsg->wParam==VK_UP && x<=LineLength(0))  || ( pMsg->wParam==VK_DOWN && y==GetWindowTextLength()))
					return true;
			}
		}
	} 
	if( m_hAlto) {
		bool salir=false;
		ASSERT(GetStyle() & ES_MULTILINE);
		
		switch(pMsg->message) {
			case WM_KEYDOWN:
				if( pMsg->wParam==VK_RETURN && !(GetStyle()&ES_READONLY)) {
					CString temp;
					GetWindowText(temp);
					temp+="\r\n";
					SetWindowText(temp);
					SetSel(temp.GetLength(), temp.GetLength());
					salir=true;
				}
			case WM_SYSCHAR:
			case WM_SYSKEYDOWN:
			case WM_SYSCOMMAND:
			case WM_SYSKEYUP:
				ValidarTamano();
		}
		if(salir)
			return true;
	}
	return CEdit::PreTranslateMessage(pMsg);
}


void CEditXP::OnSetFocus(CWnd* pOldWnd)
{
	CString temp;
	CEdit::GetWindowText(temp);
	if( temp==m_szMensaje) {
		CEdit::SetWindowText("");
	}
	CEdit::OnSetFocus(pOldWnd);
	ValidateScroll(this);
}

void CEditXP::OnDestroy()
{
	CEdit::OnDestroy();
}

void CEditXP::SetToolTip(CString texto, CString titulo) {

	int x, y;
	GetSel(x, y);
	CPoint punto= PosFromChar(x);
	ClientToScreen(&punto);
	punto.y+=5;
	if(x==y && x==0)
		punto.x+=5;
	MessageBeep(MB_OK);
	if(!::IsWindow(m_Tip))
		if(!m_Tip.Create(this))
			return;
	m_Tip.Show(texto, &punto, titulo, TTI_ERROR);
}

void CEditXP::OnKillFocus(CWnd* pNewWnd) {

	SetMensajeEx(false);
	CEdit::OnKillFocus(pNewWnd);
	if(::IsWindow(m_Tip))
		m_Tip.Hide();
}

void CEditXP::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) {

	if(::IsWindow(m_Tip))
		m_Tip.Hide();
	CEdit::OnChar(nChar, nRepCnt, nFlags);
	ValidarTamano();
	ValidateWidth();
}

void CEditXP::GetWindowText(CString &rString) {

	CEdit::GetWindowText(rString);
	if( rString==m_szMensaje)
		rString="";
}

void CEditXP::SetWindowText(LPCTSTR lpszString) {

	CString texto= lpszString;
	if( texto.IsEmpty())
		SetMensajeEx(true);
	else CEdit::SetWindowText(lpszString);
	ValidateWidth();
	ValidarTamano();
}

void CEditXP::SetMensaje(const CString &mensaje) {

	if(Text.IsEmpty()) 
		CEdit::SetWindowText("");
	m_szMensaje= mensaje;
	SetMensajeEx(false);
}

void CEditXP::SetMensajeEx(bool Limpiar) {

	CString temp;
	CEdit::GetWindowText(temp);
	if( Limpiar && GetFocus()==this) {
		CEdit::SetWindowText("");
	} else if( Limpiar || (temp.IsEmpty() && (::GetFocus()!=GetSafeHwnd()))) {
		CEdit::SetWindowText(m_szMensaje);
	} else if(Text.IsEmpty()) {
		CEdit::SetWindowText(m_szMensaje);
	}
}

CString CEditXP::GetMensaje() {

	return m_szMensaje;
}


HBRUSH CEditXP::CtlColor(CDC* pDC, UINT nCtlColor)
{
	if(nCtlColor != CTLCOLOR_EDIT)
		return NULL;
	if(!pDC || !pDC->m_hDC)
		return ::GetSysColorBrush(COLOR_WINDOW);
	CString texto;
	GetWindowText(texto);
	if(::GetFocus() == m_hWnd || !texto.IsEmpty())
		pDC->SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
	else
		pDC->SetTextColor(::GetSysColor(COLOR_BTNSHADOW));
	return ::GetSysColorBrush(COLOR_WINDOW);
}

// propiedades
void CEditXP::SetText(const CString &texto) {

	SetWindowText(texto);
}

CString CEditXP::GetText() {

	CString texto;
	GetWindowText(texto);
	return texto;
}

void CEditXP::SetRange(int minimo, int maximo) {

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

void CEditXP::SetPos(int pos) {

	m_nCurrent= pos;
	m_wndTextProgress.SetPos(pos);
	if(m_nCurrent>=m_nMax) {
		if(::IsWindow(m_wndTextProgress))
			m_wndTextProgress.ShowWindow(SW_HIDE);
	} else if(pos==0) {
		if(::IsWindow(m_wndTextProgress))
			m_wndTextProgress.ShowWindow(SW_SHOW);
	}
}

int	CEditXP::GetPos() const {

	return m_nCurrent;
}

int CEditXP::Step() {

	SetPos(m_nCurrent+ m_nStep);
	return m_nCurrent;
}

void CEditXP::SetStep(int step) {

	m_wndTextProgress.SetStep(step);
	m_nStep= step;
}

int CEditXP::GetMaximo() const {

	return m_nMax;
}

void CEditXP::OnWindowPosChanging (WINDOWPOS* pWndPos)
{
	if(::IsWindow(m_wndTextProgress)) 
		m_wndTextProgress.MoveWindow(1,1, pWndPos->cx-2, pWndPos->cy-2,false);
	CEdit::OnWindowPosChanging (pWndPos);
}

void CEditXP::SetProgressText(const CString &mensaje) {

	m_wndTextProgress.SetWindowText(mensaje);
}

CString CEditXP::GetProgressText() {

	CString texto;
	m_wndTextProgress.GetWindowText(texto);
	return texto;
}

CTextProgressCtrl *CEditXP::GetProgressCtrl() {

	return &m_wndTextProgress;
}

void CEditXP::HideProgress() {

	SetPos(m_nMax);
}

void CEditXP::ValidateWidth() {

	if(m_nAncho) {
		CString strText= Text + "       ";
		
		CRect rectClient;
		GetWindowRect(&rectClient);
		GetParent()->ScreenToClient(&rectClient);

		CDC *pDC= GetDC();
		CFont *oldFont = pDC->SelectObject(GetFont());

		CSize size= pDC->GetTextExtent(strText);
		pDC->SelectObject(oldFont);
		ReleaseDC(pDC);
		if (size.cx > m_nAncho) {
			rectClient.right= rectClient.left + size.cx;
			CRect rectparent;
			GetParent()->GetClientRect(rectparent);
			if(size.cx>=(rectparent.right-rectClient.left)) {				
				rectClient.right= rectClient.left + (rectparent.right-rectClient.left) - 5;
			}
		} else {
			if(rectClient.right==rectClient.left+m_nAncho)
				return;
			rectClient.right=rectClient.left+m_nAncho;
		}
		MoveWindow(rectClient);
	}
}

void CEditXP::InitAutoWidth(bool bAuto/*=true*/) {
	if(bAuto) {
		CRect rect;
		GetWindowRect(rect);
		m_nAncho= rect.Width();
	} else m_nAncho= 0;
}

void CEditXP::SetIcon(HICON hIcon/*=NULL*/,bool derecha/*=true*/) {

	ASSERT( GetStyle() & ES_MULTILINE );
	if(GetStyle() & ES_MULTILINE) {
		m_hIcon= hIcon;
		m_bDerecha=derecha;
		UpdateIcon();
		Invalidate();
	}
}

void CEditXP::SetIcon(UINT nIcon/*=0*/,bool derecha/*=true*/) {

	if(nIcon) {
		HICON icono= ( HICON ) ::LoadImage( AfxGetResourceHandle(),
			MAKEINTRESOURCE( nIcon ),
			IMAGE_ICON,
			16,
			16,
			LR_DEFAULTCOLOR );
		ASSERT( icono != NULL );
		SetIcon(icono,derecha);
	} else SetIcon((HICON)NULL, derecha);
}

void CEditXP::UpdateIcon() {

	if(m_hIcon) {

		CRect editRect;
		int width = GetSystemMetrics( SM_CXSMICON );
		GetClientRect(editRect);
		if(m_bDerecha)
			editRect.right-= width+6;
		else editRect.left += width + 6;
		SetRect( &editRect );
	}
}


LRESULT CEditXP::OnSetFont( WPARAM wParam, LPARAM lParam )
{
	DefWindowProc( WM_SETFONT, wParam, lParam );
	UpdateIcon();
	return 0;
}

void CEditXP::OnSize( UINT nType, int cx, int cy ) 
{
	CEdit::OnSize( nType, cx, cy );
	UpdateIcon();
}

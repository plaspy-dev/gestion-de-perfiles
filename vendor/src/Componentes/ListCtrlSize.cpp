#include "StdAfx.h"
#include "listctrlsize.h"
# include "Componentes.h"

# define TIMER_UPDATE 15

CListCtrlSize::CListCtrlSize(void)
{
	m_sizeMax= CSize(0,0);
	m_sizeMin= CSize(0,0);
	m_sizeDelta= CSize(0,0);
	m_pFin= m_pLabel= NULL;
}

CListCtrlSize::~CListCtrlSize(void)
{
}

BEGIN_MESSAGE_MAP(CListCtrlSize, CMyListCtrl)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


void CListCtrlSize::SetMinSize(CSize size) {
    
	m_sizeMin= size;
}

void CListCtrlSize::SetMaxSize(CSize size) {

	m_sizeMax= size;
}

void CListCtrlSize::SetLabel(CWnd *label) {

	m_pLabel= label;
}

void CListCtrlSize::SetFin(CWnd *fin) {

	m_pFin= fin;
}

void CListCtrlSize::Init() {

	CRect rect;
	GetWindowRect(rect);
	SetMinSize(CSize (rect.Width(), rect.Height()));
	SetMaxSize(CSize(-1,-1));
}

void CListCtrlSize::RealUpdateSize() {
	
	CRect rect,rectthis;
	GetWindowRect(&rectthis);
	GetParent()->ScreenToClient(&rectthis);
	m_sizeDelta.cy= rectthis.bottom;
	m_sizeDelta.cx= rectthis.right;

	GetParent()->SetRedraw(false);
	MoveWindow(rectthis.left, rectthis.top,m_sizeMin.cx, m_sizeMin.cy);
	AutoSizeColumns();

	if( GetItemCount()) {
		GetItemRect(0, &rect, LVIR_BOUNDS);
		int count= GetItemCount()+2;

		count*= rect.Height();
		if(m_sizeMax.cy!=-1 && count>m_sizeMax.cy) 
			count= m_sizeMax.cy;
		if( count< m_sizeMin.cy)
			count= m_sizeMin.cy;
		int ancho=rect.Width()+10;

		if( m_sizeMax.cx!=-1 && ancho>m_sizeMax.cx) 
			ancho= m_sizeMax.cx;
		if( ancho< m_sizeMin.cx)
			ancho= m_sizeMin.cx;
		rectthis.bottom= rectthis.top+count;
		rectthis.right= rectthis.left+ancho;
	} else {
		rectthis.bottom= rectthis.top + m_sizeMin.cy;
		rectthis.right= rectthis.left+ m_sizeMin.cx;
	}
	m_sizeDelta.cy-= rectthis.bottom;
	m_sizeDelta.cx-= rectthis.right;

	MoveWindow(rectthis);
	GetParent()->SetRedraw(true);
	UpdateCtrlItems();
	GetParent()->RedrawWindow();
	for(int i=0; i<m_pArrayControles.GetCount(); i++) {
		CWnd *control= (CWnd *) m_pArrayControles[i];
		if( control && ::IsWindow(*control) && control->IsWindowVisible()) {
			int row, col;
			GetCurrentPos(col, row);
			if(col!=i)
				continue;
			ASSERT(col==i);
			CRect rect;
			EnsureVisible(row,true);
			GetSubItemRect(row,col,LVIR_LABEL,rect);
			control->MoveWindow( rect.left, rect.top, rect.Width(), rect.Height());
		}
	}
}

void CListCtrlSize::UpdateSize() {

	KillTimer(TIMER_UPDATE);
	SetTimer(TIMER_UPDATE, AfxGetApp()->GetProfileInt("listctrl", "update", 50), NULL);
}

void CListCtrlSize::UpdateCtrlItems() {

	if( m_sizeDelta.cx==0 && m_sizeDelta.cy==0)
		return;

	CRect rectthis, rect;
	CWnd *parent= GetParent();
	CWnd *item= parent->GetWindow(GW_CHILD);
	GetWindowRect(&rectthis);
	parent->ScreenToClient(&rectthis);

	// establecemos las coordenadas en las antiguas
	rectthis.bottom+=m_sizeDelta.cy;
	rectthis.right+= m_sizeDelta.cx;

	while(item) {

		if( item==this) {
			item= item->GetWindow(GW_HWNDNEXT);
			continue;
		}
		
		item->GetWindowRect(rect);
		parent->ScreenToClient(rect);

		// verificamos si el elemento esta contenido en algo.
		if(rect.PtInRect(rectthis.BottomRight()) && rect.PtInRect(rectthis.TopLeft())) { // el componente esta dentro de este

			rect.bottom-= m_sizeDelta.cy;
			rect.right-=m_sizeDelta.cx;
			item->MoveWindow(rect);
			item= item->GetWindow(GW_HWNDNEXT);
			continue;
		}
		if( rect.top < rectthis.top) {
			item= item->GetWindow(GW_HWNDNEXT);
			continue;
		}
		if( m_pFin==item) 
			rect.MoveToXY(rect.left- m_sizeDelta.cx, rect.top-m_sizeDelta.cy);
		else rect.MoveToY(rect.top-m_sizeDelta.cy);
		item->MoveWindow(rect);
		item= item->GetWindow(GW_HWNDNEXT);
	}

	parent->RedrawWindow();
	if(parent->IsKindOf(RUNTIME_CLASS(CFormView))) {
		RecalculateScroll((CFormView *)parent);
	} else if(parent->IsKindOf(RUNTIME_CLASS(CDialog))) {
		parent->GetWindowRect(rect);
		rect.bottom-=m_sizeDelta.cy;
		rect.right-=m_sizeDelta.cx;
		parent->MoveWindow(rect);
	}
}


void CListCtrlSize::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) {

	CMyListCtrl::DrawItem(lpDrawItemStruct);
}

BOOL CListCtrlSize::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->message==WM_MOUSEWHEEL) {
		GetParent()->SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
	}
	return CMyListCtrl::PreTranslateMessage(pMsg);
}


BOOL CListCtrlSize::DeleteItem(int nItem) {
	
	BOOL retorno= CMyListCtrl::DeleteItem(nItem);
	UpdateSize();
	return retorno;
}

BOOL CListCtrlSize::DeleteAllItems() {

	BOOL retorno= CMyListCtrl::DeleteAllItems();
	UpdateSize();
	return retorno;
}

BOOL CListCtrlSize::SetItemText(int nItem,int nSubItem,LPCTSTR lpszText) {

	BOOL retorno= CMyListCtrl::SetItemText(nItem,nSubItem,lpszText);
	UpdateSize();
	return retorno;
}

int CListCtrlSize::InsertItem(int nItem,LPCTSTR lpszItem) {

	int retorno= CMyListCtrl::InsertItem(nItem,lpszItem);
	UpdateSize();
	return retorno;
}

int CListCtrlSize::InsertItem(int nItem,LPCTSTR lpszItem,int nImage) {

	int retorno= CMyListCtrl::InsertItem(nItem,lpszItem,nImage);
	UpdateSize();
	return retorno;
}

int	CListCtrlSize::AddItem(LPCTSTR lpszItem) {

	int retorno= CMyListCtrl::AddItem(lpszItem);
	UpdateSize();
	return retorno;
}

void CListCtrlSize::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent==TIMER_UPDATE) {
		KillTimer(TIMER_UPDATE);
		RealUpdateSize();
	}
	CListCtrlXP::OnTimer(nIDEvent);
}

void CListCtrlSize::OnDestroy()
{
	KillTimer(TIMER_UPDATE);
	CListCtrlXP::OnDestroy();
}

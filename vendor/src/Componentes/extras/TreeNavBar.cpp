#include "stdafx.h"
# include "Componentes.h"
#include "extras/TreeNavBar.h"

/////////////////////////////////////////////////////////////////////////////
// CTreeNavBar

CTreeNavBar::CTreeNavBar()
{
}

CTreeNavBar::~CTreeNavBar()
{
}

BEGIN_MESSAGE_MAP(CTreeNavBar, CGuiControlBar)
	ON_WM_CREATE()
	ON_NOTIFY(TVN_SELCHANGED, 201, OnChangeSelArborReal)
	ON_NOTIFY(NM_DBLCLK, 201, OnChangeSelArbol)
	ON_NOTIFY(NM_RCLICK, 201, OnRClick)
	ON_COMMAND(201,OpenDocument)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMyBar message handlers

HTREEITEM CTreeNavBar::FindItem(CString nombre, HTREEITEM hItem)
{
	if( !m_Tree)
		return NULL;
	HTREEITEM htiCurrent = hItem ? hItem : m_Tree.GetRootItem();
	while(htiCurrent)
	{
		if( m_Tree.GetItemText( htiCurrent)== nombre)
			return htiCurrent;
		htiCurrent = GetNextItemEx(htiCurrent);
	}
	return NULL;
}

HTREEITEM CTreeNavBar::FindItem(UINT id, HTREEITEM hItem)
{
	if( !m_Tree)
		return NULL;
	HTREEITEM htiCurrent = hItem ? hItem : m_Tree.GetRootItem();
	while(htiCurrent)
	{
		if ((UINT)m_Tree.GetItemData(htiCurrent)== id)
		
			return htiCurrent;
		htiCurrent = GetNextItemEx(htiCurrent);
	}
	return NULL;
}

HTREEITEM CTreeNavBar::GetNextItemEx(HTREEITEM hItem)
{
	HTREEITEM hti;

	if(m_Tree.ItemHasChildren(hItem))

		return m_Tree.GetChildItem(hItem);
	else
		while((hti = m_Tree.GetNextSiblingItem(hItem)) == NULL)

			if((hItem = m_Tree.GetParentItem(hItem)) == NULL)

				return NULL;
    return hti;
}

HTREEITEM CTreeNavBar::FindItemEx(CString nombre, HTREEITEM hItem) {

	if( !m_Tree)
		return NULL;
	HTREEITEM htiCurrent = hItem ? hItem : m_Tree.GetRootItem();
	while(htiCurrent)
	{
		DWORD data= m_Tree.GetItemData(htiCurrent);
		if(data && ((CMultiDocTemplateXP *)data)->GetViewClassName()==nombre)
			return htiCurrent;
		htiCurrent = GetNextItemEx(htiCurrent);
	}
	return NULL;
}

void CTreeNavBar::SetSel( CString vista) {

	m_Tree.SelectItem(FindItemEx(vista));
}

void CTreeNavBar::SetSel( UINT id) {

	HTREEITEM item=NULL;
	if( id)
		item= FindItem( id);
	m_Tree.SelectItem(item);
}

int CTreeNavBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CGuiControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	CRect rect;
	DWORD stilo= WS_VISIBLE | WS_BORDER | WS_CHILD | TVS_HASBUTTONS|TVS_LINESATROOT|TVS_HASLINES|TVS_SHOWSELALWAYS;
	if(AfxGetApp()->GetProfileInt("treenav","simple", 0))
		stilo= stilo | TVS_TRACKSELECT;
	m_Tree.Create(stilo, rect, this, 201);
	return 0;
}

void CTreeNavBar::OpenDocument() {

	CWaitCursor wait;
	HTREEITEM item= m_Tree.GetSelectedItem();
	if( item) 
		AfxGetMainWnd()->SendMessage(WM_TREENAVEGADOR, NM_DBLCLK, (LPARAM) m_Tree.GetItemData( item));
}

void CTreeNavBar::OnChangeSelArborReal(NMHDR* pNMHDR, LRESULT* pResult) {

	if(AfxGetApp()->GetProfileInt("treenav","simple", 0))
		OpenDocument();
	pResult= 0;
}

void CTreeNavBar::OnChangeSelArbol( NMHDR* pNMHDR, LRESULT* pResult) {

	OpenDocument();
	pResult= 0;
}

void CTreeNavBar::OnRClick(NMHDR* pNMHDR, LRESULT* pResult) {

	POINT point;
	POINT gpoint;
	UINT uFlags;

	GetCursorPos( &point);
	gpoint= point;
	m_Tree.ScreenToClient( &point);
	HTREEITEM item= m_Tree.HitTest(point, &uFlags);

	if( item && m_Tree.GetItemData(item) && (TVHT_ONITEM & uFlags)) {
		m_Tree.SelectItem(item);
		CNewMenu menu;
		menu.CreatePopupMenu();
		menu.AppendMenu(MF_STRING, 201, "&Abrir");
		menu.TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, gpoint.x, gpoint.y, this);
	}
	pResult= 0;
}

BOOL CTreeNavBar::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->message==WM_KEYUP && pMsg->wParam==VK_RETURN) {
		if( GetFocus()== &m_Tree) 
			OpenDocument();
	}
	return CGuiControlBar::PreTranslateMessage(pMsg);
}


# include "stdafx.h"
#include "VListCtrl.h"
# include "ItemList.h"

IMPLEMENT_DYNAMIC(CVListCtrl, CMyListCtrl)

CVListCtrl::CVListCtrl(void)
{

}

CVListCtrl::~CVListCtrl(void)
{

}

BEGIN_MESSAGE_MAP(CVListCtrl, CMyListCtrl)
	ON_NOTIFY_REFLECT(LVN_GETDISPINFO, OnGetdispinfoList)
	ON_NOTIFY_REFLECT(LVN_ODFINDITEM, OnOdfinditemList)
	ON_NOTIFY_REFLECT(LVN_ODCACHEHINT, OnOdcachehintList)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

void CVListCtrl::OnGetdispinfoList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	//Create a pointer to the item
	LV_ITEM* pItem= &(pDispInfo)->item;

	//Which item number?
	int itemid = pItem->iItem;
	if(m_pItems.GetCount()<pItem->iItem)
		return;
	CItemList *info= (CItemList *)m_pItems[itemid];

	//Do the list need text information?
	if (pItem->mask & LVIF_TEXT)
	{
		lstrcpyn(pItem->pszText,info->m_szTextos.GetCount()>pItem->iSubItem ? info->m_szTextos[pItem->iSubItem] : "",pItem->cchTextMax);
	}
	//Do the list need image information?
	if(pItem->mask & LVIF_IMAGE) 
	{
		//Set which image to use
		pItem->iImage=info->m_nImagen;
		//Show check box?

		if(GetExtendedStyle()&LVS_EX_CHECKBOXES)
		{
			//To enable check box, we have to enable state mask...
			pItem->mask |= LVIF_STATE;
			pItem->stateMask = LVIS_STATEIMAGEMASK;
			pItem->state = INDEXTOSTATEIMAGEMASK(info->m_bCheck ? 2 : 1);
		}
	}
	if(pItem->mask&LVIF_INDENT) {
		pItem->iIndent= info->m_nIndent;
	}
	if(pItem->mask&LVIF_PARAM) {
		pItem->lParam= info->m_dwData;
	}
	if(pItem->mask&LVIF_STATE) {
		pItem->state= info->m_dwState;
	}
	*pResult = 0;
}

//This functions is called when the user "writes" in the list box to find an item.
void CVListCtrl::OnOdfinditemList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// pNMHDR has information about the item we should find
	// In pResult we should save which item that should be selected
	NMLVFINDITEM* pFindInfo = (NMLVFINDITEM*)pNMHDR;

	/* pFindInfo->iStart is from which item we should search.
	We search to bottom, and then restart at top and will stop
	at pFindInfo->iStart, unless we find an item that match
	*/

	// Set the default return value to -1
	// That means we didn't find any match.
	*pResult = -1;

	//Is search NOT based on string?
	if( (pFindInfo->lvfi.flags & LVFI_STRING) == 0 )
	{
		//This will probably never happend...
		return;
	}

	/*
	Let's look on a sample list;

	Name
	Anders
	* Anna
	Annika
	Bob
	Emma
	Emmanuel

	Anna is selected. 
	If "A" is written, Annika should be selected.
	If "AND" is written, Anders should be selected. 
	If "ANNK" is written, the selection should stay on Anna.
	If "E" is written, Emma should be selected.

	*/

	//This is the string we search for
	CString searchstr = pFindInfo->lvfi.psz;

	//	TRACE(_T("Find: %s\n"), searchstr);

	int startPos = pFindInfo->iStart;
	//Is startPos outside the list (happens if last item is selected)
	if(startPos >= GetItemCount())
		startPos = 0;

	int currentPos=startPos;

	//Let's search...
	do
	{		
		//Do this word begins with all characters in searchstr?
		if( _tcsnicmp( ((CItemList *) m_pItems[currentPos])->m_szTextos[0], searchstr, searchstr.GetLength()) == 0)
		{
			//Select this item and stop search.
			*pResult = currentPos;
			break;
		}

		//Go to next item
		currentPos++;

		//Need to restart at top?
		if(currentPos >= GetItemCount())
			currentPos = 0;

		//Stop if back to start
	}while(currentPos != startPos);		
}

//This is called to give you a chance to catch data. Useless in most cases :-)
void CVListCtrl::OnOdcachehintList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMLVCACHEHINT* pCacheHint = (NMLVCACHEHINT*)pNMHDR;

	/*	TRACE(	_T("Chache item %d to item %d\n"),
	pCacheHint->iFrom,
	pCacheHint->iTo );*/

	//... cache the data pCacheHint->iFrom to pCacheHint->iTo ...

	*pResult = 0;
}

int CVListCtrl::InsertItem(int nItem,LPCTSTR lpszItem) {

	return InsertItem(nItem, lpszItem, 0);
}

int CVListCtrl::InsertItem(int nItem,LPCTSTR lpszItem,int nImage) {

	ASSERT(GetStyle() & LVS_OWNERDATA);
	ASSERT(nItem>=-1);
	CItemList *item= new CItemList(lpszItem, nImage);
	if(nItem==-1) 
		nItem=m_pItems.Add(item);
	else m_pItems.InsertAt(nItem, item);
	SetItemCount(m_pItems.GetCount());
	return nItem;
}

BOOL CVListCtrl::DeleteItem(int nItem) {

	ASSERT(GetStyle() & LVS_OWNERDATA);
	ASSERT(GetItemCount()>nItem);
	CWaitCursor wait;
	CItemList *item= (CItemList *)m_pItems[nItem];
	m_pItems.RemoveAt(nItem);
	SetItemCount(m_pItems.GetCount());
	delete item;
	return true;
}

BOOL CVListCtrl::DeleteAllItems() {

	ASSERT(GetStyle() & LVS_OWNERDATA);
	CWaitCursor wait;
	SetItemCount(0);
	CItemList *item;
	for( int i=0; i<m_pItems.GetCount(); i++) {
		item= (CItemList *)m_pItems[i];
		delete item;
	}
	m_pItems.RemoveAll();
	return true;
}

BOOL CVListCtrl::SetCheck(int nItem,BOOL fCheck /*= TRUE*/) {

	ASSERT(GetStyle() & LVS_OWNERDATA);
	ASSERT(GetItemCount()>nItem);
	CItemList *item= (CItemList *)m_pItems[nItem];
	item->m_bCheck= fCheck;
	return fCheck;
}

BOOL CVListCtrl::SetItemText(int nItem,int nSubItem,LPCTSTR lpszText) {

	ASSERT(GetStyle() & LVS_OWNERDATA);
	ASSERT(GetItemCount()>nItem);
	CItemList *item= (CItemList *)m_pItems[nItem];
	for( int i=item->m_szTextos.GetCount(); i<=nSubItem; i++) {
		item->m_szTextos.Add("");
	}
	item->m_szTextos.SetAt(nSubItem, lpszText);
	return true;
}

void CVListCtrl::OnDestroy() {

	ASSERT(GetStyle() & LVS_OWNERDATA);
	DeleteAllItems();
	CListCtrl::OnDestroy();
}

BOOL CVListCtrl::SetItemData(int nItem, DWORD_PTR dwData) {

	ASSERT(GetStyle() & LVS_OWNERDATA);
	ASSERT(GetItemCount()>nItem);
	CItemList *item= (CItemList *)m_pItems[nItem];
	item->m_dwData= dwData;
	return true;
}

CPtrArray *CVListCtrl::GetItems() {

	ASSERT((GetStyle() & LVS_OWNERDATA));
	return &m_pItems;
}

DWORD_PTR CVListCtrl::GetItemData(int nItem) {

	ASSERT(GetStyle() & LVS_OWNERDATA);
	ASSERT(GetItemCount()>nItem);
	CItemList *item= (CItemList *)m_pItems[nItem];
	return item->m_dwData;
}

CString CVListCtrl::GetItemText(int nItem, int nSubItem/*=0*/) {

	ASSERT(GetStyle() & LVS_OWNERDATA);
	ASSERT(GetItemCount()>nItem);
	CItemList *item= (CItemList *)m_pItems[nItem];

	return item->m_szTextos.GetCount()>nSubItem ? item->m_szTextos[nSubItem] : NULL;
}

BOOL CVListCtrl::GetCheck(int nItem) {

	ASSERT(GetStyle() & LVS_OWNERDATA);
	ASSERT(GetItemCount()>nItem);
	CItemList *item= (CItemList *)m_pItems[nItem];
	return item->m_bCheck;
}

void CVListCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) {

	CMyListCtrl::DrawItem(lpDrawItemStruct);
}

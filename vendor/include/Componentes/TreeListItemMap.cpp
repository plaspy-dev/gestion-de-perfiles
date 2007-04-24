# include "stdafx.h"
#include "TreeListItemMap.h"
# include "Componentes.h"

void ShowGetLastError()
{
	LPVOID lpMsgBuf;
	FormatMessage( 	FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf,
		0,
		NULL 
		);

	// Display the string.
	AfxMessageBox((LPCTSTR)lpMsgBuf);
	TRACE0((LPCTSTR)lpMsgBuf);

	// LOGERROR(((LPCTSTR)lpMsgBuf));

	// Free the buffer.
	LocalFree( lpMsgBuf );
}

CTreeListItemMap::CTreeListItemMap(void)
{
	m_pRootItem= new CTreeListItem( 1 );
	m_pRootItem->Expand();
	SetAt(m_pRootItem, m_pRootItem);
}

CTreeListItemMap::~CTreeListItemMap(void)
{
	delete m_pRootItem;
}

BOOL CTreeListItemMap::Lookup(void* key, void*& rValue) {

	m_pMutex.Lock();
	BOOL pRet = FALSE;
	try
	{
		pRet = CMapPtrToPtr::Lookup(key, rValue);
	}
	catch(...)
	{
		ShowGetLastError();
	}
	// Release the synch object
	m_pMutex.Unlock();
	return pRet;
}

void CTreeListItemMap::SetAt(void* key, void* newValue) {

	m_pMutex.Lock();
	try
	{
		CMapPtrToPtr::SetAt(key, newValue);
	}
	catch(...)
	{
		ShowGetLastError();
	}
	// Release the synch object
	m_pMutex.Unlock();
}

BOOL CTreeListItemMap::RemoveKey(void* key) {

	m_pMutex.Lock();
	BOOL pRet = FALSE;
	try
	{
		pRet = CMapPtrToPtr::RemoveKey(key);
	}
	catch(...)
	{
		ShowGetLastError();
	}
	// Release the synch object
	m_pMutex.Unlock();
	return pRet;
}

void CTreeListItemMap::Lock() {

	m_pMutex.Lock();
}

void CTreeListItemMap::UnLock() {

	m_pMutex.Unlock();
}

HTREELISTITEM CTreeListItemMap::GetRootItem() {

	return (HTREELISTITEM)m_pRootItem;
}

CTreeListItem *CTreeListItemMap::GetItemUnlok(HTREELISTITEM item) {

	if(item==NULL)
		return NULL;
	CTreeListItem *pItem= NULL;
	try
	{
		if(!CMapPtrToPtr::Lookup((void *)item, (void *&)pItem)) {
			pItem=NULL;
		}
	}
	catch(...)
	{
		ShowGetLastError();
	}
	return pItem;
}

CTreeListItem *CTreeListItemMap::GetItem(HTREELISTITEM item) {

	if(item==NULL)
		return NULL;
	m_pMutex.Lock();
	CTreeListItem *pItem= NULL;
	try
	{
		if(!CMapPtrToPtr::Lookup((void *)item, (void *&)pItem)) {
			pItem=NULL;
		}
	}
	catch(...)
	{
		ShowGetLastError();
	}
	return pItem;
}

int	CTreeListItemMap::GetChildCount(HTREELISTITEM item) {

	int pRet = 0;
	CTreeListItem *pItem=GetItem(item);
	if(pItem) {
		pRet= pItem->m_nChild;
	}
	m_pMutex.Unlock();
	return pRet;
}

void CTreeListItemMap::SetChildItem(HTREELISTITEM item, HTREELISTITEM child) {

	CTreeListItem *pChild= GetItem(child);
	CTreeListItem *pItem=GetItemUnlok(item);
	if(pItem) {
		pItem->m_pChild= pChild;
	}
	m_pMutex.Unlock();
}

void CTreeListItemMap::SetChildCount(HTREELISTITEM item, int count) {

	CTreeListItem *pItem=GetItem(item);
	if(pItem) {
		pItem->m_nChild= count;
	}
	m_pMutex.Unlock();
}

bool CTreeListItemMap::ItemHasChildren(HTREELISTITEM item) {

	bool pRet = FALSE;
	CTreeListItem *pItem=GetItem(item);
	if(pItem) {
		pRet= pItem->m_pChild!=NULL;
	}
	m_pMutex.Unlock();
	return pRet;
}

HTREELISTITEM CTreeListItemMap::GetChildItem(HTREELISTITEM item) {

	HTREELISTITEM pRet = NULL;
	CTreeListItem *pItem=GetItem(item);
	if(pItem) {
		pRet= (HTREELISTITEM)pItem->m_pChild;
	}
	// Release the synch object
	m_pMutex.Unlock();
	return pRet;
}

void CTreeListItemMap::SetNextSiblingItem( HTREELISTITEM item, HTREELISTITEM next ) {

	CTreeListItem *pNext= GetItem(next);
	CTreeListItem *pItem=GetItemUnlok(item);
	if(pItem) {
		pItem->m_pNext= pNext;
	}
	// Release the synch object
	m_pMutex.Unlock();
}

HTREELISTITEM CTreeListItemMap::GetNextSiblingItem( HTREELISTITEM item ) {

	HTREELISTITEM pRet = NULL;
	CTreeListItem *pItem=GetItem(item);
	if(pItem) {
		pRet= (HTREELISTITEM)pItem->m_pNext;
	}
	// Release the synch object
	m_pMutex.Unlock();
	return pRet;
}

void CTreeListItemMap::SetPrevSiblingItem( HTREELISTITEM item, HTREELISTITEM prev ) {

	CTreeListItem *pPrev=GetItem(prev);
	CTreeListItem *pItem=GetItemUnlok(item);
	if(pItem) {
		pItem->m_pPrev= pPrev;
	}
	m_pMutex.Unlock();
}

HTREELISTITEM	CTreeListItemMap::GetPrevSiblingItem( HTREELISTITEM item ) {

	HTREELISTITEM pRet = NULL;
	CTreeListItem *pItem=GetItem(item);
	if(pItem) {
		pRet= (HTREELISTITEM)pItem->m_pPrev;
	}
	// Release the synch object
	m_pMutex.Unlock();
	return pRet;
}

HTREELISTITEM	CTreeListItemMap::GetParentItem( HTREELISTITEM item ) {

	HTREELISTITEM pRet = NULL;
	CTreeListItem *pItem=GetItem(item);
	if(pItem) {
		pRet= (HTREELISTITEM)pItem->m_pParent;
	}
	// Release the synch object
	m_pMutex.Unlock();
	return pRet;
}

void CTreeListItemMap::SetParentItem(HTREELISTITEM item, HTREELISTITEM parent ) {

	CTreeListItem *pParent= GetItem(parent);
	CTreeListItem *pItem=GetItemUnlok(item);
	if(pItem) {
		pItem->m_pParent= pParent;
	}
	m_pMutex.Unlock();
}

DWORD CTreeListItemMap::GetItemState( HTREELISTITEM item, DWORD nStateMask) {

	DWORD pRet = NULL;
	CTreeListItem *pItem=GetItem(item);
	if(pItem) {
		pRet= pItem->GetState(nStateMask);
	}
	// Release the synch object
	m_pMutex.Unlock();
	return pRet;
}

void CTreeListItemMap::SetItemDataSort(HTREELISTITEM item, DWORD_PTR data) {

	CTreeListItem *pItem=GetItem(item);
	if(pItem) {
		pItem->SetDataSort(data);
	}
	// Release the synch object
	m_pMutex.Unlock();
}

DWORD_PTR CTreeListItemMap::GetItemDataSort( HTREELISTITEM item ) {

	DWORD_PTR resultado=0;
	CTreeListItem *pItem=GetItem(item);
	if(pItem) {
		resultado= pItem->GetDataSort();
	}
	// Release the synch object
	m_pMutex.Unlock();
	return resultado;
}

void CTreeListItemMap::SetItemTipo(HTREELISTITEM item, int tipo) {
	CTreeListItem *pItem=GetItem(item);
	if(pItem) {
		pItem->m_nTipo= tipo;
	}
	// Release the synch object
	m_pMutex.Unlock();
}

void CTreeListItemMap::SetItemColumna(HTREELISTITEM item, int columna) {

	CTreeListItem *pItem=GetItem(item);
	if(pItem) {
		pItem->m_nColumna= columna;
	}
	// Release the synch object
	m_pMutex.Unlock();
}

void CTreeListItemMap::SetItemAscendente(HTREELISTITEM item, bool ascendente) {

	CTreeListItem *pItem=GetItem(item);
	if(pItem) {
		pItem->m_bAscendente= ascendente;
	}
	// Release the synch object
	m_pMutex.Unlock();
}

void CTreeListItemMap::SetItemState(HTREELISTITEM item, DWORD dwAddState, DWORD dwRemoveState) {

	CTreeListItem *pItem=GetItem(item);
	if(pItem) {
		pItem->SetState(dwAddState, dwRemoveState);
	}
	// Release the synch object
	m_pMutex.Unlock();
}

void CTreeListItemMap::GetItemImage( HTREELISTITEM item, int& nImage, int& nExpandImage, int& nSelectedImage, int& nExpandSelectedImage ) {

	CTreeListItem *pItem=GetItem(item);
	if(pItem) {
		pItem->GetImage(nImage, nExpandImage, nSelectedImage, nExpandSelectedImage);
	}
	// Release the synch object
	m_pMutex.Unlock();
}

void CTreeListItemMap::SetItemImage( HTREELISTITEM item, int nImage, int nExpandImage, int nSelectedImage, int nExpandSelectedImage ) {

	CTreeListItem *pItem=GetItem(item);
	if(pItem) {
		pItem->SetImage(nImage, nExpandImage, nSelectedImage, nExpandSelectedImage);
	}
	// Release the synch object
	m_pMutex.Unlock();
}

LPCTSTR	CTreeListItemMap::GetItemText( HTREELISTITEM item, int nSubItem ) {

	LPCTSTR retorno=NULL;
	CTreeListItem *pItem=GetItem(item);
	if(pItem) {
		retorno= pItem->GetText(nSubItem);
	}
	// Release the synch object
	m_pMutex.Unlock();
	return retorno;
}

BOOL CTreeListItemMap::SetItemText( HTREELISTITEM item, LPCTSTR lpszText, int nSubItem ) {

	BOOL retorno=FALSE;
	CTreeListItem *pItem=GetItem(item);
	if(pItem) {
		retorno= pItem->SetText(lpszText, nSubItem);
	}
	// Release the synch object
	m_pMutex.Unlock();
	return retorno;
}

COLORREF CTreeListItemMap::GetItemColor(HTREELISTITEM item) {

	COLORREF retorno=0;
	CTreeListItem *pItem=GetItem(item);
	if(pItem) {
		retorno= pItem->m_clrTexto;
	}
	// Release the synch object
	m_pMutex.Unlock();
	return retorno;
}

void CTreeListItemMap::SetItemColor(HTREELISTITEM item, COLORREF color) {

	CTreeListItem *pItem=GetItem(item);
	if(pItem) {
		pItem->m_clrTexto= color;
	}
	// Release the synch object
	m_pMutex.Unlock();
}

void CTreeListItemMap::SetItemBold(HTREELISTITEM item, bool bold/*=true*/) {

	CTreeListItem *pItem=GetItem(item);
	if(pItem) {
		pItem->SetBold(bold);
	}
	// Release the synch object
	m_pMutex.Unlock();
}

bool CTreeListItemMap::GetItemBold(HTREELISTITEM item) {

	bool resultado=false;
	CTreeListItem *pItem=GetItem(item);
	if(pItem) {
		resultado= pItem->m_bBold;
	}
	// Release the synch object
	m_pMutex.Unlock();
	return resultado;
}

DWORD_PTR CTreeListItemMap::GetItemData( HTREELISTITEM item ) {

	DWORD_PTR resultado=0;
	CTreeListItem *pItem=GetItem(item);
	if(pItem) {
		resultado= pItem->GetData();
	}
	// Release the synch object
	m_pMutex.Unlock();
	return resultado;
}

void CTreeListItemMap::SetItemData( HTREELISTITEM item, DWORD_PTR dwData ) {

	CTreeListItem *pItem=GetItem(item);
	if(pItem) {
		pItem->SetData(dwData);
	}
	// Release the synch object
	m_pMutex.Unlock();
}

int	CTreeListItemMap::GetItemCheck( HTREELISTITEM item ) {

	int resultado=0;
	CTreeListItem *pItem=GetItem(item);
	if(pItem) {
		resultado= pItem->GetCheck();
	}
	// Release the synch object
	m_pMutex.Unlock();
	return resultado;
}

void CTreeListItemMap::SetItemCheck(HTREELISTITEM item, int check) {

	CTreeListItem *pItem=GetItem(item);
	if(pItem) {
		pItem->SetCheck(check);
	}
	// Release the synch object
	m_pMutex.Unlock();
}

int	CTreeListItemMap::GetItemLock( HTREELISTITEM item ) {

	int resultado=0;
	CTreeListItem *pItem=GetItem(item);
	if(pItem) {
		resultado= pItem->GetLock();
	}
	// Release the synch object
	m_pMutex.Unlock();
	return resultado;
}

void CTreeListItemMap::SetItemLock( HTREELISTITEM item, int lock) {

	CTreeListItem *pItem=GetItem(item);
	if(pItem) {
		pItem->SetLock(lock);
	}
	// Release the synch object
	m_pMutex.Unlock();
}

int	CTreeListItemMap::GetItemLevel(HTREELISTITEM item) {

	int resultado=0;
	CTreeListItem *pItem=GetItem(item);
	if(pItem) {
		resultado= pItem->m_nLevel;
	}
	// Release the synch object
	m_pMutex.Unlock();
	return resultado;
}

void CTreeListItemMap::SetItemLevel(HTREELISTITEM item, int level) {

	CTreeListItem *pItem=GetItem(item);
	if(pItem) {
		pItem->m_nLevel= level;
	}
	// Release the synch object
	m_pMutex.Unlock();
}

int	CTreeListItemMap::GetVisibleChildren(HTREELISTITEM item) {

	int resultado=0;
	CTreeListItem *pItem=GetItem(item);
	if(pItem) {
		resultado= pItem->m_nVisibleChild;
	}
	// Release the synch object
	m_pMutex.Unlock();
	return resultado;
}

void CTreeListItemMap::SetVisibleChildren(HTREELISTITEM item, int count) {

	CTreeListItem *pItem=GetItem(item);
	if(pItem) {
		pItem->m_nVisibleChild= count;
	}
	// Release the synch object
	m_pMutex.Unlock();
}

bool CTreeListItemMap::GetItemSelected(HTREELISTITEM item) {

	bool resultado=false;
	CTreeListItem *pItem=GetItem(item);
	if(pItem) {
		resultado= pItem->GetSelected();
	}
	// Release the synch object
	m_pMutex.Unlock();
	return resultado;
}

void CTreeListItemMap::SetItemSelected(HTREELISTITEM item, bool selected/*=true*/) {

	CTreeListItem *pItem=GetItem(item);
	if(pItem) {
		pItem->SetSelected(selected);
	}
	// Release the synch object
	m_pMutex.Unlock();
}

int	CTreeListItemMap::GetItemExpandedSelectedImage(HTREELISTITEM item) {

	int resultado=0;
	CTreeListItem *pItem=GetItem(item);
	if(pItem) {
		resultado= pItem->m_nExpandSelectedImage;
	}
	// Release the synch object
	m_pMutex.Unlock();
	return resultado;
}

int	CTreeListItemMap::GetItemSelectedImage(HTREELISTITEM item) {

	int resultado=0;
	CTreeListItem *pItem=GetItem(item);
	if(pItem) {
		resultado= pItem->m_nSelectedImage;
	}
	// Release the synch object
	m_pMutex.Unlock();
	return resultado;
}

int	CTreeListItemMap::GetItemExpandedImage(HTREELISTITEM item) {

	int resultado=0;
	CTreeListItem *pItem=GetItem(item);
	if(pItem) {
		resultado= pItem->m_nExpandImage;
	}
	// Release the synch object
	m_pMutex.Unlock();
	return resultado;
}

int CTreeListItemMap::GetItemImage(HTREELISTITEM item) {

	int resultado=0;
	CTreeListItem *pItem=GetItem(item);
	if(pItem) {
		resultado= pItem->m_nImage;
	}
	// Release the synch object
	m_pMutex.Unlock();
	return resultado;
}

void CTreeListItemMap::ResizeItem(HTREELISTITEM item, int tamano) {

	CTreeListItem *pItem=GetItem(item);
	if(pItem) {
		pItem->ReSize(tamano);
	}
	// Release the synch object
	m_pMutex.Unlock();
}

void CTreeListItemMap::ExpandItem(HTREELISTITEM item){

	CTreeListItem *pItem=GetItem(item);
	if(pItem) {
		pItem->Expand();
	}
	// Release the synch object
	m_pMutex.Unlock();
}

void CTreeListItemMap::CollapseItem(HTREELISTITEM item) {

	CTreeListItem *pItem=GetItem(item);
	if(pItem) {
		pItem->Collapse();
	}
	// Release the synch object
	m_pMutex.Unlock();
}

void CTreeListItemMap::DeleteItem(HTREELISTITEM item) {

	CTreeListItem *pItem=GetItem(item);
	if(pItem) {
		CMapPtrToPtr::RemoveKey((void *)item);
		delete pItem;
	}
	// Release the synch object
	m_pMutex.Unlock();
}

HTREELISTITEM CTreeListItemMap::AddItem(int column) {

	CTreeListItem* pNewItem;
	pNewItem = new CTreeListItem(column);
	SetAt(pNewItem, pNewItem);
	return (HTREELISTITEM)pNewItem;
}

void CTreeListItemMap::SortSubItems(int columna, HTREELISTITEM parent, int tipo, bool clean,bool ascendente, DWORD *&items, int count) {

	CTreeListItem *pParent= GetItem(parent);
	CTreeListItem *item= pParent->m_pChild;
	count=0;
	while(item) {
		if( clean) 
			item->SetText(GetLeftString(item->GetText(columna), "("), 0);
		item->m_nColumna= columna;
		item->m_bAscendente= ascendente;
		item->m_nTipo= tipo;
		items[count]= (DWORD) item;
		count++;
		item= item->m_pNext;
	}
	qsort(items, count, sizeof(DWORD), CTreeListItem::Compare);
	m_pMutex.Unlock();
}

void CTreeListItemMap::SortSubItems(int columna, CPtrArray &arreglo, DWORD *&items, int count) {

	Lock();
	CTreeListItem *item=NULL;
	if( !items)
		count=0;
	for( int i=0; i<count; i++) {
		if(item= GetItemUnlok((HTREELISTITEM)arreglo.GetAt(i))) {
			item->m_nTipo= 100;
			item->m_nColumna= 0;
			item->m_bAscendente= false;
			items[i]= (DWORD) item;
		} else count--;
	}
	if( count)
		qsort(items, count, sizeof(DWORD), CTreeListItem::Compare);

	for( int i=0; i<count; i++) {
		item= (CTreeListItem *)items[i];
		item->SetDataSort(0);
		arreglo.SetAt(i, (void *)item);
	}
	m_pMutex.Unlock();
}

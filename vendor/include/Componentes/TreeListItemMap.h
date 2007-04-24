#pragma once
#include "afxcoll.h"
# include "treelistctrl/TreeListItem.h"
# include <afxmt.h>

class CTreeListItemMap : protected CMapPtrToPtr
{
private:
	CMutex			m_pMutex;
protected:
	CTreeListItem *m_pRootItem;
public:
	CTreeListItemMap(void);
	~CTreeListItemMap(void);
private:
	CTreeListItem *GetItem(HTREELISTITEM item);
	CTreeListItem *GetItemUnlok(HTREELISTITEM item);
protected:
	BOOL Lookup(void* key, void*& rValue);
	void SetAt(void* key, void* newValue);
	BOOL RemoveKey(void* key);
public:
	void			Lock();
	void			UnLock();
	HTREELISTITEM	GetRootItem();
	HTREELISTITEM	GetChildItem(HTREELISTITEM item);
	void			SetChildItem(HTREELISTITEM item, HTREELISTITEM child);
	int				GetChildCount(HTREELISTITEM item);
	void			SetChildCount(HTREELISTITEM item, int count);
	bool			ItemHasChildren(HTREELISTITEM item);
	HTREELISTITEM	GetNextSiblingItem( HTREELISTITEM item );
	void			SetNextSiblingItem( HTREELISTITEM item, HTREELISTITEM next );
	HTREELISTITEM	GetPrevSiblingItem( HTREELISTITEM item );
	void			SetPrevSiblingItem( HTREELISTITEM item, HTREELISTITEM prev );
	HTREELISTITEM	GetParentItem( HTREELISTITEM item );
	void			SetParentItem(HTREELISTITEM item, HTREELISTITEM parent);
	DWORD			GetItemState(HTREELISTITEM item,DWORD nStateMask=TLIS_ALLSTATE);
	void			SetItemDataSort(HTREELISTITEM item, DWORD_PTR data);
	DWORD_PTR		GetItemDataSort(HTREELISTITEM item);
	void			SetItemTipo(HTREELISTITEM item, int tipo);
	void			SetItemColumna(HTREELISTITEM item, int columna);
	void			SetItemAscendente(HTREELISTITEM item, bool ascendente);
	void			SetItemState(HTREELISTITEM item, DWORD dwAddState, DWORD dwRemoveState = 0 );
	void			GetItemImage( HTREELISTITEM item, int& nImage, int& nExpandImage, int& nSelectedImage, int& nExpandSelectedImage );
	void			SetItemImage( HTREELISTITEM item, int nImage, int nExpandImage, int nSelectedImage, int nExpandSelectedImage );
	LPCTSTR			GetItemText( HTREELISTITEM item, int nSubItem );
	BOOL			SetItemText( HTREELISTITEM item, LPCTSTR lpszText, int nSubItem );
	COLORREF		GetItemColor(HTREELISTITEM item);
	void			SetItemColor(HTREELISTITEM item, COLORREF color);
	void			SetItemBold(HTREELISTITEM item, bool bold=true);
	bool			GetItemBold(HTREELISTITEM item);
	DWORD_PTR		GetItemData( HTREELISTITEM item );
	void			SetItemData( HTREELISTITEM item, DWORD_PTR dwData );
	int				GetItemCheck( HTREELISTITEM item );
	void			SetItemCheck(HTREELISTITEM item, int check);
	int				GetItemLock( HTREELISTITEM item );
	void			SetItemLock( HTREELISTITEM item, int lock);
	int				GetItemLevel(HTREELISTITEM item);
	void			SetItemLevel(HTREELISTITEM item, int level);
	int				GetVisibleChildren(HTREELISTITEM item);
	void			SetVisibleChildren(HTREELISTITEM item, int count);
	bool			GetItemSelected(HTREELISTITEM item);
	void			SetItemSelected(HTREELISTITEM item, bool selected=true);
	int				GetItemExpandedSelectedImage(HTREELISTITEM item);
	int				GetItemSelectedImage(HTREELISTITEM item);
	int				GetItemExpandedImage(HTREELISTITEM item);
	int				GetItemImage(HTREELISTITEM item);
	void			ResizeItem(HTREELISTITEM item, int tamano);
	void			ExpandItem(HTREELISTITEM item);
	void			CollapseItem(HTREELISTITEM item);
	void			DeleteItem(HTREELISTITEM item);
	HTREELISTITEM	AddItem(int column);
	void			SortSubItems(int columna, HTREELISTITEM parent, int tipo, bool clean,bool ascendente, DWORD *&items, int count);
	void			SortSubItems(int columna, CPtrArray &arreglo, DWORD *&items, int count);
};

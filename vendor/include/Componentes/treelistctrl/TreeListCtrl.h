/////////////////////////////////////////////////////////////////////////////
// CTreeListCtrl : Main control class drived from CWnd class
// Writter       : TigerX 
// Email         : idemail@yahoo.com
/////////////////////////////////////////////////////////////////////////////
#if !defined(AFX_TREELISTCTRL_H__55B6E4E9_1E57_4FEE_AA05_77C052ACE9B5__INCLUDED_)
#define AFX_TREELISTCTRL_H__55B6E4E9_1E57_4FEE_AA05_77C052ACE9B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TreeListCtrl.h : header file
/////////////////////////////////////////////////////////////////////////////
// CTreeListCtrl window
# include "afxtempl.h"
# include "treelistcolumninfo.h"
# include "TreeListHeaderCtrl.h"
# include "../newmenu/newmenu.h"
# include "TreeListItemMap.h"
# include <afxmt.h>

#define TREELISTCTRL_CLASSNAME		_T("Componentes.TreeListCtrl")


// TreeListCtrl Styles
#define TLC_TREELIST				0x00000001				// TreeList or List
#define TLC_BKGNDIMAGE				0x00000002				// image background
#define TLC_BKGNDCOLOR				0x00000004				// colored background ( not client area )
#define TLC_DOUBLECOLOR				0x00000008				// double color background

#define TLC_MULTIPLESELECT			0x00000010				// single or multiple select
#define TLC_SHOWSELACTIVE			0x00000020				// show active column of selected item
#define TLC_SHOWSELALWAYS			0x00000040				// show selected item always
#define TLC_SHOWSELFULLROWS			0x00000080				// show selected item in fullrow mode

#define TLC_HEADER					0x00000100				// show header
#define TLC_HGRID					0x00000200				// show horizonal grid lines
#define TLC_VGRID					0x00000400				// show vertical grid lines
#define TLC_TGRID					0x00000800				// show tree horizonal grid lines ( when HGRID & VGRID )

#define TLC_HGRID_EXT				0x00001000				// show extention horizonal grid lines
#define TLC_VGRID_EXT				0x00002000				// show extention vertical grid lines
#define TLC_HGRID_FULL				0x00004000				// show full horizonal grid lines
#define TLC_READONLY				0x00008000				// read only

#define TLC_TREELINE				0x00010000				// show tree line
#define TLC_ROOTLINE				0x00020000				// show root line
#define TLC_BUTTON					0x00040000				// show expand/collapse button [+]
#define TLC_CHECKBOX				0x00080000				// show check box
#define TLC_LOCKBOX					0x00100000				// show lock box
#define TLC_IMAGE					0x00200000				// show image
#define TLC_HOTTRACK				0x00400000				// show hover text

#define TLC_NOAUTOCHECK				0x40000000				// do NOT auto set checkbox of parent & child
#define TLC_NOAUTOLOCK				0x80000000				// do NOT auto set lockbox of parent & child

// TreeListCtrl State
#define TLS_SUBCLASSFROMCREATE		0x00000001				// subclass when creating
#define TLS_OFFSCREENBUFFER			0x00000002				// use off screen buffer
#define TLS_CAPTURE					0x00000008				// capture

// TreeListItem Consts
#define	TLI_ROOT	(HTREELISTITEM)0xFFFF0001				// root item
#define TLI_LAST	(HTREELISTITEM)0xFFFF0002				// last child item
#define TLI_FIRST	(HTREELISTITEM)0xFFFF0003				// first child item
#define TLI_SORT	(HTREELISTITEM)0xFFFF0004				// sort item

// TreeListCtrl TreeLine Formats
#define TLL_TOP						0x0001					// top line
#define TLL_LEFT					0x0002					// left line
#define TLL_RIGHT					0x0004					// right line
#define TLL_BOTTOM					0x0008					// bottom line

// TreeListCtrl TreeButton Formats
#define TLB_PLUS					0x0001					// plus button
#define TLB_MINUS					0x0002					// minus button
#define TLB_UNKNOW					0x0004					// unknow button

// TreeListCtrl HitTest Position
#define TLHT_ONITEMSPACE			0x0001					// on space
#define TLHT_ONITEMBUTTON			0x0002					// on button
#define TLHT_ONITEMCHECKBOX			0x0004					// on checkbox
#define TLHT_ONITEMLOCKBOX			0x0008					// on lockbox
#define	TLHT_ONITEMIMAGE			0x0010					// on icon
#define TLHT_ONITEMTEXT				0x0020					// on item text

#define TLHT_ONITEM	( TLHT_ONITEMSPACE | TLHT_ONITEMBUTTON | TLHT_ONITEMCHECKBOX | TLHT_ONITEMLOCKBOX | TLHT_ONITEMIMAGE  | TLHT_ONITEMTEXT )

// TreeListCtrl Given Consts
#define TLGN_ROOT					0x0000
#define TLGN_NEXT					0x0001
#define TLGN_PREVIOUS				0x0002
#define TLGN_PARENT					0x0003
#define TLGN_CHILD					0x0004
#define TLGN_FIRSTVISIBLE			0x0005
#define TLGN_NEXTVISIBLE			0x0006
#define TLGN_PREVIOUSVISIBLE		0x0007
#define TLGN_DROPHILITE				0x0008
#define TLGN_CARET					0x0009
#define TLGN_NEXTSELECT				0x000A

// TreeListCtrl Next Item Consts
#define TLNI_ABOVE					0x0100					// find next item form down to up
#define TLNI_BELOW					0x0200					// find next item form up to down

// TreeListCtrl SetColumnWidth Consts
#define TLSCW_AUTOSIZE              -1						// autosize column width by item width
#define TLSCW_AUTOSIZE_USEHEADER    -2						// autosize column width by item width and header width
#define TLSCW_AUTOSIZEV				-3						// autosize column width by visible item width
#define TLSCW_AUTOSIZEV_USEHEADER	-4						// autosize column width by visible item width and header width

// TreeListCtrl Message
#define TLN_SELCHANGING				TVN_SELCHANGING			// selecting a new item
#define TLN_SELCHANGED				TVN_SELCHANGED			// selected an item
#define TLN_GETDISPINFO				TVN_GETDISPINFO
#define TLN_SETDISPINFO				TVN_SETDISPINFO
#define TLN_ITEMEXPANDING			TVN_ITEMEXPANDING		// expanding an item
#define TLN_ITEMEXPANDED			TVN_ITEMEXPANDED		// an item was expanded
#define TLN_DELETEITEM				TVN_DELETEITEM			// delete an item
#define TLN_BEGINCONTROL			TVN_BEGINLABELEDIT		// entry a build in control
#define TLN_ENDCONTROL				TVN_ENDLABELEDIT		// leave a build in control

#define	TLN_ITEMUPDATING			(TVN_FIRST-20)			// while a column of an item updating
#define TLN_ITEMUPDATED				(TVN_FIRST-21)			// after a column of an item updated
#define TLN_ITEMFINISHED			(TVN_FIRST-22)			// after all columns of an item updated

#define TLN_ITEMCHECK				(TVN_FIRST-27)			// check or uncheck an item
#define TLN_ITEMLOCK				(TVN_FIRST-28)			// lock or unlock an item
#define TLN_ITEMIMAGE				(TVN_FIRST-29)			// image or unimage an item

// TreeListItem Expand Consts
#define TLE_EXPAND					0						// expand the item
#define TLE_COLLAPSE				1						// collapse the item
#define TLE_TOGGLE					2						// toggle between expand & collapse
#define TLE_COLLAPSERESET			3						// collapse the item then reset it

// TreeListCtrl Consts
#define TLL_HEIGHT					16						// default height of the item
#define TLL_WIDTH					16						// default width of the item
#define TLL_BORDER					 2						// border width of the item
#define TLL_BUTTON					 4						// button width (half)

// Draw Consts
#define DEFAULT_HRGN				(HRGN)0x0000			// default hrgn

class CTreeListCtrl;
class CTreeListItem;

// Struct for Notify
typedef struct _NMTREELIST
{
    NMHDR			hdr;									// default struct
	HTREELISTITEM 	pItem;									// item pointer
	int				iCol;									// colimn index
} NMTREELIST, FAR* LPNMTREELIST;

typedef BOOL (WINAPI *lpfnUpdateLayeredWindow)(	HWND hWnd, HDC hdcDst, POINT *pptDst, 
												SIZE *psize,HDC hdcSrc, POINT *pptSrc, 
												COLORREF crKey, BLENDFUNCTION *pblend, DWORD dwFlags );

typedef BOOL (WINAPI *lpfnSetLayeredWindowAttributes)( HWND hwnd, COLORREF crKey, BYTE xAlpha, DWORD dwFlags );

// class CTreeListHeaderCtrl;

class CThreadTreeStat;

class __COMPONENTES_EXPORT__ CTreeListCtrl : public CWnd
{
private:
	DECLARE_DYNCREATE(CTreeListCtrl)

	friend class CTreeListHeaderCtrl;
// Construction
public:
	void SetCanShowStat(bool show=true);
	void SetCanSort(bool sort=true);
	void Sort(int columna);
	void SetCustomSort(bool custom=true);
	int  GetSortedColumn();
	bool GetAscendentSort();

	CTreeListCtrl();
	virtual ~CTreeListCtrl();
	BOOL Create( DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID );

	int		GetColumnType(int columna);
	void	ResetTipos();
	void	SetType(int columna, int tipo);
	void	SetMessageLista( CString message);
	void	SetCanShowMenu(bool show=true);
	void	AutosizeColumns(bool header=true);
	void	ShowStat();
	HTREELISTITEM AddItemEx(CString nombre, int columna, CString real);
	HTREELISTITEM AddItem(CString nombre, int columna, CString real);
	void SetSortColumn(int col=-1, bool ascendente=false);

protected:
	void PreSort(int col);
	BOOL Create();
	void SortSubItems(int columna, HTREELISTITEM parent, int tipo, bool clean=false);
	CFont	m_pFontBold;
	bool	m_bCanSort;
	bool	m_bCanStat;

// Members
protected:
	// back ground colors of normal status
	COLORREF			m_crBkColor;				// background color ( none client area )

	// back ground colors of selected item
	COLORREF			m_crBkSelectedNoFocus;		// background color of no focus selected row

	// normal text color
	COLORREF			m_crText;					// color of text

	// grid colors
	COLORREF			m_crGrid;					// color of grid
	COLORREF			m_crTreeLine;				// color of tree line

	// tree list style
	DWORD				m_dwStyle;					// style of tree list control

	// font & palette
	CFont		m_Font;		// font of control
	CPalette	m_Palette;	// palette of control

	// draw rects
	CRect	m_rcClient;		// rect of control client ( include header )
	CRect	m_rcHeader;		// rect of control header 
	CRect	m_rcTreeList;	// rect of treelist

	CRect	m_rcFocus;		// rect of focus item

protected:
	// objects of control
	CString					m_szCadena;
	bool					m_bCustomSort;
	int						m_nOrderBy;
	CTreeListHeaderCtrl		m_wndHeader;			// TreeList Header Control
	CString					m_szVacia;
	CUIntArray				m_nColumType;
	CNewMenu				*m_hMenuContextual;
	bool					m_bCanShowMenu;
	int						m_nSortedColumn;
	bool					m_bAscendente;
	CUIntArray				m_pKeys;
private:
	// image of control
	CImageList			m_imgCheck;					// CheckBox Image List ( Default )
	CImageList*			m_pImageList;				// ImageList ( Delault equ &m_imgTree )
	DWORD				m_dwState;					// State of the Control
	CThreadTreeStat		*m_pThread;
	CTreeListItemMap		m_pItemMap;
private:
	// column information of control
	CPtrArray			m_arColumns;				// All Columns
	CArray<int, int>	m_arShows;					// All Show Columns
	CArray<int, int>	m_arSorts;					// All Sort Columns

	// row information of control
	CPtrArray			m_arSelects;				// Selected Items
	int					m_iSelCol;					// Selected Column
	int					m_nItemHeight;				// Height of 
	HTREELISTITEM 		m_pFocusItem;				// Focus item
	HTREELISTITEM 		m_hSelect;
	HTREELISTITEM		m_pRootItem;
	BOOL				PreTranslateKeyDownMessage( MSG* pMsg );
	void				DoBanding(BOOL bLeft, UINT nFlags, CPoint point);
	void				UpdateSelectionForRect(LPCRECT pRect, UINT nFlags);
	inline void			UnSelectItem(HTREELISTITEM item);

// Attributes
public:
	// ***************************** CTreeCtrl ******************************
	int		GetCount();
	CUIntArray *GetKeys();
	void SetKey(int columna, bool primary=true);

	CImageList*	GetImageList();										// retrieve image list of global treelist
	void		SetImageList( CImageList* pImageList );				// set image list of global treelist

	HTREELISTITEM  GetNextItem( HTREELISTITEM  pItem, UINT nCode );

	BOOL		ItemHasChildren( HTREELISTITEM  pItem );

	HTREELISTITEM  GetChildItem( HTREELISTITEM  pItem );
	HTREELISTITEM  GetNextSiblingItem( HTREELISTITEM  pItem );
	HTREELISTITEM  GetPrevSiblingItem( HTREELISTITEM  pItem );
	HTREELISTITEM  GetParentItem( HTREELISTITEM  pItem );

	HTREELISTITEM  GetFirstVisibleItem();
	HTREELISTITEM  GetNextVisibleItem( HTREELISTITEM  pItem );
	HTREELISTITEM  GetPrevVisibleItem( HTREELISTITEM  pItem );

	HTREELISTITEM  GetSelectedItem();
	HTREELISTITEM  GetNextSelectedItem( HTREELISTITEM  pItem );

	HTREELISTITEM  GetRootItem();

	DWORD	GetItemState( HTREELISTITEM  pItem, DWORD nStateMask = 0xFFFFFFFF );
	void	SetItemState( HTREELISTITEM  pItem, DWORD dwAddState, DWORD dwRemoveState = 0 );

	void	GetItemImage( HTREELISTITEM  pItem, int& nImage, int& nExpandImage, int& nSelectedImage, int& nExpandSelectedImage );
	void	SetItemImage( HTREELISTITEM  pItem, int nImage, int nExpandImage, int nSelectedImage, int nExpandSelectedImage );

	LPCTSTR	GetItemText( HTREELISTITEM  pItem, int nSubItem = 0 );
	BOOL	SetItemText( HTREELISTITEM  pItem, LPCTSTR lpszText, int nSubItem = 0 );

	void	SetItemColor(HTREELISTITEM  pItem, COLORREF color);
	COLORREF GetItemColor(HTREELISTITEM pItem);

	void	SetItemBold(HTREELISTITEM  pItem, bool bold=true);

	int		GetChildCount(HTREELISTITEM  pItem);

	DWORD_PTR	GetItemData( HTREELISTITEM  pItem );
	void	SetItemData( HTREELISTITEM  pItem, DWORD_PTR dwData );

	DWORD_PTR	GetItemDataSort( HTREELISTITEM  pItem );
	void	SetItemDataSort( HTREELISTITEM  pItem, DWORD_PTR dwData );

	int		GetVisibleCount();

	int		SetItemHeight( int cyHeight = -1 );		// set height of row ( automatic )
	int		GetItemHeight();						// retrieve height of row

	COLORREF GetBkColor( int nColor = 0 );
	COLORREF SetBkColor( COLORREF clr, int nColor = 0 );

	COLORREF GetTextColor();
	COLORREF SetTextColor( COLORREF clr );
	
	int		GetCheck( HTREELISTITEM  pItem );
	void	SetCheck( HTREELISTITEM  pItem, BOOL bCheck = TRUE );

	int		GetLock( HTREELISTITEM  pItem );
	void	SetLock( HTREELISTITEM  pItem, BOOL bLock = TRUE );

	BOOL	GetItemRect( HTREELISTITEM  pItem, LPRECT lpRect );
	BOOL	GetItemRect( HTREELISTITEM  pItem, int iSubItem, LPRECT lpRect, BOOL bTextOnly );
	
	BOOL	EnsureVisible( HTREELISTITEM  pItem, int iSubItem );

	// ***************************** CListCtrl ******************************
	int		GetItemCount();
	int		GetNextItem( int nItem, int nFlags );

	POSITION	   GetFirstSelectedItemPosition();
	HTREELISTITEM  GetNextSelectedItem( POSITION& pos );

	int GetStringWidth( LPCTSTR lpsz );

	DWORD	SetColumnFormat( int nCol, DWORD dwAdd, DWORD dwRemove );
	DWORD	GetColumnFormat( int nCol, DWORD dwMask = 0xFFFFFFFF );

	DWORD	SetColumnModify( int nCol, DWORD dwModify );
	DWORD	GetColumnModify( int nCol );

	int		SetColumnWidth( int nCol, int nWidth, int nMin = 0, int nMax = 0 );
	int		GetColumnWidth( int nCol );

	int		SetColumnImage( int nCol, int iImage );
	int		GetColumnImage( int nCol );

	BOOL	SetColumnText( int nCol, LPCTSTR lpszText );
	LPCTSTR GetColumnText( int nCol );

	BOOL	SetColumnDefaultText( int nCol, LPCTSTR lpszText );
	LPCTSTR GetColumnDefaultText( int nCol );

	DWORD	GetItemState( int nItem, DWORD nStateMask = 0xFFFFFFFF );
	void	SetItemState( int nItem, DWORD dwAddState, DWORD dwRemoveState = 0 );

	void	GetItemImage( int nItem, int& nImage, int& nExpandImage, int& nSelectedImage, int& nExpandSelectedImage );
	void	SetItemImage( int nItem, int nImage, int nExpandImage, int nSelectedImage, int nExpandSelectedImage );

	LPCTSTR	GetItemTextEx( int nItem, int nSubItem = 0 );
	LPCTSTR	GetItemText( int nItem, int nSubItem = 0 );
	BOOL	SetItemText( int nItem, int nSubItem, LPCTSTR lpszText );

	DWORD	GetItemData( int nItem );
	void	SetItemData( int nItem, DWORD dwData );

	BOOL	GetViewRect( LPRECT lpRect );
	int		GetTopIndex();
	int		GetCountPerPage();
	BOOL	GetOrigin( LPPOINT lpPoint );

	UINT	GetSelectedCount();
	BOOL	SetColumnOrderArray( int iCount, LPINT piArray );
	BOOL	GetColumnOrderArray( LPINT piArray, int iCount = -1 );

	CTreeListHeaderCtrl* GetHeaderCtrl();

	int GetSelectionMark();
	int SetSelectionMark( int iIndex );

	// *************************** CTreeListCtrl ****************************
	void	SetFont();
	CFont*	GetFont();

	DWORD	SetStyle( DWORD dwStyle );
	DWORD	GetStyle();

	DWORD	GetState( DWORD nStateMask = 0xFFFFFFFF );
	void	SetState( DWORD dwAddState, DWORD dwRemoveState = 0 );

	int		GetSelectColumn();

	int		GetColumnCount();

	void	ExchangeItem( HTREELISTITEM  pItemA, HTREELISTITEM  pItemB );
	void	MoveAfterItem( HTREELISTITEM  pItem, HTREELISTITEM  pAfter = NULL );

	CNewMenu	*GetMenuContextual();
	CNewMenu	*GetMenuContextual(CNewMenu	*menu);

protected:
	int		GetWidth();								// retrieve display high of control
	int		GetHeight();							// retrieve display width of control

	int		GetColumns();							// retrieve columns

	int		GetVisibleCount( HTREELISTITEM  pParent );

	BOOL	GetItemRectTree( HTREELISTITEM  pItem, LPRECT lpRect );
	BOOL	GetItemRectMain( HTREELISTITEM  pItem, LPRECT lpRect );

private:
	void	StatChildAdd( HTREELISTITEM  pItem, int nChild, int nVisibleChild );
	void	StatChildDel( HTREELISTITEM  pItem, int nChild, int nVisibleChild );
	void	StatExpand( HTREELISTITEM  pItem );
	void	StatCollapse( HTREELISTITEM  pItem );
	void	SetItemParentStatus( HTREELISTITEM  pItem, DWORD dwMask );
	void	SetItemChildStatus( HTREELISTITEM  pItem, DWORD dwAdd, DWORD dwRemove = 0 );

	void	SetItemCheck( HTREELISTITEM  pItem, BOOL bAutoCheck = TRUE );
	void	SetItemLock( HTREELISTITEM  pItem, BOOL bAutoLock = TRUE );

// Operations
public:
	BOOL	InsertColumn( LPCTSTR lpszColumnHeading, DWORD dwFormat = TLF_DEFAULT_LEFT, int nWidth = 0, int iImage = -1, int nMin = 0, int nMax = 0);
	BOOL	DeleteColumn( int iCol );

	int		InsertItem( int nItem, LPCTSTR lpszItem );
	BOOL	DeleteItem( int nItem );

	HTREELISTITEM 	InsertItem( LPCTSTR lpszItem, HTREELISTITEM  pParent = TLI_ROOT, HTREELISTITEM  pInsertAfter = TLI_LAST );
	BOOL			DeleteItem( HTREELISTITEM  pItem = TLI_ROOT );
	BOOL			DeleteAllItems();

	BOOL	Expand( HTREELISTITEM  pItem, int nCode );
	BOOL	Select( HTREELISTITEM  pItem, int nCode );

	HTREELISTITEM 	HitTest( CPoint pt, int* pFlag = NULL, int* pSubItem = NULL, CRect* prcText = NULL );

	void	SelectItem( HTREELISTITEM  pItem, int iSubItem = 0, BOOL bClearLastSelected = TRUE );
	void	ActiveItem( HTREELISTITEM  pItem, int iSubItem = 0 );

	HTREELISTITEM   GetTreeListItemEx( int nItem );
	int				GetTreeListItemEx( HTREELISTITEM  pItem );

protected:
	void SelectRange(HTREELISTITEM hFirst, HTREELISTITEM hLast, BOOL bOnly = TRUE);
	HTREELISTITEM 	InsertItemNew	( LPCTSTR lpszItem, HTREELISTITEM  pParent );
	HTREELISTITEM 	InsertItemFirst	( LPCTSTR lpszItem, HTREELISTITEM  pParent );
	HTREELISTITEM 	InsertItemNext	( LPCTSTR lpszItem, HTREELISTITEM  pParent, HTREELISTITEM  pInsertAfter );
	HTREELISTITEM 	GetLastChildItem( HTREELISTITEM  pParent = TLI_ROOT );
	HTREELISTITEM 	GetSortChildItem( HTREELISTITEM  pParent = TLI_ROOT );
	HTREELISTITEM 	GetValidChildItem( HTREELISTITEM  pParent, HTREELISTITEM  pChild);

	BOOL			Expand( HTREELISTITEM  pItem, int nCode, int iSubItem );

	HTREELISTITEM 	HitTest( CPoint pt, int* pFlag, int* pSubItem, CRect* prcText, CRect rcItem, HTREELISTITEM  pItem );

private:
	HTREELISTITEM 	InsertItemX( HTREELISTITEM  pParent, HTREELISTITEM  pNewItem );
	BOOL			DeleteItemX( HTREELISTITEM  pItem );
	BOOL			DeleteItemFast( HTREELISTITEM  pItem );

	HTREELISTITEM 	GetFirstShowItem( int nShowItem );
	HTREELISTITEM 	GetNextShowItem( HTREELISTITEM  pItem, BOOL bChild = TRUE );
	HTREELISTITEM 	GetNextVisibleItem( HTREELISTITEM  pItem, BOOL bChild );
	HTREELISTITEM 	GetPrevVisibleItem( HTREELISTITEM  pItem, BOOL bChild );
	HTREELISTITEM 	GetLastVisibleItem( HTREELISTITEM  pItem );

	HTREELISTITEM 	HitTestTree( CPoint pt, int* pFlag, CRect* prcText, CRect rcColumn, HTREELISTITEM  pItem );
	HTREELISTITEM 	HitTestMain( CPoint pt, int* pFlag, CRect* prcText, CRect rcColumn, HTREELISTITEM  pItem );
	HTREELISTITEM 	HitTestList( CPoint pt, int* pFlag, CRect* prcText, CRect rcColumn, HTREELISTITEM  pItem );

	HTREELISTITEM   GetTreeListItem( int nItem );
	int				GetTreeListItem( HTREELISTITEM  pItem );
	HTREELISTITEM	GetNextItemEx(HTREELISTITEM hItem);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTreeListCtrl)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
protected:
	LRESULT	Notify( DWORD dwMessage, HTREELISTITEM  pItem, int iCol );
	void	Layout();
	void	DrawCtrl( CDC* pDC );
	void	DrawBkgnd( CDC* pDC, CRect rcClip );
	void	DrawBkgndBmp( CDC* pDC, CRect rcClip, CBitmap* pBkgnd );
	void	DrawItems( CDC* pDC, CRect rcClip );
	void	DrawItem( CDC* pDC, CRect rcItem, HTREELISTITEM  pItem );
	void	DrawGrid( CDC* pDC, CRect rcItem );
	void	DrawItemBkgnd( CDC* pDC, CRect rcItem, HTREELISTITEM  pItem );
	void	DrawItemExclude( CDC* pDC, CRect rcItem, HTREELISTITEM  pItem );
	void	DrawItemTree( CDC* pDC, CRect rcColumn, HTREELISTITEM  pItem, int iCol );
	void	DrawItemMain( CDC* pDC, CRect rcColumn, HTREELISTITEM  pItem, int iCol );
	void	DrawItemList( CDC* pDC, CRect rcColumn, HTREELISTITEM  pItem, int iCol );
	void	DrawItemTreeLine( CDC* pDC, CRect rcColumn, CRect rcTree, DWORD dwFormat );
	void	DrawItemTreeButton( CDC* pDC, CRect rcColumn, CRect rcTree, DWORD dwFormat );
	void	DrawItemTreeText( CDC* pDC, CRect rcBkgnd, HTREELISTITEM  pItem, int iCol );
	void	DrawItemListText( CDC* pDC, CRect rcBkgnd, HTREELISTITEM  pItem, int iCol );
	void	DrawItemText( CDC* pDC, CRect rcBkgnd, CRect rcText, HTREELISTITEM  pItem, int iCol );
	void	SetAllScroll();
	
private:
	void	SetHorzScroll( CRect* pPreClient );
	void	SetVertScroll( CRect* pPreClient );

	BOOL	IsInSelectsTree( HTREELISTITEM  pItem );

	// Generated message map functions
protected:
	//{{AFX_MSG(CTreeListCtrl)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnEnable(BOOL bEnable);
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	afx_msg void OnNcPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnCancelMode();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSysColorChange();
	afx_msg void ExpandAll();
	afx_msg void CollapseAll();
	afx_msg void CopyAll();
	bool CopyToClipBoard(CString &valor);
	HTREELISTITEM FindItem(CString cadena, HTREELISTITEM hItem);
	HTREELISTITEM FindChildItem(const CString &cadena, HTREELISTITEM pParent, int index=0, HTREELISTITEM pFrom=NULL);
	HTREELISTITEM InsertSorted(const CString &texto, HTREELISTITEM pParent, bool duplicate=false);

	afx_msg LRESULT OnCopy(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUndo(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCut(WPARAM wParam, LPARAM lParam);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TREELISTCTRL_H__55B6E4E9_1E57_4FEE_AA05_77C052ACE9B5__INCLUDED_)

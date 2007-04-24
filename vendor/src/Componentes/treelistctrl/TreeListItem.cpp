// TreeListItem.cpp: implementation of the CTreeListItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "treelistctrl/TreeListItem.h"
# include "EditFloat.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTreeListItem::CTreeListItem( int nSubItem ) :
	m_pParent( NULL ),
	m_pChild( NULL ),
	m_pPrev( NULL ),
	m_pNext( NULL ),
	m_dwState( TLIS_SHOWCHECKBOX | TLIS_SHOWLOCKBOX | TLIS_CHECKED_NULL | TLIS_LOCKED_NULL ),
	m_dwData( 0 ),
	m_nLevel( 0 ),
	m_nChild( 1 ),
	m_nColumna( -1),
	m_bAscendente(true),
	m_nTipo(1),
	m_nVisibleChild( 1 ),
	m_dwDataSort(0),
	m_clrTexto(0),
	m_bBold(false)
{
	for( int iSubItem = 0; iSubItem < nSubItem; iSubItem++ )
	{
		m_arSubItems.Add("");
	}
}

CTreeListItem::~CTreeListItem()
{
}

void CTreeListItem::Expand()
{
	SetState( TLIS_EXPANDED, 0 );
}

void CTreeListItem::Collapse()
{
	SetState( 0, TLIS_EXPANDED );
}

DWORD CTreeListItem::GetState( DWORD dwStateMask )
{
	return m_dwState&dwStateMask;
}

void CTreeListItem::SetState( DWORD dwAddStatus, DWORD dwRemoveStatus )
{
	if( dwAddStatus&TLIS_EXPANDED )
		dwAddStatus |= TLIS_EXPANDEDONCE;

	if( dwRemoveStatus&TLIS_EXPANDEDONCE )
		dwRemoveStatus |= TLIS_EXPANDED;

	m_dwState |= dwAddStatus;
	m_dwState &=~dwRemoveStatus;
	return;
}

void CTreeListItem::GetImage( int& nImage, int& nSelectedImage, int& nExpandImage, int& nExpandSelectedImage )
{
	nImage					= m_nImage;
	nSelectedImage			= m_nSelectedImage;
	nExpandImage			= nExpandImage;
	nExpandSelectedImage	= nExpandSelectedImage;
}

void CTreeListItem::SetImage( int nImage, int nSelectedImage, int nExpandImage, int nExpandSelectedImage )
{
	m_nImage				= nImage;
	m_nSelectedImage		= nSelectedImage;
	m_nExpandImage			= nExpandImage;
	m_nExpandSelectedImage	= nExpandSelectedImage;
}

LPCTSTR CTreeListItem::GetText( int nIndex )
{
	if( nIndex >= m_arSubItems.GetSize() )
	{
		return NULL;
	}
	return m_arSubItems[nIndex];
}

BOOL CTreeListItem::SetText( LPCTSTR lpszItem, int nIndex )
{
	if( nIndex >= m_arSubItems.GetSize() )
		return FALSE;
	m_arSubItems.SetAt(nIndex, lpszItem);
	return TRUE;
}

DWORD_PTR CTreeListItem::GetData()
{
	return m_dwData;
}

void CTreeListItem::SetData( DWORD_PTR dwData )
{
	m_dwData = dwData;
}

BOOL CTreeListItem::IsShowTreeImage()
{
	if( m_dwState&TLIS_SHOWTREEIMAGE )
		return TRUE;
	else
		return FALSE;
}

void CTreeListItem::ShowTreeImage( BOOL bShow )
{
	if( bShow )
		m_dwState |= TLIS_SHOWTREEIMAGE;
	else
		m_dwState &=~TLIS_SHOWTREEIMAGE;
}

BOOL CTreeListItem::IsShowCheckBox()
{
	if( m_dwState&TLIS_SHOWCHECKBOX )
		return TRUE;
	else
		return FALSE;
}

void CTreeListItem::ShowCheckBox( BOOL bShow )
{
	if( bShow )
		m_dwState |= TLIS_SHOWCHECKBOX;
	else
		m_dwState &=~TLIS_SHOWCHECKBOX;
}

BOOL CTreeListItem::IsShowLockBox()
{
	if( m_dwState&TLIS_SHOWLOCKBOX )
		return TRUE;
	else
		return FALSE;
}

void CTreeListItem::ShowLockBox( BOOL bShow )
{
	if( bShow )
		m_dwState |= TLIS_SHOWLOCKBOX;
	else
		m_dwState &=~TLIS_SHOWLOCKBOX;
}

BOOL CTreeListItem::GetSelected()
{
	if( GetState()&TLIS_SELECTED )
		return TRUE;
	else
		return FALSE;
}

void CTreeListItem::SetSelected( BOOL bSelected )
{
	if( bSelected )
		SetState( TLIS_SELECTED, 0 );
	else
		SetState( 0, TLIS_SELECTED );
}

int CTreeListItem::GetCheck()
{
	ASSERT( GetState()&TLIS_CHECKED || GetState()&TLIS_CHECKED_NULL );
	
	if( ( GetState()&TLIS_CHECKEDMASK) == (DWORD)TLIS_CHECKEDPART )
		return 2;
	
	if( GetState()&TLIS_CHECKED )
		return 1;

	return 0;
}

void CTreeListItem::SetCheck( BOOL bCheck )
{
	if( bCheck )
		SetState( TLIS_CHECKED, TLIS_CHECKED_NULL );
	else
		SetState( TLIS_CHECKED_NULL, TLIS_CHECKED );
}

int CTreeListItem::GetLock()
{
	ASSERT( GetState()&TLIS_LOCKED || GetState()&TLIS_LOCKED_NULL );

	if( ( GetState()&TLIS_LOCKEDMASK) == (DWORD)TLIS_LOCKEDPART )
		return 2;
	
	if( GetState()&TLIS_LOCKED )
		return 1;

	return 0;
}

void CTreeListItem::SetLock( BOOL bLock )
{
	if( bLock )
		SetState( TLIS_LOCKED, TLIS_LOCKED_NULL );
	else
		SetState( TLIS_LOCKED_NULL, TLIS_LOCKED );
}

DWORD_PTR CTreeListItem::GetDataSort() {

	return m_dwDataSort;
}

void CTreeListItem::SetDataSort(DWORD_PTR dwData) {

	m_dwDataSort = dwData;
}

void CTreeListItem::SetColor(COLORREF color) {

	m_clrTexto= color;
}

void CTreeListItem::SetBold(bool bold) {

	m_bBold= bold;
}

void CTreeListItem::ReSize(int col) {

	while( col>m_arSubItems.GetCount()) {
		m_arSubItems.Add("");
	}
}

/*
void CTreeListItem::SetImageList( CImageList* pImageList )
{
	ASSERT( pImageList != NULL );
	m_pImageList = pImageList;
}

void CTreeListItem::SetImage( int nIndex )
{
	return;
}

int CTreeListItem::GetImage()
{
	return 0;
}
*/

int CTreeListItem::Compare(const void *item1, const void *item2) {

	CTreeListItem *pItem1=*(CTreeListItem **)item1;
	CTreeListItem *pItem2=*(CTreeListItem **)item2;
	int sOrder= pItem1->m_bAscendente ? 1 : -1;
	CString str1= pItem1->GetText(pItem1->m_nColumna);
	CString str2= pItem2->GetText(pItem2->m_nColumna);

	if( pItem1->m_nTipo==100 || (!str1.IsEmpty() && !str2.IsEmpty())) {

		COleDateTime t1, t2;
		switch( pItem1->m_nTipo) {
			case 2: // numero
				return (atol(str1) - atol(str2))*sOrder;
			case 3: // double
				return (atof(CEditFloat::GetNormal(str1)) < atof(CEditFloat::GetNormal(str2)) ? -1 : 1)*sOrder;
			case 4: // fecha
				if (t1.ParseDateTime(str1) && t2.ParseDateTime(str2))
					return (t1 < t2 ? -1 : 1 )*sOrder;
				else
					return 0;
				break;
			case 100: // reservado para ordenar solo los elementos seleccionados
				return pItem1->GetDataSort()-pItem2->GetDataSort();
				break;
		}
	}
	return sOrder*strcmp(str1, str2);
}

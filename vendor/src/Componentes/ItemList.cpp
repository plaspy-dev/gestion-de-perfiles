# include "stdafx.h"
# include "itemlist.h"

CItemList::CItemList() {
	m_nImagen=-1;
	m_bCheck=false;
	m_dwData=0;
	m_dwState=0;
	m_nIndent=0;
}

CItemList::CItemList(LPCTSTR lpszItem,int nImage) {
	
	m_nImagen= nImage;
	m_bCheck=false;
	m_dwData=0;
	m_dwState=0;
	m_nIndent=0;
	m_szTextos.Add(lpszItem);
}

CItemList::~CItemList() {

}

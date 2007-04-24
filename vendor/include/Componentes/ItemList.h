//***************************************************************************
//
// AUTHOR:  JUHU
//
// DESCRIPTION: Items de la lista virtual.
//
// ORIGINAL: 
//
//***************************************************************************
#pragma once

class CItemList
{
public:
	CItemList(void);
	CItemList(LPCTSTR lpszItem,int nImage);
public:
	~CItemList(void);
	int				m_nImagen;
	bool			m_bCheck;
	CStringArray	m_szTextos;
	DWORD			m_dwData;
	DWORD			m_dwState;
	int				m_nIndent;
};

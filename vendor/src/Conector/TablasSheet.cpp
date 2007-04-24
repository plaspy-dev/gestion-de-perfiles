// TablasSheet.cpp : implementation file
//

#include "stdafx.h"
#include "Conector.h"
#include "TablasSheet.h"


// CTablasSheet

IMPLEMENT_DYNAMIC(CTablasSheet, CPropertySheet)

CTablasSheet::CTablasSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	AddPage(&m_pBienvenido);
	m_psh.dwFlags |= PSH_WIZARD97;
	m_szServidor="localhost";
	m_szUsuario="root";
	m_szPuerto="3306";
	m_szDB= CApplicacion::GetAppName();
	m_szDB.MakeLower();
	m_bUsuarioSVE=true;
	m_szUsuarioSVE="Administrador";
	m_bUsarDB=true;
	m_nTipoInstalacion=0;
}

CTablasSheet::CTablasSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage, HBITMAP hbmWatermark,
						   HPALETTE hpalWatermark, HBITMAP hbmHeader)
:CPropertySheet(pszCaption, pParentWnd, iSelectPage, hbmWatermark,hpalWatermark, hbmHeader)
{
	AddPage(&m_pBienvenido);
	m_psh.dwFlags |= PSH_WIZARD97;
	m_szServidor="localhost";
	m_szUsuario="root";
	m_szPuerto="3306";
	m_szDB= CApplicacion::GetAppName();
	m_szDB.MakeLower();
	m_bUsuarioSVE=true;
	m_szUsuarioSVE="Administrador";
	m_bUsarDB=true;
	m_nTipoInstalacion=0;
}

CTablasSheet::~CTablasSheet()
{
}


BEGIN_MESSAGE_MAP(CTablasSheet, CPropertySheet)
END_MESSAGE_MAP()


// CTablasSheet message handlers
void CTablasSheet::SetTipo(int tipo) {

	m_nTipoInstalacion= tipo;
	while(GetPageCount()>1)
		RemovePage(1);
	switch(m_nTipoInstalacion) {
		case 0: // autonoma.
			AddPage(&m_pTerminar);
			break;
		case 1: // si tiene servidor.
			AddPage(&m_pServidorSolo);
			AddPage(&m_pTerminar);
			break;
		case 2: // configurar servidor.
			AddPage(&m_pServidor);
			AddPage(&m_pUsuario);
			AddPage(&m_pTerminar);
			break;
	}
}

int CTablasSheet::GetTipo() const {

	return m_nTipoInstalacion;
}

#pragma once
# include "BienvenidoPP.h"
# include "ServidorPP.h"
# include "UsuarioPP.h"
# include "TerminarPP.h"
# include "ServidorSoloPP.h"
// CTablasSheet

class CTablasSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CTablasSheet)

public:
	CTablasSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CTablasSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0, HBITMAP hbmWatermark = NULL,
		HPALETTE hpalWatermark = NULL, HBITMAP hbmHeader = NULL);
	virtual ~CTablasSheet();
	CBienvenidoPP	m_pBienvenido;
	CServidorPP		m_pServidor;
	CUsuarioPP		m_pUsuario;
	CTerminarPP		m_pTerminar;
	CServidorSoloPP		m_pServidorSolo;
	CString			m_szServidor;
	CString			m_szUsuario;
	CString			m_szContrasena;
	CString			m_szPuerto;
	CString			m_szDB;
	bool			m_bUsuarioSVE;
	CString			m_szUsuarioSVE;
	CString			m_szContrasenaSVE;
	bool			m_bUsarDB;
protected:
	int				m_nTipoInstalacion;
protected:
	DECLARE_MESSAGE_MAP()
public:
	void SetTipo(int tipo);
	int GetTipo() const;
};



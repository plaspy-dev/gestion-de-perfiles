#pragma once
#include "afxwin.h"
# include "resource.h"
// CDlgEditar dialog

class CDlgEditar : public CDialog
{
	DECLARE_DYNAMIC(CDlgEditar)

public:
	CDlgEditar(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgEditar();

// Dialog Data
	enum { IDD = IDD_DIALOGUSUARIO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_szContrasena;
	CUsuario *m_pActual;
	CStringArray	*m_saNombres;
	CStringArray	*m_saDescripciones;
	CUIntArray		*m_uaIDS;
	CString			m_szGrupo;
	DWORD			m_dwGrupo;
	CString			m_szNombre;
	CString			m_szVendedor;

	CEditXP		m_ENombre;
	CEditXP		m_EGrupos;
	CButtonXP	m_BSeleccionar;
	CButtonXP	m_BAceptar;
	CButtonXP	m_BCancelar;
	CButtonXP	m_BContrasena;
	afx_msg void OnEnChangeEditnombre();
	afx_msg void OnEnChangeEditgrupos();
	afx_msg void OnBnClickedButtonseleccionar();
	afx_msg void OnBnClickedButtoncontrasena();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};

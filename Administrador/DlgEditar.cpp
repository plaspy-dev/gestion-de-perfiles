// DlgEditar.cpp : implementation file
//

#include "stdafx.h"
# include "Administrador.h"
#include "DlgEditar.h"
# include "DlgPasswd.h"


IMPLEMENT_DYNAMIC(CDlgEditar, CDialog)
CDlgEditar::CDlgEditar(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgEditar::IDD, pParent)
{
	m_pActual= NULL;
}

CDlgEditar::~CDlgEditar()
{
}

void CDlgEditar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDITNOMBRE, m_ENombre);
	DDX_Control(pDX, IDC_EDITGRUPOS, m_EGrupos);
	DDX_Control(pDX, IDC_BUTTONSELECCIONAR, m_BSeleccionar);
	DDX_Control(pDX, IDOK, m_BAceptar);
	DDX_Control(pDX, IDCANCEL, m_BCancelar);
	DDX_Control(pDX, IDC_BUTTONCONTRASENA, m_BContrasena);
}


BEGIN_MESSAGE_MAP(CDlgEditar, CDialog)
	ON_EN_CHANGE(IDC_EDITNOMBRE, OnEnChangeEditnombre)
	ON_EN_CHANGE(IDC_EDITGRUPOS, OnEnChangeEditgrupos)
	ON_BN_CLICKED(IDC_BUTTONSELECCIONAR, OnBnClickedButtonseleccionar)
	ON_BN_CLICKED(IDC_BUTTONCONTRASENA, OnBnClickedButtoncontrasena)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgEditar message handlers

void CDlgEditar::OnEnChangeEditnombre()
{
	CString temp;
	m_ENombre.GetWindowText(temp);
	SetWindowText(temp);
	m_EGrupos.GetWindowText(temp);
	m_BAceptar.EnableWindow((m_ENombre.GetWindowTextLength() && CUsuario::GetGrupos(MarcasToNumeros( *m_saNombres, *m_uaIDS, temp))!=0));
}

void CDlgEditar::OnEnChangeEditgrupos()
{
	OnEnChangeEditnombre();
}

void CDlgEditar::OnBnClickedButtonseleccionar()
{
	CString temp;
	m_EGrupos.GetWindowText(temp);
	if( DlgListado( "Grupos", m_saNombres, &temp, m_saDescripciones)==IDOK) 
		m_EGrupos.SetWindowText(temp);
}

void CDlgEditar::OnBnClickedButtoncontrasena()
{
	CDlgPasswd dlg;
	if( dlg.DoModal()==IDOK) 
		m_szContrasena= dlg.m_szContrasena;
}

BOOL CDlgEditar::OnInitDialog()
{
	CDialog::OnInitDialog();

	if( m_pActual) {
		m_ENombre.SetWindowText(m_pActual->GetNombre());
		SetWindowText(m_pActual->GetNombre());
	} else {
		SetWindowText("Nuevo Usuario");
	}
	m_EGrupos.SetWindowText(m_szGrupo);
	m_ENombre.SetLimitText(255);
	OnEnChangeEditnombre();
	return TRUE;
}

void CDlgEditar::OnBnClickedOk()
{
	CString temp;
	m_ENombre.GetWindowText(m_szNombre);
	m_EGrupos.GetWindowText(m_szGrupo);
	m_dwGrupo= CUsuario::GetGrupos(MarcasToNumeros( *m_saNombres, *m_uaIDS, m_szGrupo));
	if( m_szVendedor=="(Todos)")
		m_szVendedor.Empty();

	if( m_szContrasena.IsEmpty() || (m_pActual && m_pActual->GetNombre()!=m_szNombre)) {
		m_szContrasena.Empty();
		OnBnClickedButtoncontrasena();
		if( m_szContrasena.IsEmpty())
			return;
	}
	OnOK();
}

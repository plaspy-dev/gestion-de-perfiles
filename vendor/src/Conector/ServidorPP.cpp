// ServidorPP.cpp : implementation file
//

#include "stdafx.h"
#include "Conector.h"
#include "ServidorPP.h"
#include "TablasSheet.h"

// CServidorPP dialog

IMPLEMENT_DYNAMIC(CServidorPP, CPropertyPage)

CServidorPP::CServidorPP()
	: CPropertyPage(CServidorPP::IDD)
{
	m_psp.dwFlags|= PSP_DEFAULT|PSP_USEHEADERTITLE|PSP_USEHEADERSUBTITLE;
	m_psp.pszHeaderTitle= "Servidor de MySQL";
	m_psp.pszHeaderSubTitle="Este servidor ya tiene instalado MySQL, pero vamos a configurarlo.";
}

CServidorPP::~CServidorPP()
{
}

void CServidorPP::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ESERVIDOR, m_EServidor);
	DDX_Control(pDX, IDC_EUSUARIO, m_EUsuario);
	DDX_Control(pDX, IDC_ECONTRASENA, m_EContrasena);
}


BEGIN_MESSAGE_MAP(CServidorPP, CPropertyPage)
	ON_EN_CHANGE(IDC_ESERVIDOR, &CServidorPP::OnEnChangeEservidor)
	ON_BN_CLICKED(IDC_BAVANZADO, &CServidorPP::OnBnClickedBavanzado)
	ON_EN_CHANGE(IDC_EUSUARIO, &CServidorPP::OnEnChangeEusuario)
END_MESSAGE_MAP()


// CServidorPP message handlers

void CServidorPP::OnEnChangeEservidor()
{
	CPropertySheet* parent = (CPropertySheet*)GetParent();
	if(parent) 
		parent->SetWizardButtons(m_EServidor.GetWindowTextLength() ? PSWIZB_NEXT |PSWIZB_BACK : PSWIZB_BACK);
}

void CServidorPP::OnBnClickedBavanzado()
{
	m_pAvanzado.DoModal();	
}

void CServidorPP::OnEnChangeEusuario()
{
	
}

BOOL CServidorPP::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	m_EServidor.SetWindowText("localhost");
	m_EUsuario.SetWindowText("root");
#ifdef _DEBUG
	m_EContrasena.SetWindowText("DebugDemo_2026!");
#endif
	return TRUE;
}

BOOL CServidorPP::OnSetActive()
{
	OnEnChangeEservidor();
	return CPropertyPage::OnSetActive();
}

LRESULT CServidorPP::OnWizardNext()
{
	CTablasSheet* parent = (CTablasSheet*)GetParent();	
	if(parent) {
		m_EServidor.GetWindowText(parent->m_szServidor);
		m_EUsuario.GetWindowText(parent->m_szUsuario);
		m_EContrasena.GetWindowText(parent->m_szContrasena);
		parent->m_szPuerto= m_pAvanzado.m_szPuerto;
		parent->m_szDB= m_pAvanzado.m_szDB;
	}
	return CPropertyPage::OnWizardNext();
}

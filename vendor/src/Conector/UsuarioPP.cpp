// UsuarioPP.cpp : implementation file
//

#include "stdafx.h"
#include "Conector.h"
#include "UsuarioPP.h"
#include "TablasSheet.h"

// CUsuarioPP dialog

IMPLEMENT_DYNAMIC(CUsuarioPP, CPropertyPage)

CUsuarioPP::CUsuarioPP()
	: CPropertyPage(CUsuarioPP::IDD)
{
	m_psp.dwFlags|= PSP_DEFAULT|PSP_USEHEADERTITLE|PSP_USEHEADERSUBTITLE;
	m_psp.pszHeaderTitle= "Usuario de la aplicación";
	m_psp.pszHeaderSubTitle="Se va a crear un usuario para la aplicación.";
}

CUsuarioPP::~CUsuarioPP()
{
}

void CUsuarioPP::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CCONFIGURAR, m_CConfigurar);
	DDX_Control(pDX, IDC_EUSUARIO, m_EUsuario);
	DDX_Control(pDX, IDC_ECONTRASENA, m_EContrasena);
	DDX_Control(pDX, IDC_ECONFIRMAR, m_EConfirmar);
	DDX_Control(pDX, IDC_SMENSAJEU, m_SMensaje);
	DDX_Control(pDX, IDC_SAYUDA, m_SAyuda);
	DDX_Control(pDX, IDC_SMENSAJE, m_SMensajeCheck);
}


BEGIN_MESSAGE_MAP(CUsuarioPP, CPropertyPage)
	ON_BN_CLICKED(IDC_CCONFIGURAR, &CUsuarioPP::OnBnClickedCconfigurar)
	ON_EN_CHANGE(IDC_EUSUARIO, &CUsuarioPP::OnEnChangeEusuario)
	ON_EN_CHANGE(IDC_ECONTRASENA, &CUsuarioPP::OnEnChangeEcontrasena)
	ON_EN_CHANGE(IDC_ECONFIRMAR, &CUsuarioPP::OnEnChangeEconfirmar)
	ON_STN_CLICKED(IDC_SMENSAJE, &CUsuarioPP::OnStnClickedSmensaje)
END_MESSAGE_MAP()


// CUsuarioPP message handlers

void CUsuarioPP::OnBnClickedCconfigurar()
{
	m_EUsuario.EnableWindow(m_CConfigurar.GetCheck());
	m_EContrasena.EnableWindow(m_CConfigurar.GetCheck());
	m_EConfirmar.EnableWindow(m_CConfigurar.GetCheck());
	if(!m_CConfigurar.GetCheck()) {
		m_SMensaje.SetWindowText("");
		CPropertySheet* parent = (CPropertySheet*)GetParent();
		if(parent) 
			parent->SetWizardButtons(PSWIZB_NEXT |PSWIZB_BACK);
	} else {
		OnEnChangeEusuario();
	}
}

void CUsuarioPP::OnEnChangeEusuario()
{
	CPropertySheet* parent = (CPropertySheet*)GetParent();
	if(!parent)
		return;
	if(!m_EUsuario.GetWindowTextLength()) {

		m_SMensaje.SetWindowText("Por favor ingrese un nombre de usuario.");
		parent->SetWizardButtons(PSWIZB_BACK);
		return;
	}
	CString password, confirma;
	m_EContrasena.GetWindowText(password);
	m_EConfirmar.GetWindowText(confirma);
	if(password.IsEmpty() && confirma.IsEmpty()) {

		m_SMensaje.SetWindowText("Por favor ingrese una contraseña.");
		parent->SetWizardButtons(PSWIZB_BACK);
		return;
	}
	if(password!=confirma) {
		m_SMensaje.SetWindowText("Las contraseñas no coinciden.");
		parent->SetWizardButtons(PSWIZB_BACK);
		return;
	}
	m_SMensaje.SetWindowText("");
	parent->SetWizardButtons(PSWIZB_BACK | PSWIZB_NEXT);
}

void CUsuarioPP::OnEnChangeEcontrasena()
{
	OnEnChangeEusuario();
}

void CUsuarioPP::OnEnChangeEconfirmar()
{
	OnEnChangeEusuario();
}

BOOL CUsuarioPP::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	m_CConfigurar.SetCheck(1);
	m_EUsuario.SetWindowText("Administrador");
	CString texto;
	texto.Format("Configurar el usuario administrador de %s. Si ya tiene un usuario administrador omita este paso.", CApplicacion::GetAppName());
	m_SAyuda.SetWindowText(texto);
	texto.Format("Deseo configurar un usuario administrador para el uso de %s.", CApplicacion::GetAppName());
	m_SMensajeCheck.SetWindowText(texto);

	return TRUE;
}

BOOL CUsuarioPP::OnSetActive()
{	
	OnBnClickedCconfigurar();
	return CPropertyPage::OnSetActive();
}

void CUsuarioPP::OnStnClickedSmensaje()
{
	m_CConfigurar.SetCheck(!m_CConfigurar.GetCheck());
	OnBnClickedCconfigurar();
}

LRESULT CUsuarioPP::OnWizardNext()
{
	CTablasSheet* parent = (CTablasSheet*)GetParent();	
	if(parent) {
		parent->m_bUsuarioSVE= m_CConfigurar.GetCheck()==1;
		m_EUsuario.GetWindowText(parent->m_szUsuarioSVE);
		m_EContrasena.GetWindowText(parent->m_szContrasenaSVE);
	}
	return CPropertyPage::OnWizardNext();
}

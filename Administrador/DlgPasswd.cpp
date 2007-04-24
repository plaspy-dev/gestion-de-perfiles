// DlgPasswd.cpp : implementation file
//

#include "stdafx.h"
#include "DlgPasswd.h"


// CDlgPasswd dialog

IMPLEMENT_DYNAMIC(CDlgPasswd, CDialog)
CDlgPasswd::CDlgPasswd(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPasswd::IDD, pParent)
{
}

CDlgPasswd::~CDlgPasswd()
{
}

void CDlgPasswd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDITCONTRASENA, m_EContrasena);
	DDX_Control(pDX, IDC_EDITCONFIRMA, m_EConfirma);
	DDX_Control(pDX, IDOK, m_BOK);
	DDX_Control(pDX, IDCANCEL, m_BCancela);
}


BEGIN_MESSAGE_MAP(CDlgPasswd, CDialog)
	ON_EN_CHANGE(IDC_EDITCONTRASENA, OnEnChangeEditcontrasena)
	ON_EN_CHANGE(IDC_EDITCONFIRMA, OnEnChangeEditcontrasena)
END_MESSAGE_MAP()


// CDlgPasswd message handlers

void CDlgPasswd::OnEnChangeEditcontrasena()
{
	CString contrasena;
	m_EConfirma.GetWindowText(contrasena);
	m_EContrasena.GetWindowText(m_szContrasena);
	m_BOK.EnableWindow((!contrasena.IsEmpty() && m_szContrasena==contrasena));
}


BOOL CDlgPasswd::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_BOK.EnableWindow(true);
	m_EConfirma.SetLimitText(16);
	m_EContrasena.SetLimitText(16);
	return TRUE;
}

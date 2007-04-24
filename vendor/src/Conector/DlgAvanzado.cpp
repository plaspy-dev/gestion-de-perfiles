// DlgAvanzado.cpp : implementation file
//

#include "stdafx.h"
#include "Conector.h"
#include "DlgAvanzado.h"
# include <resource.h>

// CDlgAvanzado dialog

IMPLEMENT_DYNAMIC(CDlgAvanzado, CDialog)

CDlgAvanzado::CDlgAvanzado(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAvanzado::IDD, pParent)
{
	m_szDB=CApplicacion::GetAppName();
	m_szDB.MakeLower();
	m_szPuerto="3306";
}

CDlgAvanzado::~CDlgAvanzado()
{
}

void CDlgAvanzado::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EBASEDEDATOS, m_EBaseDeDatos);
	DDX_Control(pDX, IDC_EPUERTO, m_EPuerto);
	DDX_Control(pDX, IDOK, m_BOK);
	DDX_Control(pDX, IDC_SNOMBREDATA, m_SNombreData);
}


BEGIN_MESSAGE_MAP(CDlgAvanzado, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgAvanzado::OnBnClickedOk)
	ON_EN_CHANGE(IDC_EBASEDEDATOS, &CDlgAvanzado::OnEnChangeEbasededatos)
	ON_EN_CHANGE(IDC_EPUERTO, &CDlgAvanzado::OnEnChangeEpuerto)
END_MESSAGE_MAP()


// CDlgAvanzado message handlers

void CDlgAvanzado::OnBnClickedOk()
{
	m_EPuerto.GetWindowText(m_szPuerto);
	m_EBaseDeDatos.GetWindowText(m_szDB);
	if(m_szPuerto.IsEmpty())
		m_szPuerto="3306";
	OnOK();
}

void CDlgAvanzado::OnEnChangeEbasededatos()
{
	m_BOK.EnableWindow(m_EBaseDeDatos.GetWindowTextLength());
}

void CDlgAvanzado::OnEnChangeEpuerto()
{	
}

BOOL CDlgAvanzado::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_EPuerto.SetWindowText(m_szPuerto);
	m_EBaseDeDatos.SetWindowText(m_szDB);
	CString texto;
	texto.Format("Se refiere al nombre de la base de datos a crear por lo general ‘%s’.", CApplicacion::GetAppName());
	m_SNombreData.SetWindowText(texto);
	return TRUE;
}

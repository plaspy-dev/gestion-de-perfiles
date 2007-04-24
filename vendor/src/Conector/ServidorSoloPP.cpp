// D:\documentos\src\Conector\ServidorSoloPP.cpp : implementation file
//

#include "stdafx.h"
#include "Conector.h"
# include "TablasSheet.h"
#include "ServidorSoloPP.h"


// CServidorSoloPP dialog

IMPLEMENT_DYNAMIC(CServidorSoloPP, CPropertyPage)

CServidorSoloPP::CServidorSoloPP()
	: CPropertyPage(CServidorSoloPP::IDD)
{
	m_psp.dwFlags|= PSP_DEFAULT|PSP_USEHEADERTITLE|PSP_USEHEADERSUBTITLE;
	m_psp.pszHeaderTitle= "Servidor MySQL";
	m_psp.pszHeaderSubTitle="Este servidor ya tiene instalado y configurado MySQL.";
}

CServidorSoloPP::~CServidorSoloPP()
{
}

void CServidorSoloPP::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ESERVIDOR, m_EServidor);
}


BEGIN_MESSAGE_MAP(CServidorSoloPP, CPropertyPage)
	ON_EN_CHANGE(IDC_ESERVIDOR, &CServidorSoloPP::OnEnChangeEservidor)
END_MESSAGE_MAP()

void CServidorSoloPP::OnEnChangeEservidor()
{
	CPropertySheet* parent = (CPropertySheet*)GetParent();
	parent->SetWizardButtons(PSWIZB_BACK | ( m_EServidor.Text.IsEmpty() ? 0 : PSWIZB_NEXT));
}

BOOL CServidorSoloPP::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	CPropertySheet* parent = (CPropertySheet*)GetParent();
	parent->SetWizardButtons(PSWIZB_BACK);

	return TRUE;
}

BOOL CServidorSoloPP::OnSetActive()
{
	OnEnChangeEservidor();
	return CPropertyPage::OnSetActive();
}

LRESULT CServidorSoloPP::OnWizardNext()
{
	CTablasSheet *parent= (CTablasSheet *)GetParent();
	if(parent)	
		parent->m_szServidor= m_EServidor.Text;
	return CPropertyPage::OnWizardNext();
}

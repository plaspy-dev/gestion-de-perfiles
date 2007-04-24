// BienvenidoPP.cpp : implementation file
//

#include "stdafx.h"
#include "Conector.h"
#include "BienvenidoPP.h"
# include <resource.h>
# include "TablasSheet.h"

// CBienvenidoPP dialog

IMPLEMENT_DYNAMIC(CBienvenidoPP, CPropertyPage)

CBienvenidoPP::CBienvenidoPP()
	: CPropertyPage(CBienvenidoPP::IDD)
	, m_nSeleccion(0)
{
	m_psp.dwFlags|= PSP_DEFAULT|PSP_USEHEADERTITLE|PSP_USEHEADERSUBTITLE;
	m_psp.pszHeaderTitle= "Bienvenido";
	m_psp.pszHeaderSubTitle="Aqui podra seleccionar su servidor, si usted tiene alguno.";
}

CBienvenidoPP::~CBienvenidoPP()
{
}

void CBienvenidoPP::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SBIENVENIDO, m_SBienvenido);
	DDX_Radio(pDX, IDC_RNO, m_nSeleccion);
}


BEGIN_MESSAGE_MAP(CBienvenidoPP, CPropertyPage)
END_MESSAGE_MAP()


// CBienvenidoPP message handlers

BOOL CBienvenidoPP::OnSetActive()
{
	CPropertySheet* parent = (CPropertySheet*)GetParent();
	parent->SetWizardButtons(PSWIZB_NEXT);
	return CPropertyPage::OnSetActive();
}

BOOL CBienvenidoPP::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	CString texto;
	texto.Format("Bienvenido al asistente de configuración de %s. Si ya tiene datos, este proceso NO sobrescribirá las tablas existentes. Si va a usar algun servidor asegurese que tenga MySQL instalado.",CApplicacion::GetAppName());
	m_SBienvenido.SetWindowText(texto);
	return TRUE;
}

LRESULT CBienvenidoPP::OnWizardNext()
{
	CTablasSheet* parent = (CTablasSheet *)GetParent();
	ASSERT(parent);
	UpdateData();
	parent->SetTipo(m_nSeleccion);
	return CPropertyPage::OnWizardNext();
}

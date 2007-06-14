#include "StdAfx.h"
# include "Template.h"
#include "ActualizadoPor.h"

IMPLEMENT_DYNAMIC(CActualizadoPor, CDialogXP)

CActualizadoPor::CActualizadoPor(CWnd* pParent /*=NULL*/)
: CDialogXP(CActualizadoPor::IDD, pParent)
{
}

CActualizadoPor::~CActualizadoPor(void)
{
}

void CActualizadoPor::DoDataExchange(CDataExchange* pDX)
{
	CDialogXP::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EACTUALIZADO, m_EActualizado);
	DDX_Control(pDX, IDOK, m_BAceptar);
}


BEGIN_MESSAGE_MAP(CActualizadoPor, CDialogXP)
	ON_BN_CLICKED(IDOK, OnBnClickedAceptar)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancelar)
END_MESSAGE_MAP()


// CImportarDlg message handlers

BOOL CActualizadoPor::OnInitDialog()
{
	CDialogXP::OnInitDialog();
	return TRUE;
}

void CActualizadoPor::OnBnClickedAceptar() {

	if(m_EActualizado.Text.IsEmpty()) {
		MessageBox(CLanguage::GetTraduccion(IDS_ERRORACTUALIZANDO));
	} else {
		m_szUsuario= m_EActualizado.Text;
		CDialogXP::OnOK();
	}
}

void CActualizadoPor::OnBnClickedCancelar() {
}

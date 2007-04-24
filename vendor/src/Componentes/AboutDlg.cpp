# include "stdafx.h"
#include "AboutDlg.h"
# include "Componentes.h"

CAboutDlg::CAboutDlg(const CString &szVersion,HICON icono, const CString &producto/*=""*/) : CDialogXP(CAboutDlg::IDD)
{
	m_szVersion= szVersion;
	m_szProducto= producto.IsEmpty() ? CApplicacion::GetAppName() : producto;
	m_hIcon= icono;
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogXP::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SVERSION, m_SVersion);
	DDX_Control(pDX, IDC_SLINKEX, m_SLink);
	DDX_Control(pDX, IDC_SPROGRAMA, m_SPrograma);
	DDX_Control(pDX, IDC_SICONO, m_SIcono);	
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogXP)
END_MESSAGE_MAP()

// Controladores de mensaje de CTemplateApp
BOOL CAboutDlg::OnInitDialog() {

	CDialogXP::OnInitDialog();
	CString texto;
	texto.Format("SDG LTDA. %s", CApplicacion::GetLongName());
	m_SPrograma.SetWindowText(texto);
	texto.Format("%s, Versión %s", m_szProducto, m_szVersion);
	m_SVersion.SetWindowText(texto);
	m_SLink.SetURL(CApplicacion::GetURL().IsEmpty() ? "http://www.sdgltda.com" : CApplicacion::GetURL());
	texto.Format("Acerca de %s", CApplicacion::GetLongName());
	SetWindowText(texto);

	m_banner.Attach(this, KCSB_ATTACH_TOP);
	m_banner.SetFillFlag(KCSB_FILL_TEXTURE);
	m_banner.SetSize(80);

	CBitmap bmp;
	bmp.LoadBitmap(IDB_BLOGO);
	m_banner.SetTexture(bmp);
	bmp.Detach();

	m_SIcono.SetIcon(m_hIcon);

	return TRUE;
}

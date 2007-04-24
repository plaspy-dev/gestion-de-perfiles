// DlgWebContrasena.cpp : implementation file
//

#include "stdafx.h"
#include "Conector.h"
#include "DlgWebContrasena.h"


// CDlgWebContrasena dialog

IMPLEMENT_DYNAMIC(CDlgWebContrasena, CDialog)

CDlgWebContrasena::CDlgWebContrasena(const CString &servidor,const CString &usuario, const CString &id, CWnd* pParent /*=NULL*/)
	: CDialog(CDlgWebContrasena::IDD, pParent)
{
	m_szUsuario= usuario;
	m_szServidor= servidor;
	m_szID= id;
}

CDlgWebContrasena::~CDlgWebContrasena()
{
}

void CDlgWebContrasena::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgWebContrasena, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgWebContrasena::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgWebContrasena::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDlgWebContrasena message handlers

void CDlgWebContrasena::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	// OnOK();
}

void CDlgWebContrasena::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

BOOL CDlgWebContrasena::OnInitDialog()
{
	AfxEnableControlContainer();

	CDialog::OnInitDialog();
	CString temp=CApplicacion::GetAppName();
	temp.MakeLower();

	VERIFY(m_EWebBrowser.CreateFromStatic(IDC_EWEBBROWSER, this));
	m_EWebBrowser.Navigate("http://" + m_szServidor + "/" + temp + "/usuario.php?usuario=" + m_szUsuario + "&id=" + m_szID);
	return TRUE;
}

BOOL CDlgWebContrasena::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	if(wParam==m_EWebBrowser.GetDlgCtrlID()) {
		Notification *not= (Notification *)lParam;
		if(not->hdr.code==CHTMLCtrl::DocumentComplete) {
			CString temp= GetRightString(not->URL, "?close=");
			if(temp=="true") {
				OnOK();
			}
		}
	}
	return CDialog::OnNotify(wParam, lParam, pResult);
}

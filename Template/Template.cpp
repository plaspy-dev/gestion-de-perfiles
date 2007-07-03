// Template.cpp : define los comportamientos de las clases para la aplicación.
//

#include "stdafx.h"
#include "Template.h"
#include "MainFrm.h"

#include "TemplateView.h"
#include "afxwin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTemplateApp

BEGIN_MESSAGE_MAP(CTemplateApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
END_MESSAGE_MAP()


// Construcción de CTemplateApp

CTemplateApp::CTemplateApp()
{
}

CTemplateApp theApp;


BOOL CTemplateApp::InitInstance()
{

	CString myid;
	myid.Format("{6A9FA33B-7458-41%d-B96A-B9D28D4B4DF9}",CApplicacion::GetID());
	if(!CWinAppEx::InitInstance(myid))
		return false;

	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	// Inicializar bibliotecas OLE
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();

	SetRegistryKey(_T("SDG"));
	

#if defined(ENABLE_LICENSE_CHECK) && !defined(_DEBUG)
	/*CTrialInfo trial;
	CString temp= m_lpCmdLine;
	if( temp.Find("-Register")!=-1) 
		return false;

	CMyRegKey key;
	key.MyOpen(HKEY_LOCAL_MACHINE, CApplicacion::GetAppKey() + "\\control");
	CDefender info(key.GetString("licencia","", ""));
	if( info.IsValidSerial()!=&info) {
		if(GetLicenceDlg()!=IDOK)
			return false;
		CDefender info1(key.GetString("licencia","", ""));
		if( info1.IsValidSerial()!=&info1) {
			CTrialInfo trial1;
			if( trial1.GetLeftDays()<0)
				return false;
		}
	}*/
#endif

	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;

	m_pMainWnd = pMainFrame;

	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	// quitar de aqui
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	return TRUE;
}

void CTemplateApp::OnFileNew() {

	if(((CMainFrame* )m_pMainWnd)->m_pUsuario) {
		((CMainFrame* )m_pMainWnd)->m_bInactivo= true;
	} else if( ((CMainFrame* )m_pMainWnd)->Conectar()) { // load papel defaults
		PRINTDLG pd;
		pd.lStructSize=(DWORD)sizeof(PRINTDLG);
		BOOL bRet=GetPrinterDeviceDefaults(&pd);
		if(bRet)
		{
			// protect memory handle with ::GlobalLock and ::GlobalUnlock
			DEVMODE FAR *pDevMode=(DEVMODE FAR *)::GlobalLock(m_hDevMode);
			// set orientation to landscape
			pDevMode->dmOrientation= GetProfileInt("printer", "orientacion", DMORIENT_PORTRAIT);
			pDevMode->dmPaperSize= GetProfileInt("printer", "papel", DMPAPER_LETTER);
			::GlobalUnlock(m_hDevMode);
		}
	}
}

void CTemplateApp::OnFileOpen() {

}

CDocument* CTemplateApp::OpenDocumentFile(LPCTSTR lpszFileName)
{
	return NULL;
}

// Cuadro de diálogo CAboutDlg utilizado para el comando Acerca de

class CAboutDlg : public CDialogXP
{
public:
	CAboutDlg();

// Datos del cuadro de diálogo
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Compatibilidad con DDX/DDV
public:
	CHyperLink m_SLink;
	CStatic		m_SVersion;
	virtual BOOL OnInitDialog();
// Implementación
protected:
	DECLARE_MESSAGE_MAP()
public:
	CKCSideBannerWnd m_banner;
};

CAboutDlg::CAboutDlg() : CDialogXP(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogXP::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SVERSION, m_SVersion);
	DDX_Control(pDX, IDC_SLINKEX, m_SLink);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogXP)
END_MESSAGE_MAP()

// Comando de la aplicación para ejecutar el cuadro de diálogo
void CTemplateApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// Controladores de mensaje de CTemplateApp
BOOL CAboutDlg::OnInitDialog() {

	CDialogXP::OnInitDialog();
	CString texto;
	texto.Format(CLanguage::GetTraduccion(IDS_SVERSION), OrganizationExample23_VERSION);
	m_SVersion.SetWindowText(texto);
	m_SLink.SetURL("http://mysql-demo.example.test");

	m_banner.Attach(this, KCSB_ATTACH_TOP);
	m_banner.SetFillFlag(KCSB_FILL_TEXTURE);
	m_banner.SetSize(80);
	
	CEnBitmap bitmap;
	bitmap.CopyImage(CApplicacion::GetBitmap());
	CRect rect;
	m_banner.GetWindowRect(rect);
	bitmap.ResizeImage(rect.Width(),rect.Height());
	m_banner.SetTexture((HBITMAP)bitmap.Detach());

	return TRUE;
}


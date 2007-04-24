// Template.cpp : define los comportamientos de las clases para la aplicación.
//

#include "stdafx.h"
#include "Template.h"
#include "MainFrm.h"

#include "TemplateView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTemplateApp

BEGIN_MESSAGE_MAP(CTemplateApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_FILE_PRINT_SETUP, OnFilePrintSetup)
END_MESSAGE_MAP()


// Construcción de CTemplateApp

CTemplateApp::CTemplateApp()
{
	m_mlparamold= NULL;
}


void CTemplateApp::OnFilePrintSetup() {

	PRINTDLG pd;
	pd.lStructSize=(DWORD)sizeof(PRINTDLG);
	if(GetPrinterDeviceDefaults(&pd))
	{
		CPageSetupDialog psd;

		// Initialize margins
		psd.m_psd.rtMargin.top = GetProfileInt("printer", "y", 20)*100;
		psd.m_psd.rtMargin.left = GetProfileInt("printer", "x", 20)*100;
		psd.m_psd.rtMargin.right = GetProfileInt("printer", "cx", 20)*100;
		psd.m_psd.rtMargin.bottom = GetProfileInt("printer", "cy", 20)*100;
		psd.m_psd.hDevMode= pd.hDevMode;

		if( psd.DoModal()==IDOK) {

			// margenes
			WriteProfileInt("printer", "x",psd.m_psd.rtMargin.left/100);
			WriteProfileInt("printer", "cx",psd.m_psd.rtMargin.right/100);
			WriteProfileInt("printer", "y",psd.m_psd.rtMargin.top/100);
			WriteProfileInt("printer", "cy",psd.m_psd.rtMargin.bottom/100);
			SelectPrinter(psd.m_psd.hDevNames, psd.m_psd.hDevMode);

			// papel
			DEVMODE FAR *pDevMode=(DEVMODE FAR *)::GlobalLock(m_hDevMode);
			WriteProfileInt("printer", "orientacion", pDevMode->dmOrientation);
			WriteProfileInt("printer", "papel",pDevMode->dmPaperSize);
			::GlobalUnlock(m_hDevMode);
		}
	} else CWinApp::OnFilePrintSetup();
}

CTemplateApp theApp;

HWND CTemplateApp::RegisterApp(HWND mainframe) {

	CRegKey hReg;
	HWND main= NULL;
	DWORD dwHandle;
#ifndef _DEBUG
	if( ERROR_SUCCESS== hReg.Create(HKEY_CURRENT_USER, CApplicacion::GetAppKey() + "\\Control")) {

		if( hReg.QueryDWORDValue("MainWnd", dwHandle)==ERROR_SUCCESS) {

			main= (HWND) dwHandle;
			CString titulo;
			if( ::IsWindow(main)) {
				CWnd *confirma=CWnd::FromHandle(main);
				confirma->GetWindowText(titulo);
				if( titulo.Find("Template")!=-1) {
					SetForegroundWindow(main);
					if (IsIconic(main)) 
						::ShowWindow(main, SW_RESTORE);
					return main;
				}
			}
		}
		dwHandle= (DWORD) mainframe;
		hReg.SetDWORDValue("MainWnd", dwHandle);
		hReg.Close();
	}
#endif
	return NULL;
}


BOOL CTemplateApp::InitInstance()
{
	// Windows XP requiere InitCommonControlsEx() si un manifiesto de
	// aplicación especifica el uso de ComCtl32.dll versión 6 o posterior para habilitar
	// estilos visuales. De lo contrario, se generará un error al crear ventanas.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Establecer para incluir todas las clases de control comunes que desee utilizar
	// en la aplicación.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

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
	
	CString temp= m_lpCmdLine;
	SetRegistryKey(_T("SDG"));
	// Crear ventana de marco principal MDI
	CNewMenu::SetMenuDrawMode(GetProfileInt(_T("NewMenu"),_T("Style"),2));


	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;

	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	HWND main= RegisterApp(pMainFrame->GetSafeHwnd());
	if( main) {
		if( temp.Find("-close")!=-1)
			SendMessage( main, WM_CLOSE,0,0);
		else if(!temp.IsEmpty()) {
			ProcessShellCommand(cmdInfo);
		}
		return false;
	}
	if( temp.Find("-close")!=-1)
		return false;

#if defined(ENABLE_LICENSE_CHECK) && !defined(_DEBUG)
	CTrialInfo trial;
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
	}
#endif

	m_pMainWnd = pMainFrame;

	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	// quitar de aqui
	if( !ProcessShellCommand(cmdInfo))
		OnFileNew();

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

void CTemplateApp::OnFileSave()
{
}

BOOL CTemplateApp::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->message==WM_LBUTTONDOWN || pMsg->message==WM_RBUTTONDOWN || pMsg->message==WM_KEYDOWN || pMsg->message==WM_SYSKEYDOWN) {
		if( m_pMainWnd) 
			((CMainFrame *) m_pMainWnd)->m_nInactivo=0;
	} else if( pMsg->message==WM_MOUSEMOVE && pMsg->lParam!=m_mlparamold && m_pMainWnd) {
		m_mlparamold= pMsg->lParam;
		((CMainFrame *) m_pMainWnd)->m_nInactivo=0;
	}
	return CWinApp::PreTranslateMessage(pMsg);
}

// Cuadro de diálogo CAboutDlg utilizado para el comando Acerca de

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Datos del cuadro de diálogo
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Compatibilidad con DDX/DDV

// Implementación
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// Comando de la aplicación para ejecutar el cuadro de diálogo
void CTemplateApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// Controladores de mensaje de CTemplateApp


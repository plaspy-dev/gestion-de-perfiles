// MainFrm.cpp : implementación de la clase CMainFrame
//

#include "stdafx.h"
#include "Template.h"
# include "TemplateView.h"
# include "ResidenciaView.h"
# include "OficinaView.h"
#include "MainFrm.h"
# include "EstudiosView.h"
# include "SeguimientoView.h"
# include "BusquedaView.h"
# include "GestionView.h"
# include "UsuarioDlg.h"
# include "areasview.h"
# include "ImportarDlg.h"
# include "IndexarDlg.h"
# include "TipoView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

# define	ANIMATION_TIMER			300
# define	TIMER_EVENT				32000


// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWndXP)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWndXP)
	ON_WM_CREATE()
	ON_UPDATE_COMMAND_UI(ID_VER_NAVEGADOR, OnUpdateVerNavegador)
	ON_COMMAND(ID_CONFIGURACION_SERVIDORES, OnConfiguracionServidores)
	ON_WM_TIMER()
	ON_UPDATE_COMMAND_UI(ID_FILE_NEW, OnUpdateFileNew)
	ON_MESSAGE(WM_QUERYCONECTOR,OnQueryConector)
	ON_MESSAGE(WM_QUERYUSUARIO,OnQueryUsuario) 
	ON_MESSAGE(WM_MYSQL,OnMySQL)
	ON_COMMAND_RANGE(32771, 32773, OnSelectLanguage)
	ON_UPDATE_COMMAND_UI_RANGE(32771, 32773, OnUpdateLanguage)
	ON_UPDATE_COMMAND_UI(ID_ARCHIVO_ADMINISTRADOR, OnUpdateAdministrador)
	ON_COMMAND(ID_ARCHIVO_ADMINISTRADOR, OnAdministrador)
	ON_COMMAND(ID_FILE_OPEN, OnImportar)
	ON_UPDATE_COMMAND_UI(ID_FILE_OPEN, OnUpdateImportar)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,
	ID_INDICATOR_EXT,
	ID_INDICATOR_EXT,
	ID_INDICATOR_EXT,
	ID_INDICATOR_EXT,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
};


// Construcción y destrucción de CMainFrame

CMainFrame::CMainFrame()
{
	m_pUsuario= NULL;
	m_nAnimation=0;
	m_bLoad=false;
}

CMainFrame::~CMainFrame()
{
	if(m_pUsuario)
		delete m_pUsuario;
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWndXP::OnCreate(lpCreateStruct) == -1)
		return -1;
	// barra de herramientas principal
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("No se pudo crear la barra de herramientas\n");
		return -1;      // No se pudo crear
	}
	// barra de estado
	if (!m_wndStatusBar.CreateStatusBar(this, indicators, sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("No se pudo crear la barra de estado\n");
		return -1;      // Fehler beim Erzeugen
	}
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	CREATE_JDOCK(this);

	// establecemos los clicks del arbol en uno
	AfxGetApp()->WriteProfileInt("treenav","simple", 1);

	// barras de herramientas
	if (!m_pNavBar.Create(_T("Navegador"),WS_CHILD, this, 200))
	{
		TRACE0("Failed to create my bar2\n");
		return -1;
	}

	m_pNavBar.SetBarStyle(m_pNavBar.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	m_pNavBar.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_pNavBar, AFX_IDW_DOCKBAR_RIGHT);


	// los tabs
	DWORD style = MT_TOP | MT_IMAGES;
	style|= AfxGetApp()->GetProfileInt("control", "showtab", 1)==2 ? MT_HIDEWLT2VIEWS : 0;
	m_mdiTabs.Create(this, style);

	// el conector
	m_pConector.SetMainFrame(this);
	m_pConector.Cargar();

	// barra de estado
	m_wndStatusBar.SetMode(1, XSB_PROGRESS | XSB_SMOOTH);
	m_wndStatusBar.SetRange(1, 0, 100);
	m_wndStatusBar.SetWidth(1, 100);
	m_wndStatusBar.GetProgressCtrl(1)->SetShowText(false);
	m_wndStatusBar.SetMode(2, XSB_TEXT | DT_CENTER);
	m_wndStatusBar.SetText(2, " ");
	m_wndStatusBar.SetWidth( 2, 100);

	if( AfxGetApp()->GetProfileInt("control", "animation", 1)) {
		m_wndStatusBar.SetMode(4, XSB_BITMAP | DT_CENTER);
		CString temp;
		temp.Format("%d", IDB_BLOADING1);
		m_nAnimation=IDB_BLOADING1;
		m_wndStatusBar.SetBitmap(4, temp, temp);
		SetTimer(ANIMATION_TIMER, AfxGetApp()->GetProfileInt("control", "animationtimer", 200), NULL);
	} else {
		m_wndStatusBar.SetMode(4, XSB_TEXT | DT_CENTER);
		m_wndStatusBar.SetNumber(4, 0, 0);
	}

	m_wndStatusBar.SetMode(3, XSB_TEXT | DT_CENTER);
	m_wndStatusBar.SetText(3, " ", " ");
	m_wndStatusBar.SetWidth(3, 100);

	m_DefaultNewMenu.LoadToolBar(IDR_MAINFRAME, NULL);
	// Preserve the installation-path update from the archived Componentes DLL.
	TCHAR executablePath[MAX_PATH];
	const DWORD pathLength = GetModuleFileName(NULL, executablePath, MAX_PATH);
	if (pathLength && pathLength < MAX_PATH) {
		CString installPath(executablePath);
		installPath.MakeLower();
		CMyRegKey registration(HKEY_LOCAL_MACHINE, CApplicacion::GetAppKey());
		registration.WriteString("", "InstallPath", installPath.Left(installPath.ReverseFind('\\') + 1));
	}
	/*if(!Registrar())
	return -1;*/
	// este es el de la licencia
	// SetTimer(4, 30*60*1000, NULL);

	m_LogoMdiClientWnd.SetFlash(CApplicacion::GetAppPath( AfxGetApp()->GetProfileString("swfop", "archivo", "logo.swf")), 
		AfxGetApp()->GetProfileInt("swfop", "ancho", 400),
		AfxGetApp()->GetProfileInt("swfop", "alto", 150),
		AfxGetApp()->GetProfileInt("swfop", "bkcolor", -1)
		);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	return TRUE;
}


// Diagnósticos de CMainFrame

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG


// Controladores de mensajes de CMainFrame


void CMainFrame::ResetToolbars() {

	DockControlBar(&m_wndToolBar,AFX_IDW_DOCKBAR_TOP);

	m_pNavBar.OnCerrar();

	DockControlBar(&m_pNavBar, AFX_IDW_DOCKBAR_RIGHT);

	m_pNavBar.LoadStateBar("Barras\\Barras");

	m_pNavBar.ShowWindow(SW_SHOW);

	RecalcLayout();
	AfxGetApp()->WriteProfileInt("control", "notstore", 1);
}

void CMainFrame::LoadState() {

	CWaitCursor wait;

#ifndef _DEBUG
	CString temp="CBusquedaView,CTemplateView,CResidenciaView,COficinaView,CEstudiosView,CSeguimientoView,CGestionView,CAreasView,CTipoView";
	AfxGetApp()->WriteProfileString("control", "classname", temp);
#endif
	
	LoadBarState("barras\\Barras");
	m_pNavBar.LoadStateBar("Barras\\Barras");

	if( !AfxGetApp()->GetProfileInt("barras\\Barras", "store",0)) {
		m_pNavBar.ShowWindow(SW_SHOW);
	}
	CMDIFrameWndXP::LoadState();
}

void CMainFrame::SaveState() {

	if( !m_pUsuario || !m_pUsuario->GetID())

		return;

	CMDIFrameWndXP::SaveState();

	// m_PropBar.SaveBar("Barras\\Barras");
}

bool CMainFrame::Conectar() {

	if(!m_bLoad) {
		CLanguage::InitLanguage();
		m_pNavBar.SetWindowText(CLanguage::GetTraduccion(IDS_NAVEGADOR));
	}
	m_bLoad=true;

	FUNCTION_STACK_TRACE("AAAAAAAA");
	bool existe=false;
	if( m_pUsuario) 
		if( m_pUsuario->GetID())
			existe=true;	
	if( !m_pConector.GetDatabasesCount()) {

		CBitmap asistente;
		asistente.LoadBitmap(IDB_BASISTENTE);
		InitAsistente(&asistente, &asistente);
		m_pConector.SetMainFrame( this);
		m_pConector.Cargar();
		if(!m_pConector.GetDatabasesCount()) {
			OnConfiguracionServidores();
			if( !m_pConector.GetDatabasesCount())
				return false;
		}
	}

	KillTimer(50000);
	m_bConectando= true;
	CStringArray	datos;

	if( GetDlgIngreso(existe, datos)==IDOK) {

		if( !existe)
			m_pNavBar.m_Tree.DeleteAllItems();

		m_pConector.SetPasswd(datos[0]);
		m_pConector.SetUser(datos[1]);

		if( !m_pUsuario)
			m_pUsuario= new CUsuario( datos[2], atoi(datos[3]), m_pConector.EsIncrustado() ? 2 : atoi(datos[4]));
		else {
			m_pUsuario->SetUser(datos[2]);
			m_pUsuario->SetId(atoi(datos[3]));
			m_pUsuario->SetGrupo(m_pConector.EsIncrustado() ? 2 : atoi(datos[4]));
		}
		// m_pConector.SetRealUser(dlg.m_szNombreReal);
		m_wndStatusBar.SetText(2, m_pConector.EsIncrustado() ? "Usuario" : datos[2]);

		// registrarse en el chat
		SendMessage(WM_REGISTRARUSUARIO, 0, -2);
		if( !existe) {
			AfxGetApp()->WriteProfileInt("control", "loadstate", 1);
			LoadDocumentos();
			LoadState();
			m_pConector.Query("SELECT SYSDATE()", TIMER_EVENT, this);
		}
		SendMessageBroadCast(WM_QUERYUSUARIO, 0, (LPARAM)m_pUsuario);
		SendMessageBroadCast(WM_QUERYCONECTOR, 0, (LPARAM)&m_pConector);
		if( AfxGetApp()->GetProfileInt("control", "timeout", 300))
			SetTimer(50000, 1000, NULL);
		SAFE_DELETE(m_pFadeWnd);
		SendMessageBroadCast(WM_INACTIVIDAD, 0,1);
		AfxGetApp()->WriteProfileInt("control", "loadstate", 0);
		m_bConectando= false;
		return true;
	}
	return false;
}

bool CMainFrame::LoadDocumentos() {

	FUNCTION_STACK_TRACE("AAAAAAAB");

	HTREEITEM hRoot;

	hRoot= m_pNavBar.m_Tree.InsertItem(CLanguage::GetTraduccion(IDS_DATOS));
	m_pNavBar.m_Tree.SetItemState(hRoot, TVIS_BOLD,TVIS_BOLD);

	CNewMenu *mimenu= new CNewMenu();

	mimenu->CreateMenu();

	LoadDocument(mimenu, hRoot, CLanguage::GetTraduccion(IDS_BUSQUEDA), IDR_TemplateTYPE ,RUNTIME_CLASS(CBusquedaView));
	LoadDocument(mimenu, hRoot, CLanguage::GetTraduccion(IDS_PERSONALES), IDR_TemplateTYPE ,RUNTIME_CLASS(CTemplateView));	
	LoadDocument(mimenu, hRoot, CLanguage::GetTraduccion(IDS_RESIDENCIA), IDR_TemplateTYPE ,RUNTIME_CLASS(CResidenciaView));
	LoadDocument(mimenu, hRoot, CLanguage::GetTraduccion(IDS_OFICINA), IDR_TemplateTYPE ,RUNTIME_CLASS(COficinaView));
	LoadDocument(mimenu, hRoot, CLanguage::GetTraduccion(IDS_EDUCACION), IDR_TemplateTYPE ,RUNTIME_CLASS(CEstudiosView));
	LoadDocument(mimenu, hRoot, CLanguage::GetTraduccion(IDS_SEGUIMIENTO), IDR_TemplateTYPE ,RUNTIME_CLASS(CSeguimientoView));
	LoadDocument(mimenu, hRoot, CLanguage::GetTraduccion(IDS_GESTIONBASE), IDR_TemplateTYPE ,RUNTIME_CLASS(CGestionView));
	if(m_pUsuario->Pertenece(GRUPO_ADMINISTRADOR)) {
		LoadDocument(mimenu, hRoot, CLanguage::GetTraduccion(IDS_AREAS), IDR_TemplateTYPE ,RUNTIME_CLASS(CAreasView));
		LoadDocument(mimenu, hRoot, CLanguage::GetTraduccion(IDS_TIPOS), IDR_TemplateTYPE ,RUNTIME_CLASS(CTipoView));
	}

	m_pMenus.Add(CLanguage::GetTraduccion(IDS_DATOS), (DWORD)mimenu);

	m_pNavBar.m_Tree.Expand(hRoot, TVE_EXPAND);

	// LoadDocumentosEx("administrador.dll", NULL, m_pUsuario->GetGrupo());

	return CMDIFrameWndXP::LoadDocumentos(m_pUsuario->GetGrupo());
}

// Diagnósticos de CMainFrame

LRESULT CMainFrame::OnQueryConector(WPARAM wParam, LPARAM lParam) {

	if(lParam) {
		CWnd *temp= (CWnd *) lParam;
		temp->SendMessage( WM_QUERYCONECTOR, 0, (LPARAM) &m_pConector);
	}
	return (LRESULT) &m_pConector;
}

LRESULT CMainFrame::OnQueryUsuario(WPARAM wParam, LPARAM lParam) {

	if(lParam) {
		CWnd *temp= (CWnd *) lParam;
		temp->SendMessage( WM_QUERYUSUARIO, 1, (LPARAM) m_pUsuario);
	}
	return (LRESULT) m_pUsuario;
} 

int CMainFrame::OnMySQLThreadEnd(MySQLMessage *hilo) {

	PostMessage(WM_STATUSBAR, 2);
	if( hilo->GetID()>49000 || TIMER_EVENT==hilo->GetID()) {
		return 0;
	} else if( hilo->GetID()>99) {
	} else DeleteHilos(hilo);
	return 0;
}

int CMainFrame::OnMySQLQueryResult(MySQLMessage *message) {

	switch( message->GetID()) {
		case TIMER_EVENT:
			{
				CMyTime tiempo(message->GetAt(0));
				m_pTServidor=tiempo;
				m_pTLocal= CTime::GetCurrentTime();
				m_psDiferencia= m_pTLocal-m_pTServidor;
				SetTimer(TIMER_EVENT, 1000, NULL);
				SendMessageBroadCast(WM_QUERYTIME, 0, (LPARAM)&m_pTServidor);
			}
		case 50001: // no se hace nada
			return 0;
	}
	if( message->GetID()>99) {
		PostMessage(WM_STATUSBAR, 1);
	} else Messages(message); // SOLO en el caso de NEW_XML
	return 0;
}

int CMainFrame::OnQueryEmpty(int owner) {

	switch( owner) {
		case TIMER_EVENT:
		case 50001: // no se hace nada
			return 0;
	}
	return 0;
}

int CMainFrame::OnQueryTitle(MySQLMessage *message) {

	switch( message->GetID()) {
		case TIMER_EVENT:
		case 50001: // no se hace nada
		case 50000: // chat
			return 0;
	}
	return 0;
}

LRESULT CMainFrame::OnMySQL(WPARAM wParam, LPARAM lParam) {

	CMySQLMessage *mensaje= (CMySQLMessage *)lParam;
	switch( wParam) 
	{
	case THREAD_END:
		if(mensaje->GetParent()==this)
			OnMySQLThreadEnd(mensaje);
		else 
			PostMessage( WM_STATUSBAR, 2);
		break;
	case QUERY_COUNT2:
		PostMessage( WM_STATUSBAR, 0, mensaje->GetCount());
		break;
	case QUERY_RESULT:
		if(mensaje->GetParent()==this)
			OnMySQLQueryResult(mensaje);
		else 
			PostMessage( WM_STATUSBAR, 1);
		break;
	case QUERY_TITLE:
		if(mensaje->GetParent()==this)
			OnQueryTitle(mensaje);
		break;
	case QUERY_EMPTY:
		if(mensaje->GetParent()==this)
			OnQueryEmpty(mensaje->GetID());
		break;
	case QUERY_ERROR:
		{
			CString formato;
			formato.Format(CLanguage::GetTraduccion(IDS_ERRORMYSQL), mensaje->GetAt(0),mensaje->GetAt(1));
			SetErrorEx(NULL,formato, 3, "mysql");
		}
		break;
	}
	return 0;
}

bool CMainFrame::DeleteHilos( MySQLMessage *hilo) {
	return true;
}

bool CMainFrame::Messages( MySQLMessage *contenido) {

	FUNCTION_STACK_TRACE("AAAAAAAE");
	PostMessage( WM_STATUSBAR, 1);
	return true;
}

void CMainFrame::OnUpdateVerNavegador(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_pUsuario!=NULL);
	CMDIFrameWndXP::OnUpdateVerNavegador(pCmdUI);
}

void CMainFrame::OnConfiguracionServidores()
{
	if( GetDlgDataBases()==IDOK) {

		if( m_pUsuario) {
			SetErrorEx(NULL, "Debe Reiniciar "+ CApplicacion::GetAppName() + " acceso para realizar los cambios", 0, "conector");
		} else {
			m_pConector.SetMainFrame( this);
			m_pConector.Cargar();
		}
	}
}

void CMainFrame::OnTimer(UINT nIDEvent)
{
	if( nIDEvent==TIMER_EVENT) {
		CTime tiempo= CTime::GetCurrentTime();
		CTimeSpan span(0, 0, 1, 0);
		if( tiempo<m_pTLocal || span < tiempo-m_pTLocal) {
			KillTimer(TIMER_EVENT);
			m_pConector.Query("SELECT SYSDATE()", TIMER_EVENT, this);
		} else {
			m_pTLocal= tiempo;
			m_pTServidor= m_pTLocal-m_psDiferencia;
		}
	} else if( nIDEvent==ANIMATION_TIMER) {

		if( !m_nCurrentHilos) {
			if( m_nAnimation!= IDB_BLOADING1) {
				m_nAnimation=IDB_BLOADING1;
				CString temp;
				temp.Format("%d", m_nAnimation);
				m_wndStatusBar.SetBitmap(4, temp, temp);
			}
			return;
		}
		if( m_nAnimation==IDB_BLOADING10) 
			m_nAnimation=IDB_BLOADING1;
		m_nAnimation++;
		CString temp;
		temp.Format("%d", m_nAnimation);
		m_wndStatusBar.SetBitmap(4, temp, temp);
#ifdef ENABLE_LICENSE_CHECK
	} else if(nIDEvent==4) { // licencia
		/*
#ifdef _DEBUG
		CMyRegKey key;
		key.MyOpen(HKEY_LOCAL_MACHINE, CApplicacion::GetAppKey() + "\\control");
		CDefender info(key.GetString("licencia","", ""));
		if( info.IsValidSerial()!=&info) {
			CTrialInfo info;
			if( info.GetLeftDays()<=0) {
				PostMessage(WM_CLOSE);
			}
		} else {
			KillTimer(4);
		}
#endif
		*/
#endif
	} else if( nIDEvent==50000) {
		// inactividad
		KillTimer(nIDEvent);
		return;
		m_nInactivo++;
		if( m_bInactivo || m_nInactivo>=AfxGetApp()->GetProfileInt("control", "timeout", 300)) {
			KillTimer(50000);
			SendMessage(WM_REGISTRARUSUARIO, 0, -3);
			SendMessageBroadCast(WM_INACTIVIDAD, 0,0);
			m_bInactivo=true;
			SAFE_DELETE(m_pFadeWnd);
			if( m_bFocus) {
				if( AfxGetApp()->GetProfileInt("control", "efecto", 1) && IsWindowsNT())
					m_pFadeWnd= new CFadeWnd(this);
				while( Conectar()==false) {
					if( AfxMessageBox( "Esta acción Cerrara "+ CApplicacion::GetAppName() + ".\n¿Desea Cerrarlo?", MB_ICONQUESTION | MB_YESNO)==IDYES) {
						PostMessage(WM_CLOSE);
						return;
					}
				}
				m_bInactivo=false;
				m_nInactivo=0;
			}
		}
	}
	CMDIFrameWndXP::OnTimer(nIDEvent);
}

void CMainFrame::OnUpdateFileNew(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!m_pUsuario || AfxGetApp()->GetProfileInt("control", "timeout", 1));
}

void CMainFrame::OnSelectLanguage(UINT id) 
{
	DWORD idioma= LANG_SPANISH;
	switch(id) {
		case 32772:
			idioma=LANG_GERMAN;
			break;
		case 32773:
			idioma= LANG_ENGLISH;
			break;
	}
	CLanguage::SetLanguage(idioma);
	if(CLanguage::GetLanguage()!=idioma) {

		CString formato;
		formato.Format(CLanguage::GetTraduccion(IDS_RESTART), CApplicacion::GetAppName());
		MessageBox(formato);
		PostMessage(WM_CLOSE);
	}
}

void CMainFrame::OnUpdateLanguage(CCmdUI *pCmdUI) {

	DWORD idioma= CLanguage::GetLanguage();
	switch(pCmdUI->m_nID) {
		case 32772:
			pCmdUI->SetCheck(idioma==LANG_GERMAN);
			break;
		case 32773:
			pCmdUI->SetCheck(idioma==LANG_ENGLISH);
			break;
		default:
			pCmdUI->SetCheck(idioma==LANG_SPANISH || idioma==0);
			break;
	}
}

void CMainFrame::OnUpdateAdministrador(CCmdUI *pCmdUI) {
	
	pCmdUI->Enable(m_pUsuario && m_pUsuario->GetGrupo()==2 && m_pConector.EsIncrustado());
}

void CMainFrame::OnAdministrador() {

	CUsuarioDlg dlg;
	if(dlg.DoModal()==IDOK)
	{
		m_pUsuario->SetGrupo(1);
		m_wndStatusBar.SetText(2, "Administrador");
		SendMessageBroadCast(WM_QUERYUSUARIO, 0, (LPARAM) m_pUsuario);
		LoadDocument((CNewMenu *)m_pMenus.GetData(), m_pNavBar.m_Tree.GetRootItem(), CLanguage::GetTraduccion(IDS_AREAS), IDR_TemplateTYPE ,RUNTIME_CLASS(CAreasView));
		LoadDocument((CNewMenu *)m_pMenus.GetData(), m_pNavBar.m_Tree.GetRootItem(), CLanguage::GetTraduccion(IDS_TIPOS), IDR_TemplateTYPE ,RUNTIME_CLASS(CTipoView));
	}
}

void CMainFrame::OnImportar() {

	CFileDialog filedlg(true, "csv", "", 6,CLanguage::GetTraduccion(IDS_TIPODEIMPORTACION), this);
	if(filedlg.DoModal()==IDOK) {
		CImportarDlg dlg;
		dlg.m_szArchivo= filedlg.GetPathName();
		if(dlg.DoModal()==IDOK) {
			CIndexarDlg idlg;
			idlg.DoModal();

			CString formato;
			formato.Format(IDS_RESTART, CApplicacion::GetAppName());
			MessageBox(formato);
			PostMessage(WM_CLOSE);
		}
	}
}

void CMainFrame::OnUpdateImportar(CCmdUI *pCmdUI) {

	pCmdUI->Enable(m_pUsuario && m_pUsuario->GetGrupo()==1);
}

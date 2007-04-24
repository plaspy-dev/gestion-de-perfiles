// Conector.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "Conector.h"
# include "TablasSheet.h"
# include "DlgIngreso.h"
# include "DlgIngreso2.h"
# include "DlgIngreso3.h"
# include "DlgBases.h"
# include <ResourceDLL.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static AFX_EXTENSION_MODULE ConectorDLL = { NULL, NULL };

// CConectorApp

BEGIN_MESSAGE_MAP(CConectorApp, CWinApp)
END_MESSAGE_MAP()


// CConectorApp construction

CConectorApp::CConectorApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CConectorApp object

CConectorApp theApp;


// CConectorApp initialization

BOOL CConectorApp::InitInstance()
{
	CWinApp::InitInstance();
	if (!AfxSocketInit())
	{
		// AfxMessageBox(104);
		ASSERT(false);
		return FALSE;
	}

	if (!AfxInitExtensionModule(ConectorDLL, m_hInstance)) {
		ASSERT(false);
		return false;
	}
	new CDynLinkLibrary(ConectorDLL);
#ifndef _DEBUG
	if(!RegisterFile(CApplicacion::GetAppPath("conector.dll"))) {

		CString texto;
		texto.Format("%s Esta corrupto.\nPor favor ReInstale %s o Informe al Administrador del sistema.", CApplicacion::GetAppName(), CApplicacion::GetAppName());
		AfxMessageBox(texto, MB_ICONERROR);
		ASSERT(false);
		return false;
	}
#endif
	return TRUE;
}

void InitAsistente(CBitmap *bmpHeader,CBitmap *bmpWater) {

	CWnd *main= AfxGetMainWnd();
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CBitmap bmp;
	bmp.LoadBitmap(IDB_BASISTENTE);
	if(bmpWater==NULL) 
		bmpWater=&bmp;
	if(bmpHeader==NULL)
		bmpHeader=&bmp;
	CTablasSheet dlg("Asistente", main, 0, *bmpWater, NULL, *bmpHeader);
	if(dlg.DoModal()==ID_WIZFINISH) {
		
	}
}

DWORD GetDlgIngreso(bool existe, CUsuario3 *usuario, bool local/*=false*/) {

	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CConector	m_pConector;
	m_pConector.Cargar(local ? HKEY_LOCAL_MACHINE : HKEY_CURRENT_USER );
	ASSERT(m_pConector.GetDatabasesCount()>0);
	if(m_pConector.GetDatabasesCount()==0) 
		return IDCANCEL;

	if(m_pConector.EsIncrustado()) {

		usuario->SetNombreCompleto("Administrador");
		usuario->SetUser("Administrador");
		// no se le da contraseña
		usuario->SetGrupo(0);
		usuario->SetId(1); // administrador
		usuario->SetDescripcion("Administrador"); // la descripcion.
		return IDOK;
	}

	CDlgIngreso3 dlg;
	dlg.m_bExiste=existe;
	dlg.m_bLocalMachine= local;

	if(dlg.DoModal()==IDOK) {

		CString temp;

		usuario->SetContrasena(dlg.m_szPasswd);
		usuario->SetNombreCompleto(dlg.m_szNombreUsuario);
		usuario->SetUser(dlg.m_szNombreReal);
		usuario->SetId(dlg.m_dwID);
		usuario->SetGrupo(dlg.m_dwGrupo);
		usuario->SetDescripcion(dlg.m_szExtra);
		usuario->SetPermisos(dlg.m_pPermisos);
		return IDOK;
	}
	return IDCANCEL;
}

DWORD GetDlgIngreso(bool existe, CUsuario2 *usuario, bool local/*=false*/) {

	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CConector	m_pConector;
	m_pConector.Cargar(local ? HKEY_LOCAL_MACHINE : HKEY_CURRENT_USER );
	ASSERT(m_pConector.GetDatabasesCount()>0);
	if(m_pConector.GetDatabasesCount()==0) 
		return IDCANCEL;

	if(m_pConector.EsIncrustado()) {

		usuario->SetNombreCompleto("Administrador");
		usuario->SetUser("Administrador");
		// no se le da contraseña
		usuario->SetId(1); // administrador
		usuario->SetDescripcion("1"); // la descripcion.
		return IDOK;
	}


	CDlgIngreso2 dlg;
	dlg.m_bExiste=existe;
	dlg.m_bLocalMachine= local;

	if(dlg.DoModal()==IDOK) {

		CString temp;

		usuario->SetContrasena(dlg.m_szPasswd);
		usuario->SetNombreCompleto(dlg.m_szNombreUsuario);
		usuario->SetUser(dlg.m_szNombreReal);
		usuario->SetId(dlg.m_dwID);
		usuario->SetGrupo(dlg.m_dwGrupo);
		usuario->SetData(dlg.m_dwCac);
		usuario->SetDescripcion(dlg.m_szExtra);
		usuario->SetAutonumerico(dlg.m_nAutoNumerico);
		return IDOK;
	}
	return IDCANCEL;
}

// necesario para iniciar el ingreso
DWORD GetDlgIngreso(bool existe, CStringArray &datos, bool local/*=false*/) {

	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	datos.RemoveAll();

	CConector	m_pConector;
	m_pConector.Cargar(local ? HKEY_LOCAL_MACHINE : HKEY_CURRENT_USER );
	ASSERT(m_pConector.GetDatabasesCount()>0);
	if(m_pConector.GetDatabasesCount()==0) 
		return IDCANCEL;

	if(m_pConector.EsIncrustado()) {

		datos.Add(""); // 0 contraseña
		datos.Add("Administrador"); // 1 nombre del usuario codificado o no.
		datos.Add("Administrador"); // 2 nombre real del usuario
		datos.Add("1"); // 3 id del usuario
		datos.Add("1"); // 4 grupo del usuario
		datos.Add(""); // 5 datos del cac
		datos.Add(""); // extra
		return IDOK;
	}


	CDlgIngreso dlg;
	dlg.m_bExiste=existe;
	dlg.m_bLocalMachine= local;

	if(dlg.DoModal()==IDOK) {

		CString temp;

		datos.Add(dlg.m_szPasswd); // 0 contraseña
		datos.Add(dlg.m_szNombreUsuario); // 1 nombre del usuario codificado o no.
		datos.Add(dlg.m_szNombreReal); // 2 nombre real del usuario
		temp.Format("%d",dlg.m_dwID); 
		datos.Add(temp); // 3 id del usuario
		temp.Format("%d", dlg.m_dwGrupo); 
		datos.Add(temp); // 4 grupo del usuario
		temp.Format("%d", dlg.m_dwCac);
		datos.Add(temp); // 5 datos del cac
		datos.Add(dlg.m_szExtra);
		return IDOK;
	}
	return IDCANCEL;
}

// bases de datos
DWORD GetDlgDataBases() {

	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CDlgBases dlg;
	return dlg.DoModal();
}

CResourceDLL::CResourceDLL()
{
	m_hOldInst = AfxGetResourceHandle ();
	ASSERT (m_hOldInst != NULL);
	AfxSetResourceHandle( ConectorDLL.hResource);
}

CResourceDLL::~CResourceDLL()
{
	AfxSetResourceHandle( m_hOldInst );
}

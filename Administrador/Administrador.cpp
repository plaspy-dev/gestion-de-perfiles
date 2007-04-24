// Administrador.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "Administrador.h"
# include "PaisesView.h"
# include "FormUser.h"
# include "DepartamentosView.h"
# include "CiudadesView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static AFX_EXTENSION_MODULE AdministradorDLL= {NULL, NULL};
HINSTANCE g_hInstance=NULL;


BEGIN_MESSAGE_MAP(CAdministradorApp, CWinApp)
END_MESSAGE_MAP()


CAdministradorApp::CAdministradorApp()
{
}

CAdministradorApp theApp;


// CAdministradorApp initialization

BOOL CAdministradorApp::InitInstance()
{
	CWinApp::InitInstance();
	if (!AfxSocketInit())
	{
		AfxMessageBox(104);// IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}
	g_hInstance= m_hInstance;
	if (!AfxInitExtensionModule(AdministradorDLL, m_hInstance))
		return false;
# ifdef _DEBUG
	DllRegisterServer();
# endif
	return TRUE;
}

__declspec(dllexport) UINT LoadFunciones( CStringArray *funciones) {

	new CDynLinkLibrary(AdministradorDLL);
	(*funciones).Add("InitAdministrador");
	(*funciones).Add("InitPaises");
	(*funciones).Add("InitDepartamentos");
	(*funciones).Add("InitUsuarios");
	return 1;
}

/////////////////////////////////////////////////////////////////////////////
// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
	CRegKey key;
	HRESULT result;
	// registramos la ruta del dll
	if( (result= key.Create( HKEY_LOCAL_MACHINE, CApplicacion::GetAppKey() + "\\Addins\\Administrador"))==ERROR_SUCCESS) {

		if( (result= key.SetStringValue( "Nombre", "Administración"))!= ERROR_SUCCESS)
			return result;
		if( (result= key.SetStringValue( "Descripcion", "Modulo de Administración"))!= ERROR_SUCCESS)
			return result;
		if( (result= key.SetDWORDValue("ValidGrups", 1))!= ERROR_SUCCESS)
			return result;
		TCHAR szEXEPath[MAX_PATH];
		ASSERT(g_hInstance);
		GetModuleFileName(g_hInstance, szEXEPath, MAX_PATH);
		if( (result= key.SetStringValue( "FullPath", szEXEPath))!= ERROR_SUCCESS)
			return result;
		key.Close();
		return S_OK;
	}
	return result;
}

/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry
STDAPI DllUnregisterServer(void)
{
	CRegKey key;
	HRESULT result;
	if( (result= key.Open( HKEY_LOCAL_MACHINE, CApplicacion::GetAppKey() + "\\addins\\"))==ERROR_SUCCESS) {

		if( (result= key.DeleteSubKey("Administrador"))!=ERROR_SUCCESS)
			if( (result= key.RecurseDeleteKey("Administrador"))!=ERROR_SUCCESS)
				return result;
		key.Close();
		return S_OK;
	}
	return NULL;
}


__declspec(dllexport) CMultiDocTemplateXP *InitAdministrador(CString *strLabel, DWORD dwGrupo, CString *parent)
{
	if( (dwGrupo & 1)==0)
		return NULL;
	HICON hIcon;
	CWinApp* pApp = AfxGetApp();
	ASSERT(pApp != NULL);
	CMultiDocTemplateXP *pDocTemplate;


	pDocTemplate =  new CMultiDocTemplateXP(128,
											RUNTIME_CLASS(CComponentesDoc),
											RUNTIME_CLASS(CMDIChildWndXP),
											RUNTIME_CLASS(CFormUser));
	pApp->AddDocTemplate(pDocTemplate);
	*strLabel = "Usuarios";
	*parent= "Administrador";
	return pDocTemplate;
}

__declspec(dllexport) CMultiDocTemplateXP *InitPaises(CString *strLabel, DWORD dwGrupo, CString *parent)
{
	if( (dwGrupo & 1)==0)
		return NULL;
	HICON hIcon;
	CWinApp* pApp = AfxGetApp();
	ASSERT(pApp != NULL);
	CMultiDocTemplateXP *pDocTemplate;

	pDocTemplate =  new CMultiDocTemplateXP(128,
		RUNTIME_CLASS(CComponentesDoc),
		RUNTIME_CLASS(CMDIChildWndXP),
		RUNTIME_CLASS(CPaisesView));
	pApp->AddDocTemplate(pDocTemplate);
	*strLabel = "Paises";
	*parent= "Administrador";
	return pDocTemplate;
}

__declspec(dllexport) CMultiDocTemplateXP *InitDepartamentos(CString *strLabel, DWORD dwGrupo, CString *parent)
{
	if( (dwGrupo & 1)==0)
		return NULL;
	HICON hIcon;
	CWinApp* pApp = AfxGetApp();
	ASSERT(pApp != NULL);
	CMultiDocTemplateXP *pDocTemplate;

	pDocTemplate =  new CMultiDocTemplateXP(128,
		RUNTIME_CLASS(CComponentesDoc),
		RUNTIME_CLASS(CMDIChildWndXP),
		RUNTIME_CLASS(CDepartamentosView));
	pApp->AddDocTemplate(pDocTemplate);
	*strLabel = "Departamentos";
	*parent= "Administrador";
	return pDocTemplate;
}

__declspec(dllexport) CMultiDocTemplateXP *InitUsuarios(CString *strLabel, DWORD dwGrupo, CString *parent)
{
	if( (dwGrupo & 1)==0)
		return NULL;
	HICON hIcon;
	CWinApp* pApp = AfxGetApp();
	ASSERT(pApp != NULL);
	CMultiDocTemplateXP *pDocTemplate;

	pDocTemplate =  new CMultiDocTemplateXP(128,
		RUNTIME_CLASS(CComponentesDoc),
		RUNTIME_CLASS(CMDIChildWndXP),
		RUNTIME_CLASS(CCiudadesView));
	pApp->AddDocTemplate(pDocTemplate);
	*strLabel = "Ciudades";
	*parent= "Administrador";
	return pDocTemplate;
}

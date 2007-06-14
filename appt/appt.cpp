// appt.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "appt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CapptApp, CWinApp)
END_MESSAGE_MAP()

CapptApp::CapptApp()
{
}

CapptApp theApp;

BOOL CapptApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

__declspec(dllexport) CString GetExeName() {

	// no usada mas
	return "template.exe";
}

__declspec(dllexport) CString GetAppName() {

	return "Perfiles";
}

__declspec(dllexport) CString GetLongName() {
	
	return "Gestión de perfiles";
}

__declspec(dllexport) CString GetCompany() {

	return "SDG";
}

__declspec(dllexport) HBITMAP GetLogo() {

	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	HBITMAP hBmp = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), 
		MAKEINTRESOURCE(IDB_BLOGO),
		IMAGE_BITMAP, 
		0,0, 
		LR_LOADMAP3DCOLORS);
	return hBmp;
}

__declspec(dllexport) CString GetAppKey() {

	return "Software\\SDG\\" + GetAppName();
}

__declspec(dllexport) int GetIDApp() {

	// necesario para el keygen
	return 1;
}

__declspec(dllexport) CString GetURL() {

	return "http://mysql-demo.example.test";
}



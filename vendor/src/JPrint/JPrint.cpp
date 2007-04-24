// JPrint.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "JPrint.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: If this DLL is dynamically linked against the MFC DLLs,
//		any functions exported from this DLL which call into
//		MFC must have the AFX_MANAGE_STATE macro added at the
//		very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//


// CJPrintApp

BEGIN_MESSAGE_MAP(CJPrintApp, CWinApp)
END_MESSAGE_MAP()


// CJPrintApp construction

CJPrintApp::CJPrintApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CJPrintApp object

CJPrintApp theApp;


// CJPrintApp initialization

BOOL CJPrintApp::InitInstance()
{
	CWinApp::InitInstance();
#ifndef _DEBUG
	if(!RegisterFile(CApplicacion::GetAppPath("jprint.dll"))) {

		CString texto;
		texto.Format("%s Esta corrupto.\nPor favor ReInstale %s o Informe al Administrador del sistema.", CApplicacion::GetAppName(), CApplicacion::GetAppName());
		AfxMessageBox(texto, MB_ICONERROR);
		return FALSE;
	}
#endif
	return TRUE;
}

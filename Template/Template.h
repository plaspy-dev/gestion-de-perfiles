// Template.h: archivo de encabezado principal para la aplicación Template
//
#pragma once

#ifndef __AFXWIN_H__
	#error "incluir 'stdafx.h' antes de incluir este archivo para PCH"
#endif

#include "resource.h"       // Símbolos principales

# include <IOFile/IOFile.h>

# define WM_INSERTPKZ	WM_USER+1500
# define GRUPO_ADMINISTRADOR	1


class CTemplateApp : public CWinApp
{
public:
	CTemplateApp();
	LPARAM	m_mlparamold;

// Reemplazos
public:
	virtual BOOL InitInstance();
	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();
	HWND		RegisterApp(HWND mainframe);

// Implementación
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

	virtual CDocument* OpenDocumentFile(LPCTSTR lpszFileName);
	afx_msg void OnFileSave();
	afx_msg void OnFilePrintSetup();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

extern CTemplateApp theApp;
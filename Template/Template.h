// Template.h: archivo de encabezado principal para la aplicación Template
//
#pragma once

#ifndef __AFXWIN_H__
	#error "incluir 'stdafx.h' antes de incluir este archivo para PCH"
#endif

#include "resource.h"       // Símbolos principales

# include <IOFile/IOFile.h>
# include <JLang/JLang.h>

# define WM_INSERTPKZ	WM_USER+1500
# define WM_SHOWFALLECIDO	WM_USER+1501


# define GRUPO_ADMINISTRADOR	1


class CTemplateApp : public CWinAppEx
{
public:
	CTemplateApp();

// Reemplazos
public:
	virtual BOOL InitInstance();
	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();

// Implementación
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

	virtual CDocument* OpenDocumentFile(LPCTSTR lpszFileName);
};

extern CTemplateApp theApp;
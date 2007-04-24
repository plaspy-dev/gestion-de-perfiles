// JLang.h : main header file for the JLang DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

# ifndef __JLANG_EXPORT__
#define __JLANG_EXPORT__ __declspec(dllimport)
# endif

# include <Componentes/Componentes.h>

# include "Language.h"

// CJLangApp
// See JLang.cpp for the implementation of this class
//

class CJLangApp : public CWinApp
{
public:
	CJLangApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

#ifndef __JLANG_DLL__
#ifdef _DEBUG
# pragma comment(lib,"jlangd")
# else
# pragma comment(lib,"jlang")
# endif
#endif

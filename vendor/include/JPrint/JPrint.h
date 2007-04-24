// JPrint.h : main header file for the JPrint DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

# ifndef __JPRINT_EXPORT__
# define __JPRINT_EXPORT__ __declspec(dllimport)
# endif

# include <Componentes/Componentes.h>
# include "ReportGenerator.h"


# include "PrintRTF.h"
# include "ListDemoViewPrint.h"
# include "TreeListPrint.h"
# include "StringHelpers.h"
# include "HtmlPrint.h"
# include "NumberClass.h"


// CJPrintApp
// See JPrint.cpp for the implementation of this class
//

class CJPrintApp : public CWinApp
{
public:
	CJPrintApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

#ifndef _JPRINT_DLL_
#ifdef _DEBUG
# pragma comment(lib,"jprintd")
# else
# pragma comment(lib,"jprint")
# endif
#endif

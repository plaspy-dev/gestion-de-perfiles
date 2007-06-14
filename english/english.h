// english.h : main header file for the english DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CenglishApp
// See english.cpp for the implementation of this class
//

class CenglishApp : public CWinApp
{
public:
	CenglishApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

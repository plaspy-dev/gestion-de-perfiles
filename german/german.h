// german.h : main header file for the german DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CgermanApp
// See german.cpp for the implementation of this class
//

class CgermanApp : public CWinApp
{
public:
	CgermanApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

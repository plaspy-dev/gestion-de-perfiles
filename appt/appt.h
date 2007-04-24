// appt.h : main header file for the appt DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CapptApp
// See appt.cpp for the implementation of this class
//

class CapptApp : public CWinApp
{
public:
	CapptApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

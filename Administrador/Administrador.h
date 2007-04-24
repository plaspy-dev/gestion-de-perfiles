// Administrador.h : main header file for the Administrador DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif


class CAdministradorApp : public CWinApp
{
public:
	CAdministradorApp();

// Overrides
public:
	virtual BOOL InitInstance();
	DECLARE_MESSAGE_MAP()
};
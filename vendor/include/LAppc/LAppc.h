// LAppc.h : main header file for the LAppc DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

# ifndef __LAPPC_EXPORT__
# define __LAPPC_EXPORT__ __declspec(dllimport)
# endif

# include "definiciones.h"
# include "applicacion.h"
// vienen de componentes.h
# include "MD5Checksum.h"
#include "Crc32Static.h"
# include "myregkey.h"
#include "mytime.h"
# include "XWinVer.h"
# include "VistaTools.h"


__LAPPC_EXPORT__ bool RegisterFile(CString file, CString *szMD5=NULL, bool force=false);

class CLAppcApp : public CWinApp
{
public:
	CLAppcApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};


#ifndef _LAPPC_DLL_
	#ifdef _DEBUG
		# pragma comment(lib,"lappcd")
	# else
		# pragma comment(lib,"lappc")
	# endif
#endif
// Conector.h : main header file for the Conector DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

# ifndef __CONECTOR_EXPORT___
#define __CONECTOR_EXPORT___ __declspec(dllimport)
# endif

// dependencias
# include <componentes/componentes.h>


# include "DiskID.h"
# include "RealConector.h"
# include "Usuario.h"
# include "Usuario2.h"
# include "Usuario3.h"
# include "trialinfo.h"
# include "Query.h"
# include "conectorformview.h"

// CConectorApp
// See Conector.cpp for the implementation of this class
//

class CConectorApp : public CWinApp
{
public:
	CConectorApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

__CONECTOR_EXPORT___ void InitAsistente(CBitmap *bmpHeader=NULL,CBitmap *bmpWater=NULL);
__CONECTOR_EXPORT___ DWORD GetDlgIngreso(bool existe, CStringArray &datos, bool local=false);
__CONECTOR_EXPORT___ DWORD GetDlgIngreso(bool existe, CUsuario2 *usuario, bool local=false);
__CONECTOR_EXPORT___ DWORD GetDlgIngreso(bool existe, CUsuario3 *usuario, bool local=false);
__CONECTOR_EXPORT___ DWORD GetDlgDataBases();

#ifndef _CONECTOR_DLL_
	#ifdef _DEBUG
		# pragma comment(lib,"conectord")
	# else
		# pragma comment(lib,"conector")
	# endif
#endif


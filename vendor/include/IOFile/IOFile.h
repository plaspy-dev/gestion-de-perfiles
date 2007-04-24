// IOFile.h : main header file for the IOFile DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

# ifndef __IOFILE_EXPORT__
#define __IOFILE_EXPORT__ __declspec(dllimport)
# endif

// dependencias
# include <conector/conector.h>


# define		EXTENCION_DESCONOCIDA	0
# define		EXTENCION_HTML			1
# define		EXTENCION_SIT			2
# define		EXTENCION_XML			3
# define		EXTENCION_TXT			4
# define		EXTENCION_EXCEL			5

# define		TIPO_DESCONOCIDA		20
# define		TIPO_NORMAL				0
# define		TIPO_MARCA				1
# define		TIPO_MODELO				2
# define		TIPO_ITEM				3
# define		TIPO_MODELOEX			4
# define		TIPO_CLIENTE			5
# define		TIPO_ODS				6
# define		TIPO_REPARACION			7
# define		TIPO_CONSULTA			8


# define	ID_FIND_TERCEROS	105
// CIOFileApp
// See IOFile.cpp for the implementation of this class
//


# include "Defender.h"
# include "DlgElementos.h"
# include "Export.h"
# include "FileInfo.h"
# include "FileWatch.h"
# include "HtmlFile.h"
# include "Import.h"
# include "XUnzip.h"
# include "ScreenSaverDE.h"
# include "TonyJpegDecoder.h"
# include "TonyJpegEncoder.h"
# include "OleRichEditCtrl.h"
# include "ImageDataObject.h"

class CIOFileApp : public CWinApp
{
public:
	CIOFileApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

// funciones comunes
__IOFILE_EXPORT__ CString	NumerosToMarcas(CStringArray &nombres, CUIntArray &id, CString valor);
__IOFILE_EXPORT__ CString	MarcasToNumeros( CStringArray &nombres, CUIntArray  &id, CString valor);
__IOFILE_EXPORT__ int DlgListado(CString titulo,CStringArray *elementos, CString *elemento, CStringArray *descripciones=NULL);
__IOFILE_EXPORT__ CString	MarcasToNumerosEx( CStringArray &nombres, CUIntArray  &id, CString valor, CString &salida);
__IOFILE_EXPORT__  bool Exportar(CMyListCtrl *pLista,CString Titulo, bool showfile= false);
__IOFILE_EXPORT__  bool Exportar(CTreeListCtrl *pTree,CString Titulo, bool showfile=false);
__IOFILE_EXPORT__  DWORD GetLicenceDlg();
__IOFILE_EXPORT__  DWORD GetProgressDlg(CStringArray *querys, CStringArray *mensajes, const CString titulo,bool silent=false);


#ifndef _IOFILE_DLL_
	#ifdef _DEBUG
		# pragma comment(lib,"iofiled")
	# else
		# pragma comment(lib,"iofile")
	# endif
#endif
// Componentes.h : main header file for the Componentes DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#ifndef __COMPONENTES_EXPORT__
	#define __COMPONENTES_EXPORT__ __declspec(dllimport)
#endif

// dependencias
# include <definiciones.h>
# include <lappc/lappc.h>


# define ID_WINDOW_CLOSE_ALL	32775
# define ID_NEXT_PANEEX			32784
# define ID_PREV_PANEEX			32785

// solo para el mainframe
# define	ACELERADORES_PREFIJO		32000
# define	MESSAGE_DELAY				3000

# define	ID_VER_NAVEGADOR			136

// temporizadores
# define	TIMER_RESTORE_STATUS_BAR	123
# define	JUPDATER_TIMER				50041
# define	JUPDATER_INTERVALO		60*1000*60
# define	JUPDATER_INTERVALO_INICIAL	1000*60

// iniciamos la importacion de todas las librerias

# include "ButtonXP.h"
# include "CheckListBoxEx.h"
# include "ColorButton.h"
# include "COLORLISTBOX.H"
# include "ComboBoxXP.h"
# include "Draw.h"
# include "EditAyuda.h"
# include "EditFloat.h"
# include "EditTiempo.h"
# include "EditXP.h"
# include "FadeWnd.h"
# include "GuiControlBar.h"
# include "GuiDockToolBarEx.h"
# include "HyperLink.h"
# include "INFOBARCTRL.H"
# include "ListCtrlSize.h"
# include "MasEdit.h"
# include "MsgBoxHook.h"
# include "MyListCtrl.h"
# include "nbMessageBox.h"
# include "newmenu\DialogXP.h"
# include "newmenu\MDIChildWndXP.h"
# include "newmenu\MDIFrameWndXP.h"
# include "newmenu\MultiDocTemplateXP.h"
# include "newmenu\NewMenu.h"
# include "SEPARADOR\CHECKBOXSEPARATOR.H"
# include "SEPARADOR\SECTIONSEPARATOR.H"
# include "SEPARADOR\SEPARATOR.H"
# include "SEPARADOR\STATICSEPARATOR.H"
# include "SEPARADOR\VISUALSTYLESXP.H"
# include "SortHeaderCtrl.h"
# include "SplashScreenEx.h"
# include "SplitterControl.h"
# include "StatusBarXP.h"
# include "TabCtrlSSL.h"
# include "TabPageSSL.h"
# include "TextProgressCtrl.h"
# include "ToolbarXP.h"
# include "Tools.h"
# include "TreeCtrlXP.h"
# include "treelistctrl\ThreadTreeStat.h"
# include "treelistctrl\TreeListCtrl.h"
# include "treelistctrl\TreeListItem.h"
# include "XInfoTip.h"
# include "xmlreader.h"
# include "xmlreader.h"
# include "xmlwriter.h"
# include "XTabCtrl.h"
# include "akTrayIcon.h"
#include "logomdi.h"
#include "BitmapPicture.h"
#include "mditabs/MDITabs.h"
#include "mditabs/LBSpinButtonCtrl.h"
#include "XStatusBar.h"
#include "extras/TreeNavBar.h"
# include "extras/MiscelaneosEx.h"
# include "ComponentesDoc.h"
# include "MemDC.h"
# include "MatrixStatic.h"
# include "VListCtrl.h"
# include "shockwaveflash1.h"
# include "EnBitmap.h"
# include "ImageProcessors.h"
# include "WinAppEx.h"
# include "KCSideBannerWnd.h"
# include "hilos/ThreadAuto.h"
# include "HTMLCtrl.h"
# include "resizablegrip.h"
# include "resizableformview.h"
# include "resizabledialog.h"

// CComponentesApp
// See Componentes.cpp for the implementation of this class
//


// tipo de documentos a importar
typedef CMultiDocTemplateXP *( * LPDLLFUNC)(CString *, DWORD, CString *);
typedef CMultiDocTemplateXP *( * LPDLLFUNC3)(CString *, const CMiscelaneosEx &, DWORD, CString *);
// funcion principal para cargar funciones.
typedef UINT *(*LOADFUNCIONES)(CStringArray *);



class CComponentesApp : public CWinApp
{
public:
	CComponentesApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

__COMPONENTES_EXPORT__ int MyMessageBox(CString texto, DWORD tipo=MB_OK, CString registro=""); 
__COMPONENTES_EXPORT__ ULONG GetMaxLen( CString cadena);
__COMPONENTES_EXPORT__ bool IsValid( CString cadena);
__COMPONENTES_EXPORT__ bool IsValid( CString cadena, CString valor);
__COMPONENTES_EXPORT__ void SetError( CWnd *elemento, CString error, DWORD tipo=0);
__COMPONENTES_EXPORT__ int SetErrorEx( CWnd *elemento, CString error, DWORD tipo=0, CString registro="");
__COMPONENTES_EXPORT__ void RecalculateScroll(CFormView *parent, int delta=10);
__COMPONENTES_EXPORT__ bool IsWindowsNT();
__COMPONENTES_EXPORT__ CString GetLeftString( CString cadena, CString separador);
__COMPONENTES_EXPORT__ CString GetRightString(CString cadena, CString separador);
__COMPONENTES_EXPORT__ void ValidateScroll(CWnd *componente);
__COMPONENTES_EXPORT__ void AboutBox(const CString &version,HICON icono=NULL, const CString &producto="");

#ifndef _COMPONENTES_DLL_
	#ifdef _DEBUG
		# pragma comment(lib,"componentesd")
	# else
		# pragma comment(lib,"componentes")
	# endif
#endif


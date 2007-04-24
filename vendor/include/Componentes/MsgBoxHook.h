////////////////////////////////////////////////////////////////////////////////
//
//               Copyright (C) 2003, nabocorp
//                     All Rights Reserved
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _MSGBOXHOOK_H_INCLUDED
#define _MSGBOXHOOK_H_INCLUDED

 __COMPONENTES_EXPORT__ bool InstallMessageBoxHook(CString strDoNotShowAgain = "", CString strDoNotAskAgain = "");
 __COMPONENTES_EXPORT__ void UninstallMessageBoxHook();

 __COMPONENTES_EXPORT__ int NabMessageBox(LPCTSTR lpszText, UINT nType = MB_OK, UINT nIDHelp = 0 );
 __COMPONENTES_EXPORT__ int AFXAPI NabMessageBox( UINT nIDPrompt, UINT nType = MB_OK, UINT nIDHelp = (UINT) -1 );

#endif

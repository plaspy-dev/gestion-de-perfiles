////////////////////////////////////////////////////////////////////////////////
//
//               Copyright (C) 2003, nabocorp
//                     All Rights Reserved
//
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MsgBoxHook.h"
#include "separador/Separator.h"

#pragma warning(disable:4786)
#include <map>
#include <vector>
#include <algorithm>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

HHOOK g_hHook;

struct MessageBoxInfo
{
    CString strDoNotString;
    bool bChecked;
};

map<HWND, WNDPROC> mapProc;
map<HWND, CSeparator*> mapSep;
map<HWND, CButton*> mapBut;
map<CString, MessageBoxInfo> mapChecked;

CString g_strDoNotShowAgain = "Do not show again";
CString g_strDoNotAskAgain = "Do not ask again";

#define IDC_SEPARATOR 111
#define IDC_CHECKBOX 666

//
// Message Box identification stuff
//

struct count
{
    int nIcons;
    int nStatics;
    int nButtons;
    int nOthers;
};

BOOL CALLBACK InspectChildren(HWND hwnd, LPARAM lParam)
{
    struct count *pCount = (struct count*) lParam;

    TCHAR szBuf[256];
    GetClassName( hwnd, szBuf, 256 );
    if (lstrcmp(szBuf, _T("Static")) == 0)
    {
        DWORD dwStyle = GetWindowLong(hwnd, GWL_STYLE);
        if (dwStyle & SS_ICON) pCount->nIcons++;
        else pCount->nStatics++;
    }
    else if (lstrcmp(szBuf, _T("Button")) == 0)
    {
        pCount->nButtons++;
    }
    else
    {
        pCount->nOthers++;
    }

    // stop as soon we see a problem
    if (   pCount->nOthers  > 0
        || pCount->nIcons   > 1
        || pCount->nStatics > 1
        || pCount->nButtons > 3)
    {
        return FALSE;
    }

    return TRUE;
}

bool IsAMessageBox(HWND hwnd)
{
    TCHAR szBuf[256];
    GetClassName( hwnd, szBuf, 256 );
    if (lstrcmp(szBuf, _T("#32770")))
        return false;

    struct count c;
    memset(&c, 0, sizeof(c));

    EnumChildWindows(hwnd, InspectChildren, (LPARAM) &c);

    return (   c.nOthers  == 0
            && c.nIcons   == 1
            && c.nStatics == 1
            && c.nButtons  > 0
            && c.nButtons  < 4);
}

//
// Move horizontally all the buttons of a dialog
//

BOOL CALLBACK MoveButtons(HWND hwnd, LPARAM lParam)
{
    int offset = * (int*) lParam;

    TCHAR szBuf[256];
    GetClassName( hwnd, szBuf, 256 );
    if (lstrcmp(szBuf, _T("Button")) == 0)
    {
        // get the window placement (position and size)
        WINDOWPLACEMENT placement;
        ::GetWindowPlacement(hwnd, &placement);
        CRect rc = placement.rcNormalPosition;

        ::SetWindowPos(hwnd, NULL, rc.left+offset/2, rc.top, rc.Width(), rc.Height(), SWP_NOZORDER);
    }

    return TRUE;
}

//

// Hook stuff
//

LRESULT CALLBACK HookWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    // is this a window for us?
    if (mapProc.find(hWnd) == mapProc.end())
    {
        // this should never happen
        ASSERT(false);
        return ::DefWindowProc(hWnd, uMsg, wParam, lParam );
    }

    // WM_NCDESTROY is the very last message we can get for a window
    if (uMsg == WM_NCDESTROY)
    {
        // clean up
        LRESULT rc = CallWindowProc( mapProc[hWnd], hWnd, uMsg, wParam, lParam );
        mapProc.erase(mapProc.find(hWnd));

        // clean up
        if (mapSep.find(hWnd) != mapSep.end())
        {
            delete mapSep[hWnd];
            mapSep.erase(mapSep.find(hWnd));
        }

        // clean up
        if (mapBut.find(hWnd) != mapBut.end())
        {
            delete mapBut[hWnd];
            mapBut.erase(mapBut.find(hWnd));
        }

        return rc;
    }
 
    // get the message
    CString strMessage = "";
    HWND hStatic = ::GetDlgItem(hWnd, 65535);
    if (hStatic)
    {
        int nLength = ::SendMessage(hStatic, WM_GETTEXTLENGTH, 0, 0);
        TCHAR* szMsg = new TCHAR[nLength+1];
        ::SendMessage(hStatic, WM_GETTEXT, nLength+1, (long) szMsg);
        strMessage = szMsg;
        delete [] szMsg;
    }

    // do we have this one?
    map<CString, MessageBoxInfo>::iterator it = mapChecked.find(strMessage);
    if (it == mapChecked.end())
        return CallWindowProc( mapProc[hWnd], hWnd, uMsg, wParam, lParam );

    // if the user clicked the checkbox then handle this ourself
    if (uMsg == WM_COMMAND && wParam == IDC_CHECKBOX)
    {
        it->second.bChecked = !it->second.bChecked;
        ::SendMessage(::GetDlgItem(hWnd, IDC_CHECKBOX), BM_SETCHECK, it->second.bChecked ? BST_CHECKED : BST_UNCHECKED, 0);
        return 0;
    }

    // call the default windowproc
    LRESULT rc = CallWindowProc( mapProc[hWnd], hWnd, uMsg, wParam, lParam );
 
    if (uMsg == WM_INITDIALOG)
    {
        // some metrics
        static const int k_nCaptionHeight = GetSystemMetrics(SM_CYCAPTION);
        static const int k_nBorderHeight = GetSystemMetrics(SM_CYEDGE);
        static const int k_nBorderWidth = GetSystemMetrics(SM_CXEDGE);
        static const int k_nSepHeigth = 3;
        static const int k_nMargin = 7;

        // get standard font (from static)
        HFONT hFont = (HFONT) ::SendMessage(hStatic, WM_GETFONT, 0, 0);

        // calc the size of the checkbox
        CDC *pDC = CWnd::FromHandle(hWnd)->GetDC();
        int dc = pDC->SaveDC();
        pDC->SelectObject(CFont::FromHandle(hFont));
        CSize size = pDC->GetTextExtent(it->second.strDoNotString);
        pDC->RestoreDC(dc);
        CWnd::FromHandle(hWnd)->ReleaseDC(pDC);

        // more metrics
        int nCheckboxHeight = min(13, size.cy);
        int nOffset = 2*k_nSepHeigth + nCheckboxHeight + k_nMargin;

        // get the window placement (position and size)
        WINDOWPLACEMENT placement;
        ::GetWindowPlacement(hWnd, &placement);
        CRect rc = placement.rcNormalPosition;
        rc.bottom -= k_nMargin;

        // make sure the box is wide enough for the checkbox
        size.cx += 2*k_nMargin + 2*k_nBorderWidth + 13 + 6;
        if (rc.Width() < size.cx)
        {
            // move each button so they appear centered
            int offset = size.cx - rc.Width();
            EnumChildWindows(hWnd, MoveButtons, (LPARAM) &offset);

            // adjust window width
            rc.right = rc.left + size.cx;
        }

        // set the window pos
        ::SetWindowPos(hWnd, NULL, rc.left, rc.top-nOffset/2, rc.Width(), rc.Height()+nOffset, SWP_NOZORDER);

        // ajust rect
        rc.bottom -= k_nCaptionHeight + k_nBorderHeight;

        // create the separator
        CSeparator *pSep = new CSeparator;
        CRect rc_sep(-1, rc.Height(), rc.Width(), rc.Height()+k_nSepHeigth);
        pSep->Create(/*NULL, "",*/SS_SIMPLE|WS_CHILD|WS_VISIBLE, rc_sep, CWnd::FromHandle(hWnd), IDC_SEPARATOR);
        mapSep[hWnd] = pSep;

        // create the button
        CButton *pButton = new CButton;
        CRect rc_but(k_nMargin, rc.Height()+2*k_nSepHeigth, rc.Width(), rc.Height()+2*k_nSepHeigth+nCheckboxHeight);
        pButton->Create(it->second.strDoNotString, WS_CHILD|WS_VISIBLE|BS_CHECKBOX, rc_but, CWnd::FromHandle(hWnd), IDC_CHECKBOX);
        pButton->SetFont(CFont::FromHandle(hFont));
        pButton->SetCheck(it->second.bChecked);
        mapBut[hWnd] = pButton;
    }

    return rc;
}

LRESULT CALLBACK SetHook( int nCode, WPARAM wParam, LPARAM lParam )
{   
    if ( nCode == HC_ACTION )
    {      
        CWPSTRUCT* pwp = (CWPSTRUCT*)lParam;
        if (pwp->message == WM_INITDIALOG)
        {
            // alter message boxes only by subclassing them
            if (IsAMessageBox(pwp->hwnd))
                mapProc[pwp->hwnd] = (WNDPROC) SetWindowLong(pwp->hwnd, GWL_WNDPROC, (LONG) HookWndProc);
        }
    }
    return CallNextHookEx(g_hHook, nCode, wParam, lParam );
}

bool InstallMessageBoxHook(CString strDoNotShowAgain, CString strDoNotAskAgain)
{
    g_strDoNotShowAgain = strDoNotShowAgain;
    g_strDoNotAskAgain = strDoNotAskAgain;

    g_hHook = SetWindowsHookEx(WH_CALLWNDPROC,
                               (HOOKPROC)SetHook,
                               NULL,
                               GetCurrentThreadId());
    return (g_hHook != NULL);
}

void UninstallMessageBoxHook()
{
    if (g_hHook)
    {
        UnhookWindowsHookEx(g_hHook);
    }
}

//
// message box
//

int NabMessageBox(LPCTSTR lpszText, UINT nType, UINT nIDHelp)
{
    mapChecked[lpszText].strDoNotString = ( (nType&0xF) ? g_strDoNotShowAgain : g_strDoNotAskAgain);
    mapChecked[lpszText].bChecked = false;
    int rc = AfxMessageBox(lpszText, nType, nIDHelp);
    if (mapChecked.find(lpszText) != mapChecked.end())
    {
        bool bChecked = mapChecked[lpszText].bChecked;
        mapChecked.erase(mapChecked.find(lpszText));
        return (bChecked ? -1 : 1) * rc;
    }
    return rc;
}

int AFXAPI NabMessageBox( UINT nIDPrompt, UINT nType, UINT nIDHelp)
{
    CString strText;
    strText.LoadString(nIDPrompt);
    return NabMessageBox(strText, nType, nIDHelp);
}


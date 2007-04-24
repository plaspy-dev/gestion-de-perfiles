///////////////////////////////////////////////////////////////////////////////
//
// ButtonXP.cpp : implementation file
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
# include "Componentes.h"
#include "ButtonXP.h"
#include "Draw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "newmenu/NewMenu.h"

////////////////////////////////////////////////////////////////////////////////
CButtonXP::CButtonXP ()
{
	m_bValidate= true;
}

////////////////////////////////////////////////////////////////////////////////
CButtonXP::~CButtonXP ()
{
}

////////////////////////////////////////////////////////////////////////////////
void CButtonXP::PreSubclassWindow ()
{
	FUNCTION_STACK_TRACE("FFFFE000");
	if(!m_WVersion.IsVistaorLater())
		ModifyStyle (0, BS_OWNERDRAW);
    m_MouseMgr.Init (m_hWnd);
}

////////////////////////////////////////////////////////////////////////////////
void CButtonXP::DrawItem (LPDRAWITEMSTRUCT pDis)
{
	FUNCTION_STACK_TRACE("FFFFE001");
    CBufferDC cDC (pDis->hDC);
	if(!cDC.m_hDC)
		return;
    CRect rcItem (pDis->rcItem);
    CFontDC font (cDC,DEFAULT_GUI_FONT);
    CPenDC pen (cDC);
    CBrushDC brush (cDC);
	DWORD color3dface= GetSysColor(COLOR_3DFACE);

	cDC.FillSolidRect( rcItem, color3dface);
	rcItem.DeflateRect(1,1,1,1);

    if ( (pDis->itemState & ODS_DISABLED) ||
         (!m_MouseMgr.MouseOver() && !(pDis->itemState & ODS_SELECTED)) )
    {
        if ( (pDis->itemState & ODS_FOCUS) ||
            LOWORD(::SendMessage (::GetParent (m_hWnd), DM_GETDEFID, 0, 0)) == GetDlgCtrlID() )
        {
            pen.Color (::GetSysColor (COLOR_3DDKSHADOW));
        }
        else
        {
            pen.Color (::GetSysColor (COLOR_3DSHADOW));
        }
        brush.Color (HLS_TRANSFORM (color3dface, +15, 0));
    }
    else
    {
		COLORREF crBorder = GetSysColor (COLOR_HIGHLIGHT);
        pen.Color (crBorder);

        if ( (pDis->itemState & ODS_SELECTED) )
        {
			brush.Color( HLS_TRANSFORM (crBorder, +50, -50));
            cDC.SetTextColor (RGB(240,240,240));
        }
        else
        {
            brush.Color (HLS_TRANSFORM (crBorder, +80, -66));
            cDC.SetTextColor (::GetSysColor (COLOR_BTNTEXT));
        }
    }
    if ( (pDis->itemState & ODS_DISABLED) )
    {
        cDC.SetTextColor (::GetSysColor (COLOR_GRAYTEXT));
    }
    else if ( (pDis->itemState & ODS_SELECTED) )
    {
        cDC.SetTextColor (RGB(240,240,240));
    }
    else if ( m_MouseMgr.MouseOver() )
    {
        cDC.SetTextColor (0);
    }
    else
    {
        cDC.SetTextColor (::GetSysColor (COLOR_BTNTEXT));
    }

	cDC.Rectangle (rcItem);
	cDC.SetBkMode (TRANSPARENT);
	cDC.DrawText (CWindowText (this), rcItem, DT_SINGLELINE|DT_CENTER|DT_VCENTER);

	if ( (pDis->itemState & ODS_FOCUS) )
	{
		rcItem.DeflateRect (2, 2);
		cDC.DrawFocusRect (rcItem);
	}
}

////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CButtonXP, CButton)
    //{{AFX_MSG_MAP(CButtonXP)
    ON_WM_MOUSEMOVE()
    ON_WM_MOUSEOUT()
    //}}AFX_MSG_MAP
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////////
void CButtonXP::OnMouseMove (UINT, CPoint)
{
	FUNCTION_STACK_TRACE("FFFFE002");
    Default();
    m_MouseMgr.OnMouseMove();
}

///////////////////////////////////////////////////////////////////////////////
void CButtonXP::OnMouseOut ()
{
	FUNCTION_STACK_TRACE("FFFFE003");
    m_MouseMgr.OnMouseOut();
}

void CButtonXP::OnSetFocus(CWnd* pOldWnd)
{
	CButton::OnSetFocus(pOldWnd);
	if( m_bValidate)
		ValidateScroll(this);
}

BOOL CButtonXP::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->message==WM_KEYUP && pMsg->wParam==VK_ESCAPE) 
        GetParent()->GetNextDlgTabItem(this, true)->SetFocus();
	return CButton::PreTranslateMessage(pMsg);
}

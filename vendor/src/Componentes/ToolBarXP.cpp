///////////////////////////////////////////////////////////////////////////////
//
// ToolBarXP.cpp: implementation of the CToolBarXP class.
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
# include "Componentes.h"
#include "ToolBarXP.h"
#include "Tools.h"
#include "Draw.h"
# include "newmenu/newmenu.h"

# include <atlimage.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// From <afximpl.h>
struct AUX_DATA
{
	int _unused1, _unused2;
	int _unused3, _unused4;
	int cxBorder2, cyBorder2;
};

extern __declspec(dllimport) AUX_DATA afxData;

class INIT_afxData
{
public:
    INIT_afxData ()
    {
        afxData.cxBorder2 = afxData.cyBorder2 = 0;
    }
} g_afxData2;

///////////////////////////////////////////////////////////////////////////////
CToolBarXP::CToolBarXP () : m_bCheckVisibility (false)
{
}

///////////////////////////////////////////////////////////////////////////////
BOOL CToolBarXP::LoadToolBar (UINT nIDResource, COLORREF rgbBack/*=RGB(192,192,192)*/)
{
	FUNCTION_STACK_TRACE("FFFFE0C8");
    if ( !CToolBar::LoadToolBar (nIDResource) )
    {
        return false;
    }
    SendMessage (TB_SETEXTENDEDSTYLE, 0, TBSTYLE_EX_DRAWDDARROWS);

	m_bmToolbar.LoadBitmap(nIDResource);
	CSize tamano= m_bmToolbar.GetTamano();
	
	m_imgToolbar.Create(tamano.cy, tamano.cy, ILC_COLOR24|ILC_MASK, 1, 1);
	m_imgToolbar.Add(&m_bmToolbar, rgbBack);
	GetToolBarCtrl().SetHotImageList(&m_imgToolbar);

	m_bmToolbarDis.LoadBitmap(nIDResource);
	m_bmToolbarDis.MakeDisabled(rgbBack); // RGB(255, 0, 255));
	m_imgToolbarDis.Create(tamano.cy, tamano.cy, ILC_COLOR24|ILC_MASK, 1, 1);
	m_imgToolbarDis.Add(&m_bmToolbarDis, rgbBack);
	GetToolBarCtrl().SetDisabledImageList(&m_imgToolbarDis);

	m_bmToolbarNA.LoadBitmap(nIDResource);
	m_bmToolbarNA.MakeNotActive(rgbBack); // RGB(255, 0, 255));
	m_imgToolbarNA.Create(tamano.cy, tamano.cy, ILC_COLOR24|ILC_MASK, 1, 1);
	m_imgToolbarNA.Add(&m_bmToolbarNA, rgbBack);
	GetToolBarCtrl().SetImageList(&m_imgToolbarNA);

	tamano.cx= tamano.cy;
	SetSizes(CSize(tamano.cy+7,tamano.cy+7), tamano);

    return true;
}

///////////////////////////////////////////////////////////////////////////////
bool CToolBarXP::InsertControl (int nIndex, CWnd& Ctrl)
{
	FUNCTION_STACK_TRACE("FFFFE0C9");
    TBBUTTON tbbutton;
    int nCount = DefWindowProc (TB_BUTTONCOUNT, 0, 0);
    CWindowRect rcCtrl (Ctrl);

    tbbutton.iBitmap = rcCtrl.Width();
    tbbutton.idCommand = Ctrl.GetDlgCtrlID();
    tbbutton.fsState = 0;
    tbbutton.fsStyle = TBSTYLE_SEP;
    tbbutton.bReserved[0] = 0;
    tbbutton.bReserved[1] = 0;
    tbbutton.iString = -1;
    tbbutton.dwData = NULL;

    if ( nIndex < 0 || nIndex > nCount )
    {
        nIndex = nCount;
    }
    if ( GetToolBarCtrl().InsertButton (nIndex, &tbbutton) )
    {
        m_bDelayedButtonLayout = true;

        return true;
    }
    return false;
}

///////////////////////////////////////////////////////////////////////////////
void CToolBarXP::TrackPopupMenu (int nID, CMenu* pMenu)
{
	FUNCTION_STACK_TRACE("FFFFE0CA");
    CRect rcTBItem;
    int nTBIndex = CommandToIndex (nID);
    TPMPARAMS tpmp = { sizeof TPMPARAMS };
    
    GetItemRect (nTBIndex, rcTBItem);
    ClientToScreen (rcTBItem);
    CopyRect (&tpmp.rcExclude, rcTBItem);
    rcTBItem.OffsetRect (4, 0);
    
    SetButtonStyle (nTBIndex, GetButtonStyle (nTBIndex)|TBBS_INDETERMINATE);
    UpdateWindow();
    ::TrackPopupMenuEx (pMenu->GetSafeHmenu(), TPM_LEFTBUTTON, tpmp.rcExclude.left, tpmp.rcExclude.bottom, GetParentFrame()->GetSafeHwnd(), &tpmp);
    SetButtonStyle (nTBIndex, GetButtonStyle (nTBIndex)&~TBBS_INDETERMINATE);
}

///////////////////////////////////////////////////////////////////////////////
LRESULT CToolBarXP::DefWindowProc (UINT nMsg, WPARAM wParam, LPARAM lParam)
{
    LRESULT lRes = CToolBar::DefWindowProc (nMsg, wParam, lParam);

    if ( m_bCheckVisibility && nMsg == TB_GETBUTTON )
    {
        TBBUTTON* pButton = (TBBUTTON*)lParam;

        if ( IS_CONTROL(*pButton) )
        {
            if ( m_bCheckVisibility == 1 ) pButton->fsState |=  TBSTATE_HIDDEN;
            else                           pButton->fsState &= ~TBSTATE_HIDDEN;
        }
    }
    return lRes;
}

///////////////////////////////////////////////////////////////////////////////
CSize CToolBarXP::CalcDynamicLayout (int nLength, DWORD dwMode)
{
	FUNCTION_STACK_TRACE("FFFFE0CB");
    bool bHideControls = (dwMode & LM_VERTDOCK) == LM_VERTDOCK;
    m_bCheckVisibility = (BYTE)(bHideControls ? 1 : 2);
    CSize size = CToolBar::CalcDynamicLayout (nLength, dwMode);
    m_bCheckVisibility = false;

    if ( dwMode & LM_COMMIT )
    {
        TBBUTTON tbbutton;
        int nCount = DefWindowProc (TB_BUTTONCOUNT, 0, 0);

        for ( int i = 0; i < nCount; i++ )
        {
            VERIFY(DefWindowProc (TB_GETBUTTON, i, (LPARAM)&tbbutton));

            if ( !IS_CONTROL(tbbutton) )
            {
                continue;
            }
            CWnd* pWnd = GetDlgItem (tbbutton.idCommand);

            if ( pWnd == NULL )
            {
                continue;
            }
            if ( bHideControls )
            {
                GetToolBarCtrl().HideButton (tbbutton.idCommand, true);
                pWnd->ShowWindow (SW_HIDE);
            }
            else
            {
                GetToolBarCtrl().HideButton (tbbutton.idCommand, false);

                // Update control position
                CRect rcControl;

                VERIFY (DefWindowProc (TB_GETITEMRECT, i, (LPARAM)&rcControl));
                rcControl.DeflateRect (1, 1);
                
                CWindowRect rcCtrl (pWnd);
                int         nNoSize = 0;

                if ( rcControl.Width() == rcCtrl.Width() )
                {
                    nNoSize = SWP_NOSIZE;
                }
                pWnd->SetWindowPos (NULL, rcControl.left, rcControl.top, rcControl.Width(), rcControl.Height(),
                                    nNoSize|SWP_NOACTIVATE|SWP_NOZORDER|SWP_SHOWWINDOW);
            }
        }
    }
    return size;
}

///////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CToolBarXP, CToolBar)
    ON_WM_PAINT()
    ON_NOTIFY_REFLECT(TBN_DROPDOWN, OnButtonDropDown)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

IMPLEMENT_DYNAMIC(CToolBarXP, CToolBar)

void CToolBarXP::DrawGripper(CDC* pDC, const CRect& rect) {

	FUNCTION_STACK_TRACE("FFFFE0CD");
	if( !pDC || !pDC->m_hDC)
		  return;

	COLORREF color= CNewMenu::GetMenuBarColorXP();
	pDC->FillSolidRect(rect, color);
	color = HLS_TRANSFORM (color, -50, 0);
    for (int n=rect.top+m_cyTopBorder+4;n<rect.Height()-m_cyTopBorder-m_cyBottomBorder-8;n+=4)
    {
		pDC->FillSolidRect(rect.left+5+m_cxLeftBorder,n+1+1,2,2,RGB(255,255,255));
		pDC->FillSolidRect(rect.left+4+m_cxLeftBorder,n  +1,2,2,color);
    }
}

///////////////////////////////////////////////////////////////////////////////
// Paint the toolbar
void CToolBarXP::OnPaint ()
{
	/*if(m_WVersion.IsVistaorLater()) {
		Default();
		return;
	}*/

    if ( m_bDelayedButtonLayout )
    {
        Layout();
    }
    CPaintDC cpDC (this);
    CBufferDC cDC (cpDC);

	if( !cDC.m_hDC) {
		Default();
		return;
	}

    CRect rcClip;
    cDC.GetClipBox (rcClip);
    cDC.SetBkMode (TRANSPARENT);
    cDC.SelectObject (CFont::FromHandle ((HFONT)GetStockObject (DEFAULT_GUI_FONT)));
	COLORREF colorface= ::GetSysColor (COLOR_3DFACE);
	COLORREF crHighLight = ::GetSysColor (COLOR_HIGHLIGHT);
	COLORREF mybrush;

	// cDC.FillSolidRect (rcClip, colorface);
	{
		TRIVERTEX        vert[2];
		GRADIENT_RECT    gRect;

		vert [0] .x      = rcClip.left;
		vert [0] .y      = rcClip.top;
		vert [0] .Red    = 0xff00;
		vert [0] .Green  = 0xff00;
		vert [0] .Blue   = 0xff00;
		vert [0] .Alpha  = 0x0000;

		vert [1] .x      = rcClip.right;
		vert [1] .y      = rcClip.bottom;
		vert [1] .Red    = GetRValue(colorface)<<8;
		vert [1] .Green  = GetGValue(colorface)<<8;
		vert [1] .Blue   = GetBValue(colorface)<<8;
		vert [1] .Alpha  = 0x0000;

		gRect.UpperLeft  = 0;
		gRect.LowerRight = 1;

		cDC.GradientFill(vert,2,&gRect,1,GRADIENT_FILL_RECT_V);
	}

    CPoint ptCursor;
    ::GetCursorPos (&ptCursor);
    ScreenToClient (&ptCursor);

    CClientRect rcClient (this);
    HIMAGELIST m_hImageList = (HIMAGELIST)DefWindowProc (TB_GETIMAGELIST, 0, 0);
    TBBUTTON tbbutton;
    int nCount = DefWindowProc (TB_BUTTONCOUNT, 0, 0);
    int nHotItem = GetToolBarCtrl().GetHotItem();

    for ( int i = 0; i < nCount; i++ )
    {
        VERIFY(DefWindowProc (TB_GETBUTTON, i, (LPARAM)&tbbutton));

        if ( !IS_VISIBLE(tbbutton) )
        {
            continue;
        }
        CRect rcButton;
        VERIFY(DefWindowProc (TB_GETITEMRECT, i, (LPARAM)&rcButton));

        if ( !CRect().IntersectRect (rcClip, rcButton) )
        {
            continue;
        }
        bool bOver = nHotItem == i && IS_ENABLED(tbbutton);
        bool bPressed = false;

        if ( IS_INDETERMINATE(tbbutton) )
        {
            CPenDC pen (cDC, ::GetSysColor (COLOR_3DDKSHADOW));

            cDC.MoveTo (rcButton.left, rcButton.bottom);
            cDC.LineTo (rcButton.left, rcButton.top);
            cDC.LineTo (rcButton.right-1, rcButton.top);
            cDC.LineTo (rcButton.right-1, rcButton.bottom-1);
            cDC.LineTo (rcButton.left, rcButton.bottom-1);
            bOver = true;
        }
        else if ( bOver || IS_CHECKED(tbbutton) )
        {
            bPressed = KEYDOWN(VK_LBUTTON) && rcButton.PtInRect (ptCursor);

            if ( IS_DROPDOWN(tbbutton) && bPressed )
            {
                bPressed = ptCursor.x < rcButton.right-13;

                if ( bPressed )
                {
                    rcButton.right -= 13;
                }
            }
			mybrush= bPressed||(bOver&&IS_CHECKED(tbbutton)) ? HLS_TRANSFORM (crHighLight, +50, -50) : (bOver ? HLS_TRANSFORM (crHighLight, +70, -57) : HLS_TRANSFORM (crHighLight, +80, -66));
			// PRESIONADO, sobre y ???????
			CPenDC pen (cDC, crHighLight);
            CBrushDC brush (cDC,mybrush);

            cDC.Rectangle (&rcButton);

            if ( IS_DROPDOWN(tbbutton) )
            {
                if ( bPressed )
                {
                    int nLeft = rcButton.left;

                    rcButton.left = rcButton.right-1;
                    rcButton.right += 13;

                    brush.Color (HLS_TRANSFORM (crHighLight, +70, -66));
                    cDC.Rectangle (&rcButton);

                    rcButton.left = nLeft;
                }
                else
                {
                    cDC.MoveTo (rcButton.right-14, rcButton.top);
                    cDC.LineTo (rcButton.right-14, rcButton.bottom);
                }
            }
        }
        if ( IS_SEPARATOR(tbbutton) )
        {
            CPenDC pen (cDC, HLS_TRANSFORM (colorface/*::GetSysColor (COLOR_3DFACE)*/, -15, 0));

            if ( IS_WRAP(tbbutton) )
            {
                cDC.MoveTo (rcClient.left+2, rcButton.bottom-4);
                cDC.LineTo (rcClient.right-2, rcButton.bottom-4);
            }
            else
            {
                cDC.MoveTo ((rcButton.right+rcButton.left)/2-1, rcButton.top+2);
                cDC.LineTo ((rcButton.right+rcButton.left)/2-1, rcButton.bottom-2);
            }
        }
        else if ( !IS_CONTROL(tbbutton) )
        {
            if ( IS_DROPDOWN(tbbutton) )
            {
                CPenDC pen (cDC, ( bOver && !IS_INDETERMINATE(tbbutton) ) ? RGB(0,0,0) : ::GetSysColor (IS_ENABLED(tbbutton) ? COLOR_BTNTEXT : COLOR_GRAYTEXT));

                cDC.MoveTo (rcButton.right-9, (rcButton.top+rcButton.bottom)/2-1);
                cDC.LineTo (rcButton.right-4, (rcButton.top+rcButton.bottom)/2-1);
                cDC.MoveTo (rcButton.right-8, (rcButton.top+rcButton.bottom)/2);
                cDC.LineTo (rcButton.right-5, (rcButton.top+rcButton.bottom)/2);
                cDC.SetPixel (rcButton.right-7, (rcButton.top+rcButton.bottom)/2+1, pen.Color());

                rcButton.right -= 14;
            }
            if ( tbbutton.iBitmap >= 0 )
            {
                if ( !IS_ENABLED(tbbutton) || (bOver && !bPressed) )
                {
                    HICON hIcon = ImageList_ExtractIcon (NULL, m_hImageList, tbbutton.iBitmap);
                    cDC.DrawState (CPoint (rcButton.left + ( bOver ? 4 : 3 ), rcButton.top + ( bOver ? 4 : 3 )), m_sizeImage, hIcon, DSS_MONO, CBrush (bOver ? (IS_INDETERMINATE(tbbutton) ? HLS_TRANSFORM (::GetSysColor (COLOR_3DFACE), -20, 0) : HLS_TRANSFORM (::GetSysColor (COLOR_HIGHLIGHT), +50, -66)) : HLS_TRANSFORM (::GetSysColor (COLOR_3DFACE), -27, 0)));
                    DestroyIcon (hIcon);
                }
                if ( IS_ENABLED(tbbutton) )
                {
                    ::ImageList_Draw (m_hImageList, tbbutton.iBitmap, cDC.m_hDC,
                                      rcButton.left + ( (bOver && !bPressed) ? 2 : 3 ), rcButton.top + ( (bOver && !bPressed) ? 2 : 3 ), ILD_TRANSPARENT);
                }
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void CToolBarXP::OnButtonDropDown (NMHDR* lpnmhdr, LRESULT* plResult)
{
	FUNCTION_STACK_TRACE("FFFFE0CF");
    LPNMTOOLBAR lpnmtb = (LPNMTOOLBAR)lpnmhdr;

    *plResult = TBDDRET_DEFAULT;

    if ( !m_pDockSite->SendMessage (WM_COMMAND, DROPDOWN(lpnmtb->iItem)) )
    {
        *plResult = TBDDRET_TREATPRESSED;
    }
}

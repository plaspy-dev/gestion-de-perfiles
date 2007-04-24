/****************************************************************************
 *																			*
 *								GuiToolKit  								*
 *							 (MFC extension)								*
 * Created by Francisco Campos G. www.beyondata.com fcampos@beyondata.com	*
 *--------------------------------------------------------------------------*
 *																			*
 * This program is free software;so you are free to use it any of your		*
 * applications (Freeware, Shareware, Commercial),but leave this header		*
 * intact.																	*
 *																			*
 * These files are provided "as is" without warranty of any kind.			*
 *																			*
 *			       GuiToolKit is forever FREE CODE !!!!!					*
 *																			*
 *--------------------------------------------------------------------------*
 * Created by: Francisco Campos G.											*
 * Bug Fixes and improvements : (Add your name)								*
 * -Francisco Campos														*
 *																			*
 ****************************************************************************/

#include "stdafx.h"
# include "Componentes.h"
#include "guidocktoolbarex.h"
# include "draw.h"
# include "newmenu/newmenu.h"

/////////////////////////////////////////////////////////////////////////////
// CDockButton

CDockButton::CDockButton()
{
	m_bMouseOver=FALSE;
	m_hIcon=NULL;
}

CDockButton::~CDockButton()
{
}


BEGIN_MESSAGE_MAP(CDockButton, CWnd)
	//{{AFX_MSG_MAP(CDockButton)
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_KILLFOCUS()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDockButton message handlers

BOOL CDockButton::Create(LPCTSTR lpszWindowName,DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID,HWND hWnd)
{
	// TODO: Add your specialized code here and/or call the base class
	m_Caption=lpszWindowName;
	m_nDockBarAling=nID;
	m_hWnd=hWnd;
	return CWnd::Create(NULL,lpszWindowName, dwStyle |WS_CLIPSIBLINGS|WS_CLIPCHILDREN, rect, pParentWnd, nID+(int)m_hWnd);

}

int CDockButton::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: Add your specialized creation code here

	return 0;
}

CSize GetSizeIcon(HICON hIcon)
{
	CSize m_SizeImage;
	ICONINFO bm;
	ZeroMemory(&bm, sizeof(ICONINFO));
	::GetIconInfo(hIcon,&bm);
	m_SizeImage =CSize((BYTE)(bm.xHotspot*2),(BYTE)(bm.yHotspot*2));
	::DeleteObject(bm.hbmMask);
	::DeleteObject(bm.hbmColor);
	return m_SizeImage;
}

void CDockButton::SetIconBtn(HICON hIcon)
{
	m_hIcon=hIcon;
	m_szBtn= GetSizeIcon(m_hIcon);
}

void CDockButton::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (m_bMouseOver) return;

	if (!IsTopParentActive() || !GetTopLevelParent()->IsWindowEnabled())
		return;
	CRect m_rect;
	GetClientRect(m_rect);
	if (m_rect.PtInRect(point))
	{

		CGuiControlBar* m_tb=GetBar(m_hWnd);
		if 	((m_tb->IsVisible() && m_tb->IsAutoHide()) )
		{
			
			m_tb->ScrollOff();
			GetParentFrame()->ShowControlBar(m_tb, FALSE, FALSE);
		}
		else
		{
			if 	(!m_tb->IsVisible())
			{
				CGuiControlBar* m_tb1=IsControlVisible(m_tb);
				if (m_tb1!= NULL)
				{
					if (m_tb1 != m_tb)
					{
						GetParentFrame()->ShowControlBar(m_tb1, FALSE, TRUE);
					}
				}
				SetTimer(2, AfxGetApp()->GetProfileInt("Barras\\Barras","showitdelay",500), NULL);
				m_bMouseOver=TRUE;
			}
		}
	}


	CWnd::OnMouseMove(nFlags, point);
}


CGuiControlBar* CDockButton::IsControlVisible(CGuiControlBar* m_Bar)
{
	CGuiDockToolBarEx *barra= NULL;
	CDockButton* pBtn;
	int nSize=0;

	for( int i=0; i<4; i++) {
		
		barra= (CGuiDockToolBarEx *) AfxGetMainWnd()->SendMessage(WM_JINFO, i);
		ASSERT(barra);
		nSize= barra->m_ArrayButton.GetSize();

		for( int j=0; j<nSize; j++) {

			pBtn= (CDockButton*) barra->m_ArrayButton[j];
			CGuiControlBar* pBar= GetBar(pBtn->m_hWnd);
			if(pBar == m_Bar) continue;
			if (pBar->IsWindowVisible())
				return pBar;
		}
	}
	return NULL;
}

CGuiControlBar* CDockButton::GetBar(HWND hWnd)
{
	POSITION pos = GetParentFrame()->m_listControlBars.GetHeadPosition();
	while (pos)
	{
		CControlBar* pBar =
			(CControlBar*)GetParentFrame()->m_listControlBars.GetNext(pos);
		ASSERT_VALID(pBar);
		if (pBar->m_hWnd == hWnd)
		   return (CGuiControlBar *)pBar;
	}
	return NULL;
}

void CDockButton::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	if(!dc.m_hDC)
		return;

	CBrush cb;

	COLORREF m_clrFace;
	COLORREF m_clrBorder;

	m_clrFace= m_bMouseOver ? HLS_TRANSFORM (::GetSysColor (COLOR_HIGHLIGHT), +80, -66) : GetSysColor(COLOR_3DFACE);
	m_clrBorder= m_bMouseOver ? GetSysColor(COLOR_HIGHLIGHT) : GetSysColor(COLOR_3DSHADOW);
	
	CPen cp(PS_SOLID,1,m_clrFace);
	CPen* cpold=dc.SelectObject(&cp);
	CRect m_rect;
	GetClientRect(m_rect);
	cb.CreateSolidBrush(m_clrFace);
	dc.FillRect(m_rect,&cb);
	
	CPen pen(PS_SOLID, 1, m_clrBorder);
	dc.SelectObject(&pen);
	CBrush brush(m_clrFace);
	CBrush *oldbrush= dc.SelectObject(&brush);

	CRect rect= m_rect;
	// dc.DrawIcon(CPoint(m_rect.left+2,m_rect.top+2),m_hIcon);
	// GuiDrawLayer::DrawIcon(&dc,CPoint(m_rect.left+2,m_rect.top+2),m_szBtn,m_hIcon);

	switch(m_nDockBarAling)
	{
		case AFX_IDW_DOCKBAR_TOP:
			rect.top-=5;
			dc.RoundRect(rect, CPoint(10,10));
			dc.MoveTo(m_rect.left,m_rect.top);
			dc.LineTo(m_rect.right,m_rect.top);
			if(m_hIcon != NULL)
				m_rect.left+=m_szBtn.cx+4;
			DrawTextHorz(&dc,m_rect);
			break;
		case AFX_IDW_DOCKBAR_LEFT:
			rect.left-=5;
			dc.RoundRect(rect, CPoint(10,10));

			dc.MoveTo(m_rect.left,m_rect.top);
			dc.LineTo(m_rect.left,m_rect.bottom);
			if(m_hIcon != NULL)
				m_rect.top+=m_szBtn.cy+4;
			DrawTextVert(&dc,m_rect);
			break;
		case AFX_IDW_DOCKBAR_RIGHT:
			rect.right+=5;
			dc.RoundRect(rect, CPoint(10,10));

			dc.MoveTo(m_rect.right-1,m_rect.top);
			dc.LineTo(m_rect.right-1,m_rect.bottom);
			if(m_hIcon != NULL)
				m_rect.top+=m_szBtn.cy+4;
			DrawTextVert(&dc,m_rect);
			break;
		case AFX_IDW_DOCKBAR_BOTTOM:
			rect.bottom+=5;
			dc.RoundRect(rect, CPoint(10,10));

			dc.MoveTo(m_rect.left,m_rect.bottom-1);
			dc.LineTo(m_rect.right,m_rect.bottom-1);
			if(m_hIcon != NULL)
				m_rect.left+=m_szBtn.cx+4;
			DrawTextHorz(&dc,m_rect);
			break;
	}
	dc.SelectObject(cpold);
	dc.SelectObject(oldbrush);
}

void CDockButton::OnTimer(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (!m_bMouseOver) return;
	CRect m_rc;
	CPoint pt(GetMessagePos());
	CRect m_rect;
	GetClientRect(m_rect);
	CPoint ptB=pt;
	ScreenToClient(&pt);
	CGuiControlBar* m_tb=GetBar(m_hWnd);
	m_tb->GetWindowRect(&m_rc);

	if( nIDEvent==2) {
		KillTimer(2);
		CRect myrect;
		GetClientRect(myrect);
		if (myrect.PtInRect(pt)) {

			GetParent()->GetWindowRect(m_tb->m_rectHiden);
			m_tb->ScrollOn();  //numero de ventana a activar
			SetTimer(1,AfxGetApp()->GetProfileInt("Barras\\Barras","timeout",1000),0);
			Invalidate();
		} else m_bMouseOver=false;
		return;
	}

	if (m_rc.PtInRect(ptB)) return;
	if (m_rect.PtInRect(pt)) return;
	if (m_tb->IsTracking()) return;
	if (!m_rect.PtInRect(pt))
	{
		m_bMouseOver=0;
		KillTimer(1);
		CGuiControlBar* m_tb1=IsControlVisible(m_tb);
		if (!m_tb1)
			m_tb->ScrollOff();
		Invalidate();
		//aqui se debe ocultar la ventana auto hide
	}

	CWnd::OnTimer(nIDEvent);
}

void CDockButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CWnd::OnLButtonDown(nFlags, point);
}

void CDockButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CWnd::OnLButtonUp(nFlags, point);
}

void CDockButton::OnKillFocus(CWnd* pNewWnd)
{
	CWnd::OnKillFocus(pNewWnd);
}


void CDockButton::DrawTextHorz(CDC* pDC, CRect m_rect)
{
	if(!pDC || !pDC->m_hDC)
		return;
	pDC->SetBkMode(TRANSPARENT);
	CFont* pOldFont = pDC->SelectObject(&m_fontHorzMenu);
	pDC->DrawText(m_Caption, m_rect,
		DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	pDC->SelectObject(pOldFont);
}


void CDockButton::DrawTextVert(CDC* pDC, CRect m_rect)
{
	if(!pDC || !pDC->m_hDC)
		return;

	pDC->SetTextColor(::GetSysColor(COLOR_MENUTEXT));

	int iGraphicsMode = pDC->GetGraphicsMode();
	pDC->SetBkMode(TRANSPARENT);

	// I know precise text size
	CSize m_sizeHorz;

	pDC->SetGraphicsMode(GM_ADVANCED);
	ASSERT(pDC->GetMapMode()==MM_TEXT); // esto es para revisar que este correcto para el gettextextent

	CFont* pOldFont = pDC->SelectObject(&m_fontVertMenu);

	CSize m_sizeCad=pDC->GetTextExtent(m_Caption);
	m_sizeHorz.cx = ( m_sizeCad.cx + CXTEXTMARGIN*2)+8;
	m_sizeHorz.cy = (_cyHorzFont + _cyTextMargin*2)+1;

	CRect rcString = CRect(
	CPoint(m_rect.right - _cyTextMargin, m_rect.top + CXTEXTMARGIN), m_sizeHorz);

	pDC->DrawText(m_Caption, rcString,
		DT_SINGLELINE | DT_NOCLIP | DT_NOPREFIX);// don't forget DT_NOCLIP
	pDC->SelectObject(pOldFont);
	pDC->SetGraphicsMode(iGraphicsMode);
}

/////////////////////////////////////////////////////////////////////////////
// CGuiDockToolBarEx


CGuiDockToolBarEx::CGuiDockToolBarEx()
{
	m_clrface= GetSysColor(COLOR_3DFACE);
	m_Size=20;
	n_numBars=0;
	m_ArrayButton.RemoveAll();

}

CGuiDockToolBarEx::~CGuiDockToolBarEx()
{
	m_ArrayButton.RemoveAll();
}

void CGuiDockToolBarEx::OnDestroy()
{
	// Libera la memoria
    for( int i=0; i<n_numBars; i++ )
    {
    CDockButton *pBtn = (CDockButton*)m_ArrayButton.GetAt(i);
    if( pBtn )
        delete pBtn;
    }
	CWnd::OnDestroy();

	// TODO: Add your message handler code here
}



BEGIN_MESSAGE_MAP(CGuiDockToolBarEx, CControlBar)
	//{{AFX_MSG_MAP(CGuiDockToolBarEx)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_NCPAINT()
	ON_WM_NCCALCSIZE()
	ON_WM_SYSCOLORCHANGE()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_WM_SIZE()
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CGuiDockToolBarEx message handlers

BOOL CGuiDockToolBarEx::Create(CWnd* pParentWnd,DWORD dwAling, UINT id)
{
	ASSERT ( dwAling & CBRS_ALIGN_ANY);
	SetBarStyle (dwAling);
	DWORD dwStyle = CCS_NOPARENTALIGN|CCS_NOMOVEY|CCS_NODIVIDER|CCS_NORESIZE;
	if (pParentWnd->GetStyle() & WS_THICKFRAME)
		dwStyle |= SBARS_SIZEGRIP;
	dwStyle |= WS_CHILD;
	return CControlBar::Create(0,0,dwStyle,	CRect(0,0,0,0),pParentWnd,id);
}

int CGuiDockToolBarEx::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	if (!InitGlobalFont())
	   return -1;
	return 0;
}

BOOL CGuiDockToolBarEx::OnEraseBkgnd(CDC* pDC)
{
	CBrush cbr;
	cbr.CreateSolidBrush(m_clrface);
	CRect m_rect;
	GetClientRect(m_rect);
	pDC->FillRect(m_rect,&cbr);
	return TRUE;

}

void CGuiDockToolBarEx::CalcButton()
{
	CRect rcCli;
	GetClientRect(rcCli);

	int sidepos= rcCli.top;
	int nPos=rcCli.left;

	CClientDC dc(this);
	CRect m_Arect;
	for (int iCont=0; iCont< n_numBars;iCont++)
	{
		CDockButton* ct=(CDockButton*) m_ArrayButton[iCont];
		CSize m_sChar=dc.GetTextExtent(ct->GetString());
		m_Arect.SetRectEmpty();
		CSize szBtn=ct->GetSizeIconBtn();

		if (ct->GetAling() == AFX_IDW_DOCKBAR_TOP ||
			ct->GetAling() == AFX_IDW_DOCKBAR_BOTTOM)
		{
			m_Arect.left=nPos+4;
			m_Arect.right+= nPos+m_sChar.cx+4;
			m_Arect.right+= szBtn.cy+6;
			m_Arect.top=rcCli.top;
			m_Arect.bottom=rcCli.bottom;
			nPos+=m_Arect.Width();
			nPos+=2;
		}
		else
		{
			m_Arect.top=sidepos+4;
			m_Arect.bottom+= sidepos+m_sChar.cx+4;
			m_Arect.bottom+= szBtn.cx+6;
			m_Arect.right+=rcCli.right;
			m_Arect.left+=rcCli.left;
			sidepos+=m_Arect.Height();
			sidepos+=8;
		}
		ct->SetRect(m_Arect);
		ct->MoveWindow(m_Arect,TRUE);
	}

}

void CGuiDockToolBarEx::AddToolBars(CGuiControlBar* m_ctrlBars)
{
	CString m_string;
	m_ctrlBars->GetWindowText(m_string);
	m_ArrayButton.SetAtGrow(n_numBars,
		new CDockButton());
	if (!((CDockButton *)m_ArrayButton[n_numBars])->Create(m_string,WS_VISIBLE,CRect(0,0,0,0),this,m_ctrlBars->GetDlgID(),m_ctrlBars->m_hWnd))
		return ;
	((CDockButton *)m_ArrayButton[n_numBars])->SetIconBtn(m_ctrlBars->GetIcon());
	n_numBars++;
	CalcButton();
	UpdateBars();
	Invalidate();
	UpdateWindow();
}

void CGuiDockToolBarEx::DeleteToolBars(CGuiControlBar* m_ctrlBars)
{
	int i;
	for (i=0; i < n_numBars; i++)
	{

		CGuiControlBar *m_tb=(CGuiControlBar *) ((CDockButton *)m_ArrayButton[i])->GetBar(((CDockButton *)m_ArrayButton[i])->GethWnd());
		if (m_tb == m_ctrlBars)
			break;
	}

	CDockButton *pBtn = (CDockButton*)m_ArrayButton.GetAt(i);
    if( pBtn )
        delete pBtn;
	m_ArrayButton.RemoveAt(i);
	n_numBars--;
	UpdateBars();
	CalcButton();
	Invalidate();
	UpdateWindow();
}

void CGuiDockToolBarEx::UpdateToolBars(CGuiControlBar *m_ctrlBars) {

	if(!m_pDockBar)
		return;
	ASSERT(m_pDockBar);
	DWORD stilo= GetBarStyle();
	CRect rectmain, rect;
	m_pDockBar->GetWindowRect(rectmain);
	GetWindowRect(rect);

	if( stilo&CBRS_ALIGN_RIGHT) {

		if( rect.right+m_Size< rectmain.right) {
			rect.OffsetRect(rectmain.Width(), 0);
			m_pDockBar->DockControlBar(this, &rect);
		}
	} else if( stilo&CBRS_ALIGN_LEFT) {

		if( rect.left-m_Size> rectmain.left) {

			rect.OffsetRect(-rectmain.Width(), 0);
			m_pDockBar->DockControlBar(this, &rect);
		}
	}  else if( stilo&CBRS_ALIGN_BOTTOM) {
		if( rect.bottom+m_Size< rectmain.bottom) {

			rect.OffsetRect(0, rectmain.Height());
			m_pDockBar->DockControlBar(this, &rect);
		}
	}
}

void CGuiDockToolBarEx::UpdateBars()
{
	if (m_ArrayButton.GetSize() > 0)
		ShowWindow(SW_SHOW);
	else
		ShowWindow(SW_HIDE);
}

void CGuiDockToolBarEx::OnSysColorChange()
{
	m_clrface= GetSysColor(COLOR_3DFACE);
	CControlBar::OnSysColorChange();
}

void CGuiDockToolBarEx::OnPaint()
{
	CPaintDC dc(this);
	if(!dc.m_hDC)
		return;
	CRect m_rect;
	GetClientRect(m_rect);
	CBrush cbr;
	cbr.CreateSolidBrush(m_clrface);
	dc.FillRect(m_rect,&cbr);
	CalcButton();
	UpdateBars();
}

void CGuiDockToolBarEx::OnMouseMove(UINT nFlags, CPoint point)
{
	CControlBar::OnMouseMove(nFlags, point);
}

void CGuiDockToolBarEx::OnTimer(UINT nIDEvent)
{
	CControlBar::OnTimer(nIDEvent);
}

void CGuiDockToolBarEx::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp)
{
	UpdateBars();
	CControlBar::OnNcCalcSize(bCalcValidRects, lpncsp);
}

void CGuiDockToolBarEx::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
{
}

CSize CGuiDockToolBarEx::CalcFixedLayout(BOOL bStretch, BOOL bHorz)
{
	if( bStretch) {
		if (bHorz)
			return CSize(32767,m_Size);
		else
			return CSize(m_Size,32767);
	}
	CRect rcWin;
	m_pDockBar->GetWindowRect(rcWin);
	if(!bHorz)
		return  CSize(m_Size,rcWin.Height());
	else
		return CSize(rcWin.Width(),m_Size);
}

void CGuiDockToolBarEx::OnSize(UINT nType, int cx, int cy)
{
	CControlBar::OnSize(nType, cx, cy);
}

void CGuiDockToolBarEx::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	CControlBar::OnWindowPosChanged(lpwndpos);
}

void CGuiDockToolBarEx::OnLButtonDown(UINT nFlags, CPoint point)
{
	// CControlBar::OnLButtonDown(nFlags, point);
}

void CGuiDockToolBarEx::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// CControlBar::OnLButtonDblClk(nFlags, point);
}

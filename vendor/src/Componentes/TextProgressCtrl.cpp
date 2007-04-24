// TextProgressCtrl.cpp : implementation file
//
// Written by Chris Maunder (chrismaunder@codeguru.com)
// Copyright 1998.
//
// Modified : 26/05/98 Jeremy Davis, jmd@jvf.co.uk
//				Added colour routines
//
// TextProgressCtrl is a drop-in replacement for the standard 
// CProgressCtrl that displays text in a progress control.
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed by any means PROVIDING it is not sold for
// profit without the authors written consent, and providing that this
// notice and the authors name is included. If the source code in 
// this file is used in any commercial application then an email to
// the me would be nice.
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability if it causes any damage to your
// computer, causes your pet cat to fall ill, increases baldness or
// makes you car start emitting strange noises when you start it up.
//
// Expect bugs.
// 
// Please use and enjoy. Please let me know of any bugs/mods/improvements 
// that you have found/implemented and I will fix/incorporate them into this
// file. 

#include "stdafx.h"
# include "Componentes.h"
#include "TextProgressCtrl.h"
# include "newmenu/newmenu.h"
# include "MemDC.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTextProgressCtrl

CTextProgressCtrl::CTextProgressCtrl()
{
	FUNCTION_STACK_TRACE("FFFFE0C1");

	m_fountHorz.CreateStockObject(DEFAULT_GUI_FONT);

    m_nPos			= 0;
    m_nStepSize		= 1;
    m_nMax			= 100;
    m_nMin			= 0;
    m_bShowText		= TRUE;
	m_colFore		= ::GetSysColor(COLOR_HIGHLIGHT);
	m_colBk			= ::GetSysColor(COLOR_3DFACE);
	m_colTextFore	= ::GetSysColor(COLOR_HIGHLIGHT);
	m_colTextBk		= ::GetSysColor(COLOR_3DFACE);

    m_nBarWidth = -1;
	m_dwFormato=DT_CENTER | DT_VCENTER | DT_SINGLELINE;
}

CTextProgressCtrl::~CTextProgressCtrl()
{
}

BEGIN_MESSAGE_MAP(CTextProgressCtrl, CProgressCtrl)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_SIZE()
    ON_MESSAGE(WM_SETTEXT, OnSetText)
    ON_MESSAGE(WM_GETTEXT, OnGetText)
	ON_WM_SYSCOLORCHANGE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTextProgressCtrl message handlers

void CTextProgressCtrl::OnSysColorChange() {

	FUNCTION_STACK_TRACE("FFFFE0C2");
	m_colFore		= ::GetSysColor(COLOR_HIGHLIGHT);
	m_colBk			= GetSysColor(COLOR_3DFACE);
	m_colTextFore	= ::GetSysColor(COLOR_HIGHLIGHT);
	m_colTextBk		= ::GetSysColor(COLOR_3DFACE);
	CProgressCtrl::OnSysColorChange();
}

LRESULT CTextProgressCtrl::OnSetText(UINT, LPARAM szText)
{
	FUNCTION_STACK_TRACE("FFFFE0C3");
    LRESULT result = Default();

    if ( (!szText && m_strText.GetLength()) ||
         (szText && (m_strText != szText))   )
    {
        m_strText = (LPCTSTR) szText;
        Invalidate();
    }

    return result;
}

LRESULT CTextProgressCtrl::OnGetText(UINT cchTextMax, LPARAM szText)
{
	FUNCTION_STACK_TRACE("FFFFE0C4");
    if (!_tcsncpy((LPTSTR) szText, m_strText, cchTextMax))
        return 0;
    else 
        return min(cchTextMax, (UINT) m_strText.GetLength());
}

BOOL CTextProgressCtrl::OnEraseBkgnd(CDC* /*pDC*/) 
{	
	FUNCTION_STACK_TRACE("FFFFE0C5");
 	return TRUE;
}

void CTextProgressCtrl::OnSize(UINT nType, int cx, int cy) 
{
	FUNCTION_STACK_TRACE("FFFFE0C5");
	CProgressCtrl::OnSize(nType, cx, cy);
	
    m_nBarWidth	= -1;   // Force update if SetPos called
}

void CTextProgressCtrl::OnPaint() 
{
	FUNCTION_STACK_TRACE("FFFFE0C6");
    if (m_nMin >= m_nMax) 
        return;

	CFont  *pFount;
    CRect LeftRect, RightRect, ClientRect;
    GetClientRect(ClientRect);

    double Fraction = (double)(m_nPos - m_nMin) / ((double)(m_nMax - m_nMin));

	CPaintDC PaintDC(this); // device context for painting
    CMemDC dc(&PaintDC);
	if( !dc.m_hDC) {
		Default();
		return;
	}

    LeftRect = RightRect = ClientRect;

    LeftRect.right = LeftRect.left + (int)((LeftRect.right - LeftRect.left)*Fraction);

	TRIVERTEX        vert[2];
	GRADIENT_RECT    gRect;

	vert [0] .x      = LeftRect.left;
	vert [0] .y      = LeftRect.top;
	vert [0] .Red    = 0xff00;
	vert [0] .Green  = 0xff00;
	vert [0] .Blue   = 0xff00;
	vert [0] .Alpha  = 0x0000;

	vert [1] .x      = LeftRect.right;
	vert [1] .y      = LeftRect.bottom;
	vert [1] .Red    = GetRValue(m_colFore)<<8;
	vert [1] .Green  = GetGValue(m_colFore)<<8;
	vert [1] .Blue   = GetBValue(m_colFore)<<8;
	vert [1] .Alpha  = 0x0000;

	gRect.UpperLeft  = 0;
	gRect.LowerRight = 1;

	dc.GradientFill(vert,2,&gRect,1,GRADIENT_FILL_RECT_V);

    RightRect.left = LeftRect.right;
    dc.FillSolidRect(RightRect, m_colBk);

    if (m_bShowText)
    {
        CString str;
		pFount = dc.SelectObject(&m_fountHorz);

        if (m_strText.GetLength())
            str = m_strText;
        else
            str.Format("%d%%", (int)(Fraction*100.0));

        dc.SetBkMode(TRANSPARENT);

        CRgn rgn;
        rgn.CreateRectRgn(LeftRect.left, LeftRect.top, LeftRect.right, LeftRect.bottom);
        dc.SelectClipRgn(&rgn);
        dc.SetTextColor(m_colTextBk);

		if(m_dwFormato&DT_CENTER) {

		} else {
			ClientRect.left+=2;
			ClientRect.top-=2;
		}		
        dc.DrawText(str, ClientRect, m_dwFormato);

        rgn.DeleteObject();
        rgn.CreateRectRgn(RightRect.left, RightRect.top, RightRect.right, RightRect.bottom);
        dc.SelectClipRgn(&rgn);
        dc.SetTextColor(m_colTextFore);

        dc.DrawText(str, ClientRect, m_dwFormato);
		dc.SelectObject(pFount);
    }
}

void CTextProgressCtrl::SetForeColour(COLORREF col)
{
	m_colFore = col;
}

void CTextProgressCtrl::SetBkColour(COLORREF col)
{
	m_colBk = col;
}

void CTextProgressCtrl::SetTextForeColour(COLORREF col)
{
	m_colTextFore = col;
}

void CTextProgressCtrl::SetTextBkColour(COLORREF col)
{
	m_colTextBk = col;
}

COLORREF CTextProgressCtrl::GetForeColour()
{
	return m_colFore;
}

COLORREF CTextProgressCtrl::GetBkColour()
{
	return m_colBk;
}

COLORREF CTextProgressCtrl::GetTextForeColour()
{
	return m_colTextFore;
}

COLORREF CTextProgressCtrl::GetTextBkColour()
{
	return m_colTextBk;
}
/////////////////////////////////////////////////////////////////////////////
// CTextProgressCtrl message handlers

void CTextProgressCtrl::SetShowText(BOOL bShow)
{ 
	FUNCTION_STACK_TRACE("FFFFE0C6");
    if (::IsWindow(m_hWnd) && m_bShowText != bShow)
        Invalidate();
    m_bShowText = bShow;
}


void CTextProgressCtrl::SetRange(int nLower, int nUpper)
{
	FUNCTION_STACK_TRACE("FFFFE0C7");
    m_nMax = nUpper;
    m_nMin = nLower;
}

int CTextProgressCtrl::SetPos(int nPos) 
{	
	FUNCTION_STACK_TRACE("FFFFE0C7");
    if (!::IsWindow(m_hWnd))
        return -1;

    int nOldPos = m_nPos;
    m_nPos = nPos;

    CRect rect;
    GetClientRect(rect);

    double Fraction = (double)(m_nPos - m_nMin) / ((double)(m_nMax - m_nMin));
    int nBarWidth = (int) (Fraction * rect.Width());

    if (nBarWidth != m_nBarWidth)
    {
        m_nBarWidth = nBarWidth;
        RedrawWindow();
    }

    return nOldPos;
}

int CTextProgressCtrl::StepIt() 
{	
   return SetPos(m_nPos + m_nStepSize);
}

int CTextProgressCtrl::OffsetPos(int nPos)
{
    return SetPos(m_nPos + nPos);
}

int CTextProgressCtrl::SetStep(int nStep)
{
	FUNCTION_STACK_TRACE("FFFFE0C8");
    int nOldStep = m_nStepSize;
    m_nStepSize = nStep;
    return nOldStep;
}

void CTextProgressCtrl::SetFormato(DWORD formato/*=DT_CENTER | DT_VCENTER | DT_SINGLELINE*/) {

	m_dwFormato= formato;
	Invalidate();
}

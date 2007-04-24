// StaticSeparator.cpp : implementation file
//

#include "stdafx.h"
# include "Componentes.h"
#include "separador/StaticSeparator.h"
#include "separador/VisualStylesXP.h"

/////////////////////////////////////////////////////////////////////////////
// CStaticSeparator

CStaticSeparator::CStaticSeparator()
{
}

CStaticSeparator::~CStaticSeparator()
{
}


BEGIN_MESSAGE_MAP(CStaticSeparator, CStatic)
    ON_WM_SIZE()
    ON_WM_MOVE()
    ON_WM_DESTROY()
    ON_MESSAGE(WM_SETTEXT, OnSetText)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStaticSeparator message handlers

void CStaticSeparator::PreSubclassWindow() 
{
	FUNCTION_STACK_TRACE("FFFFE09C");
	CStatic::PreSubclassWindow();

    // set bold font;
    LOGFONT logFont;
    GetFont()->GetLogFont(&logFont);
    logFont.lfWeight = FW_BOLD;
    m_font.CreateFontIndirect(&logFont);
    SetFont(&m_font);
    
    OnUpdateContents();
}

void CStaticSeparator::OnUpdateContents()
{
	FUNCTION_STACK_TRACE("FFFFE09D");
    if (m_ctrlSeparator.GetSafeHwnd())
        m_ctrlSeparator.DestroyWindow();
    
    CRect rect;
    GetClientRect(&rect);
    m_nOldWidth = rect.Width();

    CDC *pDC = GetDC();
    CFont *pFont = pDC->SelectObject(GetFont());
    CString strText;
    GetWindowText(strText);
    if (strText != "")
        strText += " ";
    CSize size = pDC->GetTextExtent(strText);
    pDC->SelectObject(pFont);
    ReleaseDC(pDC);

    rect.left += size.cx;

    if (g_xpStyle.IsAppThemed())
        rect.top += size.cy/3;

    m_ctrlSeparator.Create(WS_CHILD|WS_VISIBLE, rect, this);
}

void CStaticSeparator::OnSize( UINT nType, int cx, int cy )
{
	FUNCTION_STACK_TRACE("FFFFE09E");
    CRect rect;
    m_ctrlSeparator.GetClientRect(&rect);
    m_ctrlSeparator.SetWindowPos(NULL, 0, 0, rect.Width() + cx - m_nOldWidth, cy, SWP_NOMOVE|SWP_NOZORDER);
    m_nOldWidth = cx;
    CStatic::OnSize(nType, cx, cy);
}

void CStaticSeparator::OnMove(int x, int y)
{
	FUNCTION_STACK_TRACE("FFFFE09F");
    CStatic::OnMove(x, y);
    OnUpdateContents();
}

void CStaticSeparator::OnDestroy()
{
	FUNCTION_STACK_TRACE("FFFFE0A0");
    m_font.DeleteObject();
    m_ctrlSeparator.DestroyWindow();
    CStatic::OnDestroy();
}

LRESULT CStaticSeparator::OnSetText(WPARAM wParam, LPARAM lParam)
{
	FUNCTION_STACK_TRACE("FFFFE0A1");
    Default();
    OnUpdateContents();
    RedrawWindow();
    return TRUE;
}


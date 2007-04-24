/****************************************************************/
/*																*/
/*  InfobarCtrl.cpp												*/
/*																*/
/*  Implementation of the CInfobarCtrl class.					*/
/*	This class imitates the outlook infobar.					*/
/*																*/
/*  Programmed by Pablo van der Meer							*/
/*  Copyright Pablo Software Solutions 2002						*/
/*	http://www.pablovandermeer.nl								*/
/*																*/
/*  Last updated: 15 may 2002									*/
/*																*/
/****************************************************************/

#include "stdafx.h"
# include "Componentes.h"
# include <resource.h>
#include "InfobarCtrl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CInfobarCtrl::CInfobarCtrl()
{
	m_dwFontColor=::GetSysColor(COLOR_INFOTEXT);
	m_dwBkColor= ::GetSysColor(COLOR_INFOBK);
}

CInfobarCtrl::~CInfobarCtrl()
{
}


BEGIN_MESSAGE_MAP(CInfobarCtrl, CStatic)
	ON_WM_PAINT()
	ON_MESSAGE(WM_SETTEXT, OnSetText)
END_MESSAGE_MAP()


void CInfobarCtrl::SetBkColor(COLORREF color) {
	m_dwBkColor= color;
	Invalidate();
}

void CInfobarCtrl::SetFontColor(COLORREF color) {

	m_dwFontColor= color;
	Invalidate();
}
/********************************************************************/
/*																	*/
/* Function name : OnPaint											*/
/* Description   : This is where all the painting of our control	*/
/*				   takes place.										*/
/*																	*/
/********************************************************************/
void CInfobarCtrl::OnPaint() 
{
	FUNCTION_STACK_TRACE("FFFFE061");
	CPaintDC dc(this); // device context for painting
	CDC memDC;
	CBitmap memBitmap;
	CBitmap* pOldBitmap;

	CRect rcClient;
	GetClientRect(&rcClient);

	CString texto;
	GetWindowText(texto);

	// to avoid flicker, establish a memory dc, draw to it and then BitBlt it to the client
	memDC.CreateCompatibleDC(&dc);
	if(!memDC.m_hDC) {
		Default();
		return;
	}
	memBitmap.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());
	pOldBitmap = (CBitmap *)memDC.SelectObject(&memBitmap);

	// paint background
	memDC.FillSolidRect(rcClient, m_dwBkColor);

	// select font
	CFont* pOldFont = (CFont*)memDC.SelectObject(&m_font);
	COLORREF oldTextColor;
	int nBkMode = memDC.SetBkMode(TRANSPARENT);

	// set text color to white
	oldTextColor = memDC.SetTextColor(m_dwFontColor);

	CRect rcItem;
	GetClientRect(rcItem);

	DWORD modo= 0;
	if(GetStyle()&SS_LEFT)
		modo=DT_LEFT;
	else if(GetStyle()&SS_CENTER)
		modo=DT_CENTER;
	else if(GetStyle()&SS_RIGHT)
		modo=DT_RIGHT;

	modo|=DT_SINGLELINE | DT_VCENTER | DT_PATH_ELLIPSIS;

	// draw text
	memDC.DrawText(texto, rcItem, modo);

	CPen pen(PS_SOLID,1, ::GetSysColor(COLOR_3DDKSHADOW));
	CPen *oldpen= memDC.SelectObject(&pen);
	memDC.MoveTo(rcClient.right, rcClient.bottom-1);
	memDC.LineTo(rcClient.left, rcClient.bottom-1);

	pen.DeleteObject();
	pen.CreatePen(PS_SOLID, 1, ::GetSysColor(COLOR_BTNSHADOW));
	memDC.SelectObject(&pen);
	memDC.MoveTo(rcClient.right, rcClient.bottom-2);
	memDC.LineTo(rcClient.left, rcClient.bottom-2);

	// finally send the result to the display
	dc.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &memDC, 0, 0, SRCCOPY);

	// restore old values

	memDC.SelectObject(oldpen);
	// memDC.SelectObject(oldbrush);
	memDC.SelectObject(pOldFont);
	memDC.SetTextColor(oldTextColor);
	memDC.SetBkMode(nBkMode);
	memDC.SelectObject(pOldBitmap);
}

/********************************************************************/
/*																	*/
/* Function name : PreSubclassWindow								*/
/* Description   : Initialize control variables						*/
/*																	*/
/********************************************************************/
void CInfobarCtrl::PreSubclassWindow() 
{
	FUNCTION_STACK_TRACE("FFFFE063");
	CStatic::PreSubclassWindow();

	VERIFY(m_font.CreateStockObject( DEFAULT_GUI_FONT ));
	SetFont(&m_font);
}

void CInfobarCtrl::SetFuente(CFont &fuente) {

	m_font.DeleteObject();
	LOGFONT lg;
	fuente.GetLogFont(&lg);
	m_font.CreateFontIndirect(&lg);
	SetFont(&m_font);
}

CRect CInfobarCtrl::GetRect() {

	CRect rect;
	CDC *pDC= GetDC();
	CFont* pOldFont = (CFont*)pDC->SelectObject(&m_font);
	CString texto;
	GetWindowText(texto);
	CSize tamano= pDC->GetTextExtent(texto);

	GetParent()->GetClientRect(rect);
	rect.bottom= rect.top+tamano.cy+6;
	pDC->SelectObject(pOldFont);
	ReleaseDC(pDC);
	return rect;
}

void CInfobarCtrl::Show(bool show/*=true*/) {

	if(IsWindowVisible()==show)
		return;
	if(show) {

		CRect rect= GetRect();
		CWnd* pChild = GetParent()->GetWindow(GW_CHILD);
		CRect otro;
		while (pChild) {

			if(pChild!=this) {
				pChild->GetWindowRect(otro);
				GetParent()->ScreenToClient(&otro);
				otro.MoveToY(otro.top+rect.Height());
				pChild->MoveWindow(otro);
			}
			pChild = pChild->GetWindow(GW_HWNDNEXT);
		}
		ShowWindow(SW_NORMAL);
		MoveWindow(rect);
		GetParent()->RedrawWindow();
	} else {
		CRect rect;
		GetWindowRect(rect);

		CWnd* pChild = GetParent()->GetWindow(GW_CHILD);
		CRect otro;
		while (pChild) {

			if(pChild!=this) {
				pChild->GetWindowRect(otro);
				GetParent()->ScreenToClient(&otro);
				otro.MoveToY(otro.top-rect.Height());
				pChild->MoveWindow(otro);
			}
			pChild = pChild->GetWindow(GW_HWNDNEXT);
		}
		ShowWindow(SW_HIDE);
		GetParent()->RedrawWindow();
	}
}

LRESULT CInfobarCtrl::OnSetText(WPARAM wParam, LPARAM lParam)
{
	Default();
	RedrawWindow();
	return TRUE;
}

void CInfobarCtrl::Update() {

	if(::IsWindow(*this) && IsWindowVisible()) {
		CRect rect= GetRect();
		MoveWindow(rect, TRUE);
		Invalidate(TRUE);
	}
}

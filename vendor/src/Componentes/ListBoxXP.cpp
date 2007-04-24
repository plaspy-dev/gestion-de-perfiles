// ListBoxXP.cpp : implementation file
//

#include "stdafx.h"
#include "Componentes.h"
#include "ListBoxXP.h"


// CListBoxXP

IMPLEMENT_DYNAMIC(CListBoxXP, CListBox)

CListBoxXP::CListBoxXP()
{

}

CListBoxXP::~CListBoxXP()
{
}


BEGIN_MESSAGE_MAP(CListBoxXP, CListBox)
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()



// CListBoxXP message handlers



void CListBoxXP::PreSubclassWindow()
{
	CListBox::PreSubclassWindow();
	m_pToolTip.Create(this, false);
	m_pToolTip.SetWindowPos(&wndTopMost,0, 0,0,0,SWP_NOACTIVATE);
}



void CListBoxXP::OnMouseMove(UINT nFlags, CPoint point)
{
	CRect rectClient;
	GetClientRect(&rectClient);

	if (rectClient.PtInRect(point))
	{
		CPoint pointScreen;
		GetCursorPos(&pointScreen);
		BOOL bOutside = FALSE;
		int nItem = ItemFromPoint(point, bOutside);  // calculate listbox item number (if any)

		if (!bOutside && (nItem >= 0))
		{
			CString strText;
			GetText(nItem, strText);

			CRect rect;
			GetItemRect(nItem, &rect);
			ClientToScreen(&rect);

			CDC *pDC = GetDC();
			CFont *oldFont= pDC->SelectObject(GetFont());

			CSize size= pDC->GetTextExtent(strText);
			pDC->SelectObject(oldFont);
			ReleaseDC(pDC);

			if (size.cx > (rect.Width() - 3))
			{
				CPoint punto(rect.left, rect.top-2);
				m_pToolTip.Show(strText, &punto);
				SetTimer(1, 80, NULL);
			}
			else
			{
				m_pToolTip.Hide();
			}
		}
	}
	else
	{
		m_pToolTip.Hide();
	}
	CListBox::OnMouseMove(nFlags, point);
}

void CListBoxXP::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent==1) {
		CPoint point;
		GetCursorPos(&point);
		ScreenToClient(&point);

		CRect rectClient;
		GetClientRect(&rectClient);

		DWORD dwStyle = GetStyle();
		if ((!rectClient.PtInRect(point)) || ((dwStyle & WS_VISIBLE) == 0))
		{
			KillTimer(nIDEvent);
			m_pToolTip.Hide();
		}
	}
	CListBox::OnTimer(nIDEvent);
}

void CListBoxXP::OnDestroy()
{
	CListBox::OnDestroy();
	m_pToolTip.DestroyWindow();
}

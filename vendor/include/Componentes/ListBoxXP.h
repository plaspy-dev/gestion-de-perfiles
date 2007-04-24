#pragma once
// CListBoxXP
# include "XInfoTip.h"

class CListBoxXP : public CListBox
{
	DECLARE_DYNAMIC(CListBoxXP)
protected:
	CXInfoTip		m_pToolTip;
public:
	CListBoxXP();
	virtual ~CListBoxXP();

protected:
	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	afx_msg void OnDestroy();
};



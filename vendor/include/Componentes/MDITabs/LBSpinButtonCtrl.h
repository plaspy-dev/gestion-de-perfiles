//***************************************************************************
//
// AUTHOR:  JUHU
//
// DESCRIPTION: Alias los botones de Espacio de edicion.
//
// ORIGINAL: mailto:oleglb@mail.ru
//
#pragma once

#include "../GuiControlBar.h"
#include "../tools.h"

class  __COMPONENTES_EXPORT__ CLBSpinButtonCtrl : public CSpinButtonCtrl
{
// Construction
public:
	//***********************************************************************
	// Name:		CLBSpinButtonCtrl
	// Description:	Default constructor.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	CLBSpinButtonCtrl();
	//***********************************************************************
	// Name:		CLBSpinButtonCtrl
	// Description:	Destructor.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	virtual ~CLBSpinButtonCtrl();

// Attributes
public:

protected:
	CGuiControlBarButton m_BDerecha;
	CGuiControlBarButton m_BIzquierda;
	
	CRect m_RectD;
	CRect m_RectI;
	int m_nStateD;
	int m_nStateI;
	int m_nDisable;
	CMouseMgr	m_MouseMgr;
	HWND m_hWndBuddy;

	int  MiHitTest(CPoint point);
	
	bool m_bAutoDisable;
	bool m_bBuddyIsEdit;
	bool m_bDefaultDirection;
	bool m_bActiveSpinPressed;
	bool m_bVertical;

	CRect m_rctIsPressed;
	CRect m_rctClient;

	int m_nPrevPos;
	int m_nMinPos;
	int m_nMaxPos;

	UINT m_nSpinAlign;
	UINT m_nSpinState;
	enum m_nState {BothEnable=0,DisableRight,DisableLeft,BothDisable};
	enum m_nPosition{Outside=0,OnRightInside,OnLeftInside};

private:
	HPEN m_penLightShadow;
	HPEN m_penDarkShadow;
	HPEN m_penShadow;
	HPEN m_penLight;
	HPEN m_penButtonFace;

// Operations
public:
	//***********************************************************************
	// Name:		SetAutoDisable
	// Description:	Desahbita los botones.
	// Parameters:	bool bSetOn.
	// Return:		bool.	
	// Notes:		None.
	//***********************************************************************
	bool SetAutoDisable(bool bSetOn);

protected:
	virtual void PreSubclassWindow();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void Init();
	void CleanUpHook() const;
	void DisableRect(CDC &dc, const CRect &rectDisable) const;

	void DrawButtons(CRect *recti=NULL,CRect *rectd=NULL, CDC *pDC=NULL);
	
	afx_msg BOOL OnDeltapos(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnMouseOut(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};


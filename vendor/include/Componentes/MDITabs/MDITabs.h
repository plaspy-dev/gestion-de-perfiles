//***************************************************************************
//
// AUTHOR:  JUHU
//
// DESCRIPTION: A tabcontrol for switching between MDI-views
//
// ORIGINAL: Christian Rodemeyer,MDITabs.
//
//***************************************************************************
#pragma once

#ifndef __MDITABS_H
#define __MDITABS_H

#include "../GuiControlBar.h"
#include "LBSpinButtonCtrl.h"
# define	VENTANAS_PREFIJO		31000

/****************************************************************************\
 EMDITabStyles: Styles for customizing the appeareance of CMDITabs
\****************************************************************************/
enum EMDITabStyles
{
  MT_BOTTOM        = 0x0000, // places tabs at bottom (default)
  MT_TOP           = 0x0001, // place tabs at top
  MT_IMAGES        = 0x0002, // show images
  MT_HIDEWLT2VIEWS = 0x0004, // Hide Tabs when less than two views are open (default is one view)
  MT_TOOLTIPS      = 0x0008, // not implemented (a tooltip can appear about a tab) 
  MT_BUTTONS       = 0x0010, // not implemented (show tabs as buttons)
  MT_AUTOSIZE      = 0x0020, // not implemented (tabs are sized to fit the entire width of the control)
  MT_TASKBAR       = 0x0038  // MT_TOOLTIPS|MT_BUTTONS|MT_AUTOSIZE
};

enum EWMMTGetInfo
{
  WM_GETTABTIPTEXT = WM_APP + 0x0393, // send to associated view to get a tip text
  WM_GETTABSYSMENU = WM_APP + 0x0394  // if neccessary, the view can provide a different systemmenu by answering this message
};

/****************************************************************************\
 CMdiTabs: Deklaration
\****************************************************************************/
class  __COMPONENTES_EXPORT__ CMDITabs : public CTabCtrl
{
// Construction
public:
	//***********************************************************************
	// Name:		CMDITabs
	// Description:	Constructor.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
  CMDITabs();
  //***********************************************************************
  // Name:		CMDITabs
  // Description:	Destructor.
  // Parameters:	None.
  // Return:		None.	
  // Notes:		None.
  //***********************************************************************
  ~CMDITabs();

// Attributes
private:
	HWND       m_mdiClient;
	int        m_height;
	int        m_width;
	CImageList m_images;
	int        m_minViews; // minimum number of views 
	bool       m_bImages;
	bool       m_bTop;
	CFont		m_bFontSel;
	CFont		m_bFont;
	CGuiControlBarButton m_CloseBtn;
	int			m_stateBtn;
	CRect		m_rectButton;
	bool		m_bDrag;
	int			m_nItemDrag;
	CLBSpinButtonCtrl m_wndSpin;

// Operations
public:
	//***********************************************************************
	// Name:		Create
	// Description:	Crea un nuevo Tab.
	// Parameters:	CFrameWnd* pMainFrame, DWORD dwStyle = MT_BOTTOM|MT_IMAGES.
	// Return:		None.	
	// Notes:		MT_BOTTOM places tabs at bottom (default)
	//				MT_TOP    place tabs at top.
	//				MT_IMAGES show images
	//				MT_HIDEWLT2VIEWS Hide Tabs when less than two views are open (default is one view)
	//				MT_TOOLTIPS not implemented (a tooltip can appear about a tab) 
	//				MT_BUTTONS not implemented (show tabs as buttons)
	//				MT_AUTOSIZE not implemented (tabs are sized to fit the entire width of the control)
	//				MT_TASKBAR = MT_TOOLTIPS|MT_BUTTONS|MT_AUTOSIZE

	//***********************************************************************
	void Create(CFrameWnd* pMainFrame, DWORD dwStyle = MT_BOTTOM|MT_IMAGES);
	//***********************************************************************
	// Name:		Update
	// Description:	sync the tabctrl with all views.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		
	//***********************************************************************
	void Update();
	//***********************************************************************
	// Name:		SetMinViews
	// Description:	Cuando solo hay un tab esconder la "barra" de tabs.
	// Parameters:	int minViews.
	// Return:		None.	
	// Notes:		
	//***********************************************************************
	void SetMinViews(int minViews) {m_minViews = minViews;}
	//***********************************************************************
	// Name:		SetSelEx
	// Description:	Haga el tab selecionado al tab con id sel.
	// Parameters:	int sel.
	// Return:		None.	
	// Notes:		
	//***********************************************************************
	void	SetSelEx( int sel);
	//***********************************************************************
	// Name:		GetItemText
	// Description:	Obtenga el texto del tab seleccionatdo.
	// Parameters:	int minViews.
	// Return:		None.	
	// Notes:		
	//***********************************************************************
	CString	GetItemText(int sel);
	//***********************************************************************
	// Name:		GetItemData
	// Description:	Obtenga la informacion asociada al tab.
	// Parameters:	int index.
	// Return:		None.	
	// Notes:		
	//***********************************************************************
	DWORD GetItemData(int index);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnNcPaint();
	afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnCaptureChanged(CWnd *pWnd);

protected:
	void	DrawButton();
	virtual void  Drawtabs(CDC* dc);
	void  Drawtab(CDC* dc, int index);
	afx_msg void OnSelChange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPaint();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg LRESULT OnSizeParent(WPARAM, LPARAM lParam);

 DECLARE_MESSAGE_MAP()
};

#endif

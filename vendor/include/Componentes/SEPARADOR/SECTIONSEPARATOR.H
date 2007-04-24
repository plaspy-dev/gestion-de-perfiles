#pragma once
#include "afxwin.h"
#include "StaticSeparator.h"

#include <map>
using namespace std;
//***************************************************************************
//
// AUTHOR:  JUHU
//
// DESCRIPTION: Es un separador para agrupar componentes.
//
// ORIGINAL: 
//
//***************************************************************************
class __COMPONENTES_EXPORT__ CSectionSeparator : public CWnd
{
	// Construction
public:
	//***********************************************************************
	// Name:		CSectionSeparator
	// Description:	Constructor.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
    CSectionSeparator(void);
	//***********************************************************************
	// Name:		CSectionSeparator
	// Description:	Destructor.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
    ~CSectionSeparator(void);

// Attributes
protected:
	CString m_strLabel;
	CSeparator m_ctrlSeparator;
	bool m_bCollapsed;
	map<HWND,int> m_related;
	bool m_bChanging;
	bool m_bResizeParent;
	bool m_bAnimCollapse;
	int m_nNextSectionId;
	int m_nSectionHeight;

	CFont m_font;
	int m_nOldWidth;

	float m_fFPS;
	float m_fAnimDuration;

// Operations
public:
	//***********************************************************************
	// Name:		SetNextSectionCtrl
	// Description:	Establece el rango al ultimo control de este conjunto.
	// Parameters:	CWnd& wnd.
	// Return:		void.	
	// Notes:		None.
	//***********************************************************************
    void SetNextSectionCtrl(CWnd& wnd);
	//***********************************************************************
	// Name:		SetNextSectionId
	// Description:	Establece el rango al ultimo control de este conjunto.
	// Parameters:	int nNextId.
	// Return:		void.	
	// Notes:		None.
	//***********************************************************************
    void SetNextSectionId(int nNextId);
	//***********************************************************************
	// Name:		SetAnimOnCollapse
	// Description:	Animar al cambiar de tamaño.
	// Parameters:	int nNextId.
	// Return:		void.	
	// Notes:		None.
	//***********************************************************************
    void SetAnimOnCollapse(bool bAnim);
	//***********************************************************************
	// Name:		SetResizeParent
	// Description:	Establece el rango al ultimo control de este conjunto.
	// Parameters:	int nNextId.
	// Return:		void.	
	// Notes:		None.
	//***********************************************************************
    void SetResizeParent(bool bResize);
	//***********************************************************************
	// Name:		SetFramesPerSec
	// Description:	Establece el numero de frames por seccion.
	// Parameters:	float fFPS.
	// Return:		void.	
	// Notes:		None.
	//***********************************************************************
    void SetFramesPerSec(float fFPS);
	//***********************************************************************
	// Name:		SetAnimDuration
	// Description:	Duracion de la animacion.
	// Parameters:	float fDuration.
	// Return:		void.	
	// Notes:		None.
	//***********************************************************************
    void SetAnimDuration(float fDuration);
	//***********************************************************************
	// Name:		GetSectionHeight
	// Description:	Ancho de la seccion.
	// Parameters:	float fDuration.
	// Return:		void.	
	// Notes:		None.
	//***********************************************************************
	int  GetSectionHeight();
	
    void Collapse();
    void Uncollapse();
    bool IsCollapsed();
	void OnUpdateContents();

	afx_msg void OnPaint();
	afx_msg LRESULT OnPrint(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetText(WPARAM wParam, LPARAM lParam);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSize( UINT nType, int cx, int cy );
	afx_msg void OnMove(int x, int y);
	afx_msg void OnDestroy();

protected:
    virtual void UpdateSectionHeight();
    virtual void GetSectionTops(int& nCurrTop, int& nNextTop);
    virtual void MoveControls(map<HWND,CRect>& actions, float fDuration);
    virtual void PumpMessages();
    virtual void Print(CDC* pDC, DWORD dwFlags);
    virtual void PreSubclassWindow();

DECLARE_MESSAGE_MAP()
};

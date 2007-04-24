//***************************************************************************
//
// AUTHOR:  JUHU
//
// DESCRIPTION: Un edit con estilo XP.
//
// ORIGINAL: 
//
//***************************************************************************
#pragma once

#include "Tools.h"
# include "XInfoTip.h"
# include "TextProgressCtrl.h"
# include <LAppc/LAppc.h>
// CEditXP

class __COMPONENTES_EXPORT__ CEditXP : public CEdit
{
protected:
	DECLARE_DYNAMIC(CEditXP)
	void		SetMensajeEx(bool Limpiar=true);
	HICON		m_hIcon;
	bool		m_bDerecha;
	void		UpdateIcon();
// Construction
public:
	//***********************************************************************
	// Name:		CEditXP
	// Description:	Default constructor.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	CEditXP();
	//***********************************************************************
	// Name:		CEditXP
	// Description:	Destructor.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	virtual ~CEditXP();

// Attributes
public:
	bool		m_bSalto;
	bool		m_bSaltoEx;
	void		DrawBordes();
	void		DrawBordes(bool draw);
	// funciones del progress control
	void SetRange(int minimo, int maximo);
	void SetPos(int pos);
	void SetStep(int step);
	int Step();
	int	GetPos() const;
	int GetMaximo() const;
	CTextProgressCtrl *GetProgressCtrl();
	void HideProgress();

protected:
	// nuevo con la barra de progreso
	CTextProgressCtrl	m_wndTextProgress;
	CXWinVersion		m_WVersion;
	int		m_nMax;
	int		m_nCurrent;
	int		m_nStep;

	CString		m_szMensaje;
	CXInfoTip	m_Tip;
	COLORREF	m_crOver;
	COLORREF	m_crFocus;
	COLORREF	m_crDefault;
	CMouseMgr	m_MouseMgr;
	int			m_hAlto;
	int			m_hlFont;
	bool		m_bDrawBordes;
	int			m_nAncho;
	virtual void PreSubclassWindow();

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnSetFont(WPARAM wParam, LPARAM lParam); // Maps to WM_SETFONT

// Operaciones
public:
	void ValidateWidth();
	void InitAutoWidth(bool bAuto=true);
	//***********************************************************************
	// Name:		InitMultiline
	// Description:	Permite al edit tener varias lineas.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	void InitMultiline();
	//***********************************************************************
	// Name:		ValidarTamano
	// Description:	Para que el edit tenga el tamaño necesario para el texto que contiene.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	void ValidarTamano();
	//***********************************************************************
	// Name:		SetToolTip
	// Description:	Contenido de la ayuda dinamica.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		El mensajito que sale al poner el mouse encima.
	//***********************************************************************
	void SetToolTip(CString texto, CString titulo="");
	//***********************************************************************
	// Name:		GetWindowText
	// Description:	Escribe el contenido del edit.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		El mensajito que sale al poner el mouse encima.
	//***********************************************************************
	void GetWindowText(CString &rString);
	//***********************************************************************
	// Name:		SetToolTip
	// Description:	Obtiene el contenido del edit.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		El mensajito que sale al poner el mouse encima.
	//***********************************************************************
	void SetWindowText(LPCTSTR lpszString);


	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnMouseOut();
	afx_msg void OnSysColorChange();
	afx_msg LRESULT OnPaste(WPARAM Wparam, LPARAM LParam);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnDestroy();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnWindowPosChanging(WINDOWPOS*);

public:
	// Text
	__declspec(property(get=GetText,put=SetText))		CString	Text;
	void	SetText(const CString &texto);
	CString	GetText();

	// Mensaje
	__declspec(property(get=GetMensaje,put=SetMensaje))		CString	Mensaje;
	void	SetMensaje(const CString &mensaje);
	CString  GetMensaje();

	// texto de la barra
	__declspec(property(get=GetProgressText,put=SetProgressText))		CString	ProgressText;
	void	SetProgressText(const CString &mensaje);
	CString  GetProgressText();
	
	void SetIcon(HICON hIcon=NULL,bool derecha=true);
	void SetIcon(UINT nIcon=0,bool derecha=true);

	DECLARE_MESSAGE_MAP()
};



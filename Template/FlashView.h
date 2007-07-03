#pragma once
#include "afxwin.h"

# include <Componentes/Componentes.h>

// CFlashView form view

class CFlashView : public CFormView
{
	DECLARE_DYNCREATE(CFlashView)
protected:
	CFlashView();           // protected constructor used by dynamic creation
	virtual ~CFlashView();

public:
	enum { IDD = IDD_FLASHVIEW };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
public:
	CButton m_BMostrar;
protected:
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
public:
	afx_msg void OnBnClickedCnomostrar();
public:
	CShockwaveflash1 m_SFlash;
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	afx_msg void OnDestroy();
};



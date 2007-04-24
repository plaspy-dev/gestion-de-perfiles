#pragma once
#include "afxwin.h"
#include "afxcmn.h"
# include "resource.h"

// CPaisesView form view

class CPaisesView : public CFormView
{
	DECLARE_DYNCREATE(CPaisesView)
	CConector		*m_pConector;
protected:
	CPaisesView();           // protected constructor used by dynamic creation
	virtual ~CPaisesView();

public:
	enum { IDD = IDD_PAISESVIEW };
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
	CEditXP m_EPais;
	afx_msg void OnBnClickedBaceptar();
	CButtonXP m_BAceptar;
	CMyListCtrl m_LCPaises;
	LRESULT OnMySQLMessage(WPARAM wParam, LPARAM lParam);
	CButtonXP m_BCancelar;
	afx_msg void OnBnClickedBcancelar();
};



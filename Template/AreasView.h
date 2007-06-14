#pragma once
#include "afxwin.h"
#include "afxcmn.h"



// CAreasView form view

class CAreasView : public CFormView
{
	DECLARE_DYNCREATE(CAreasView)
	CConector	*m_pConector;
	DWORD		m_dwId;
	enum {
		query_areas=1,
		insert_area,
	};
protected:
	CAreasView();           // protected constructor used by dynamic creation
	virtual ~CAreasView();

public:
	enum { IDD = IDD_AREASVIEW };
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
	CEditXP m_EArea;
public:
	CButtonXP m_BAgregar;
public:
	CMyListCtrl m_LCAreas;
public:
	afx_msg void OnEnChangeEarea();
public:
	afx_msg void OnBnClickedBagregar();
public:
	afx_msg void OnNMDblclkLareas(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnNMRclickLareas(NMHDR *pNMHDR, LRESULT *pResult);
	void OnEditar();
	void OnEliminar();
	LRESULT OnMySQLMessage(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	CButtonXP m_BCancelar;
	CStatic	  m_SArea;
public:
	afx_msg void OnBnClickedBcancelar();
};



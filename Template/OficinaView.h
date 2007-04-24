#pragma once
#include "afxwin.h"



// COficinaView form view

class COficinaView : public CFormView
{
	DECLARE_DYNCREATE(COficinaView)

protected:
	COficinaView();           // protected constructor used by dynamic creation
	virtual ~COficinaView();
	CConector *m_pConector;
	CString		m_szOldPKZ;
	LRESULT OnMySQLMessage(WPARAM wParam, LPARAM lParam);

	enum {
		query_pkz=1,
	};
public:
	enum { IDD = IDD_OFICINAVIEW };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	virtual void OnInitialUpdate();
	afx_msg void OnEnKillfocusEpkz1();
	afx_msg void OnBnClickedBok();
	afx_msg void OnBnClickedBcancel();
	CEditXP m_EPKZ;
	CEditXP m_EDireccion;
	CEditXP m_ECPostal;
	CEditXP m_EAAereo;
	CEditXP m_ETelefono;
	CEditXP m_EFax;
	CEditXP m_EEmail;
	CEditXP m_EWebSite;
	CButtonXP m_BCancelar;
	void Clean();
	afx_msg void OnDestroy();
public:
	CEditXP m_ENombre;
	CEditXP m_EPais;
	CEditXP m_EDepartamento;
	CEditXP m_ECiudad;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	LRESULT OnInsertPKZ(WPARAM wParam, LPARAM lParam);
};



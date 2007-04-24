#pragma once
#include "afxwin.h"
# include <resource.h>

// CDlgAvanzado dialog

class CDlgAvanzado : public CDialog
{
	DECLARE_DYNAMIC(CDlgAvanzado)

public:
	CDlgAvanzado(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgAvanzado();

// Dialog Data
	enum { IDD = IDD_DAVANZADO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_EBaseDeDatos;
public:
	CEdit m_EPuerto;
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnEnChangeEbasededatos();
public:
	afx_msg void OnEnChangeEpuerto();
public:
	virtual BOOL OnInitDialog();
	CString m_szPuerto;
	CString m_szDB;
public:
	CButton m_BOK;
public:
	CStatic m_SNombreData;
};

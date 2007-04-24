#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CDlgProgreso dialog

class CDlgProgreso : public CDialog
{
	DECLARE_DYNAMIC(CDlgProgreso)
	int		m_nMaximo;
public:
	CDlgProgreso(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgProgreso();

// Dialog Data
	enum { IDD = IDD_DPROGRESO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedCancel();
public:
	virtual BOOL	OnInitDialog();
	CStringArray	*m_pMensajes;
	CStringArray	*m_pQuerys;
	CString			m_szTitulo;
public:
	CStatic m_SProgreso;
	CConector *m_pConector;
public:
	CStatic m_SMensaje;
public:
	CTextProgressCtrl m_PCProgreso;
	LRESULT OnMySQLMensaje(WPARAM wParam, LPARAM lParam);
};

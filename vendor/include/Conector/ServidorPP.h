#pragma once
#include "afxwin.h"
#include "DlgAvanzado.h"
// CServidorPP dialog

class CServidorPP : public CPropertyPage
{
	DECLARE_DYNAMIC(CServidorPP)

public:
	CServidorPP();
	virtual ~CServidorPP();

// Dialog Data
	enum { IDD = IDD_PPSERVIDOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_EServidor;
public:
	CEdit m_EUsuario;
public:
	CEdit m_EContrasena;
public:
	afx_msg void OnEnChangeEservidor();
public:
	afx_msg void OnBnClickedBavanzado();
public:
	afx_msg void OnEnChangeEusuario();
public:
	virtual BOOL OnInitDialog();
public:
	virtual BOOL OnSetActive();
	CDlgAvanzado m_pAvanzado;
public:
	virtual LRESULT OnWizardNext();
};

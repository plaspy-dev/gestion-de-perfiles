#pragma once
#include "afxwin.h"
# include <resource.h>
// CUsuarioPP dialog

class CUsuarioPP : public CPropertyPage
{
	DECLARE_DYNAMIC(CUsuarioPP)

public:
	CUsuarioPP();
	virtual ~CUsuarioPP();

// Dialog Data
	enum { IDD = IDD_PPUSUARIO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CButton m_CConfigurar;
public:
	CEdit m_EUsuario;
public:
	CEdit m_EContrasena;
public:
	CEdit m_EConfirmar;
public:
	afx_msg void OnBnClickedCconfigurar();
public:
	afx_msg void OnEnChangeEusuario();
public:
	afx_msg void OnEnChangeEcontrasena();
public:
	afx_msg void OnEnChangeEconfirmar();
public:
	virtual BOOL OnInitDialog();
public:
	virtual BOOL OnSetActive();
public:
	afx_msg void OnStnClickedSmensaje();
public:
	CStatic m_SMensaje;
public:
	virtual LRESULT OnWizardNext();
public:
	CStatic m_SAyuda;
public:
	CStatic m_SMensajeCheck;
};

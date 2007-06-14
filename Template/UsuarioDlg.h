#pragma once
#include "afxwin.h"


// CUsuarioDlg dialog

class CUsuarioDlg : public CDialog
{
	DECLARE_DYNAMIC(CUsuarioDlg)

public:
	CUsuarioDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CUsuarioDlg();

// Dialog Data
	enum { IDD = IDD_USUARIODLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CEditXP m_EContrasena;
public:
	CButtonXP m_BAceptar;
public:
	CButtonXP m_BCancelar;
public:
	afx_msg void OnBnClickedOk();
};

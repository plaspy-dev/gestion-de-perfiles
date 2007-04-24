#pragma once
# include "resource.h"
#include "afxwin.h"

// CDlgPasswd dialog

class CDlgPasswd : public CDialog
{
	DECLARE_DYNAMIC(CDlgPasswd)

public:
	CDlgPasswd(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgPasswd();

// Dialog Data
	enum { IDD = IDD_DIALOGCONTRASENA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CEditXP m_EContrasena;
	CEditXP m_EConfirma;
	CButtonXP m_BOK;
	CButtonXP m_BCancela;
	CString m_szContrasena;
	afx_msg void OnEnChangeEditcontrasena();
	virtual BOOL OnInitDialog();
};

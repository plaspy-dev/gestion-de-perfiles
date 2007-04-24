#pragma once
#include "afxwin.h"
# include "resource.h"

// CServidorSoloPP dialog

class CServidorSoloPP : public CPropertyPage
{
	DECLARE_DYNAMIC(CServidorSoloPP)

public:
	CServidorSoloPP();
	virtual ~CServidorSoloPP();

// Dialog Data
	enum { IDD = IDD_SERVIDORSOLOPP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CEditXP m_EServidor;
public:
	afx_msg void OnEnChangeEservidor();
public:
	virtual BOOL OnInitDialog();
public:
	virtual BOOL OnSetActive();
public:
	virtual LRESULT OnWizardNext();
};

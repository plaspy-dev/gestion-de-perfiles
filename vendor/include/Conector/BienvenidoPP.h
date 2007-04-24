#pragma once
#include "afxwin.h"
# include <resource.h>

// CBienvenidoPP dialog

class CBienvenidoPP : public CPropertyPage
{
	DECLARE_DYNAMIC(CBienvenidoPP)

public:
	CBienvenidoPP();
	virtual ~CBienvenidoPP();

// Dialog Data
	enum { IDD = IDD_PBIENVENIDO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnSetActive();
public:
	virtual BOOL OnInitDialog();
public:
	CStatic m_SBienvenido;
public:
	int m_nSeleccion;
public:
	virtual LRESULT OnWizardNext();
};

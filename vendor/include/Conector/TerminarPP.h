#pragma once
#include "afxwin.h"
# include <resource.h>
// CTerminarPP dialog

class CTerminarPP : public CPropertyPage
{
	DECLARE_DYNAMIC(CTerminarPP)
protected:
	bool		m_bSilent;
public:
	CTerminarPP();
	virtual ~CTerminarPP();

// Dialog Data
	enum { IDD = IDD_PPTERMINAR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CButton m_CConectar;
public:
	virtual BOOL OnInitDialog();
public:
	virtual BOOL OnSetActive();
public:
	virtual BOOL OnWizardFinish();
public:
	CButton m_CConectando;
public:
	CButton m_CConfigurando;
public:
	CButton m_CUsuario;
public:
	CButton m_CGuardar;
public:
	CStatic m_SAcciones;
public:
	CStatic m_SMensaje;
	LRESULT OnMySQLMessage(WPARAM wParam, LPARAM lParam);
public:
	CStatic m_SFinalizar;
};

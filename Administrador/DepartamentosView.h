#pragma once
#include "afxwin.h"
#include "afxcmn.h"
# include "resource.h"


// CDepartamentosView form view

class CDepartamentosView : public CFormView
{
	CConector		*m_pConector;
	LRESULT OnMySQLMessage(WPARAM wParam, LPARAM lParam);
	DECLARE_DYNCREATE(CDepartamentosView)

	enum {
		query_paises=1,
		query_departamentos,
		insert_departamento
	};

protected:
	CDepartamentosView();           // protected constructor used by dynamic creation
	virtual ~CDepartamentosView();

public:
	enum { IDD = IDD_DEPARTAMENTOSVIEW };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnBnClickedBaceptar();
	afx_msg void OnBnClickedBcancelar();
	CEditXP m_ENombre;
	CComboBoxXP m_CBPais;
	CButtonXP m_BAceptar;
	CButtonXP m_BCancelar;
	CMyListCtrl m_LCDepartamentos;
};



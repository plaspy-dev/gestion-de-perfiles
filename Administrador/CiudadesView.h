#pragma once

# include "resource.h"
#include "afxwin.h"
#include "afxcmn.h"

// CCiudadesView form view

class CCiudadesView : public CFormView
{
	DECLARE_DYNCREATE(CCiudadesView)
	CConector		*m_pConector;
	LRESULT OnMySQLMessage(WPARAM wParam, LPARAM lParam);

	enum {
		query_paises=1,
		query_departamentos,
		query_ciudades,
		insert_ciudad
	};

protected:
	CCiudadesView();           // protected constructor used by dynamic creation
	virtual ~CCiudadesView();

public:
	enum { IDD = IDD_CIUDADESVIEW };
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
	CComboBoxXP m_CBDepartamento;
	CMyListCtrl m_LCCiudades;
	CButtonXP m_BAceptar;
	CButtonXP m_BCancelar;
	afx_msg void OnCbnSelchangeCpais();
};



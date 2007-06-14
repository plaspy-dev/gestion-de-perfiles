#pragma once
#include "afxwin.h"



// CResidenciaView form view

class CResidenciaView : public CFormView
{
	DECLARE_DYNCREATE(CResidenciaView)

protected:
	CResidenciaView();           // protected constructor used by dynamic creation
	virtual ~CResidenciaView();
	CConector *m_pConector;
	CString		m_szOldPKZ;
	CString		m_szCurrentPKZ;
	LRESULT OnMySQLMessage(WPARAM wParam, LPARAM lParam);

	enum {
		query_pkz=1,
	};

public:
	enum { IDD = IDD_RESIDENCIAVIEW };
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
	afx_msg void OnEnKillfocusEpkz1();
	afx_msg void OnBnClickedBok();
	afx_msg void OnBnClickedBcancel();
	CEditXP m_EPKZ;
	CEditXP m_EDireccion;
	CEditXP m_ECPostal;
	CEditXP m_EAAereo;
	CEditXP m_ETelefono;
	CEditXP m_EFax;
	CEditXP m_EEmail;
	CEditXP m_EWebSite;
	CButtonXP m_BCancelar;
	void Clean();
	afx_msg void OnDestroy();
public:
	CEditXP m_EPais;
	CEditXP m_EDepartamento;
	CEditXP m_ECiudad;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	LRESULT OnInsertPKZ(WPARAM wParam, LPARAM lParam);
public:
	CButtonXP m_BAceptar;
public:
	CEditXP m_EEMail2;
public:
	CEditXP m_EMovil;
	CInfobarCtrl m_SFallecido;
	void OnSize(UINT nType, int cx, int cy);
	LRESULT OnShowFallecido(WPARAM wParam, LPARAM lParam);
	CUsuario *m_pUsuario;
	LRESULT OnQueryUsuario(WPARAM wParam, LPARAM lParam);
};



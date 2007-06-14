#pragma once
#include "afxwin.h"
#include "afxdtctl.h"



// CSeguimientoView form view

class CSeguimientoView : public CFormView
{
	DECLARE_DYNCREATE(CSeguimientoView)
	CConector *m_pConector;
	CString		m_szOldPKZ;
	CString		m_szCurrentPKZ;
	enum {
		query_pkz=1,
	};

protected:
	CSeguimientoView();           // protected constructor used by dynamic creation
	virtual ~CSeguimientoView();

public:
	enum { IDD = IDD_SEGUIMIENTOVIEW };
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

	CEditXP m_EPKZ;
	CDateTimeCtrl m_DTPrimero;
	CDateTimeCtrl m_DTSegundo;
	CDateTimeCtrl m_DTTercero;
	CDateTimeCtrl m_DTEncuesta;
	CEditXP m_EPrimero;
	CEditXP m_ESegundo;
	CEditXP m_ETercero;
	CEditXP m_ERespondio;
	CButtonXP m_BCancelar;
public:
	afx_msg void OnEnKillfocusEpkz();
public:
	afx_msg void OnBnClickedBcancelar();
	LRESULT OnMySQLMessage(WPARAM wParam, LPARAM lParam);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
public:
	afx_msg void OnDestroy();
	LRESULT OnInsertPKZ(WPARAM wParam, LPARAM lParam);
public:
	CButtonXP m_BAceptar;
public:
	afx_msg void OnBnClickedBok();
	void Clean();
	CInfobarCtrl m_SFallecido;
	void OnSize(UINT nType, int cx, int cy);
	LRESULT OnShowFallecido(WPARAM wParam, LPARAM lParam);
	CUsuario *m_pUsuario;
	LRESULT OnQueryUsuario(WPARAM wParam, LPARAM lParam);
};



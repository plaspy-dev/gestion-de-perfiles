#pragma once
#include "afxwin.h"
#include "afxdtctl.h"



// CGestionView form view

class CGestionView : public CFormView
{
	DECLARE_DYNCREATE(CGestionView)

	CConector *m_pConector;
	CString		m_szOldPKZ;
	CString		m_szCurrentPKZ;
	enum {
		query_pkz=1,
	};
protected:
	CGestionView();           // protected constructor used by dynamic creation
	virtual ~CGestionView();

public:
	enum { IDD = IDD_GESTIONVIEW };
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
	CEditXP m_EPKZ;
public:
	CButton m_BActualizado;
public:
	CButton m_BNoValidos;
public:
	CEditXP m_EActualizado;
public:
	CEditXP m_ENotas;
public:
	CButtonXP m_BCancelar;
public:
	CButtonXP m_BAceptar;
public:
	afx_msg void OnEnKillfocusEpkz();
public:
	afx_msg void OnBnClickedBcancelar();
public:
	afx_msg void OnBnClickedBok();
public:
	virtual void OnInitialUpdate();
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void Clean();
	LRESULT OnMySQLMessage(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnDestroy();
	LRESULT OnInsertPKZ(WPARAM wParam, LPARAM lParam);
	CInfobarCtrl m_SFallecido;
	void OnSize(UINT nType, int cx, int cy);
	LRESULT OnShowFallecido(WPARAM wParam, LPARAM lParam);
	CUsuario *m_pUsuario;
	LRESULT OnQueryUsuario(WPARAM wParam, LPARAM lParam);
	static void Actualizar(CConector *conector, const CString pkz, bool actualizado=true);
	static CString GetActualizacionI(const CString pkz, bool actualizado=true,const CString &actualizadopor="");
	static CString GetActualizacionU(const CString pkz, bool actualizado=true,const CString &actualizadopor="");
public:
	CStatic m_SActualizacion;
};



#pragma once
#include "afxwin.h"
#include "afxdtctl.h"



// CEstudiosView form view

class CEstudiosView : public CFormView
{
	DECLARE_DYNCREATE(CEstudiosView)

	CConector *m_pConector;
	CString		m_szOldPKZ;
	CString		m_szCurrentPKZ1;
	CString		m_szCurrentPKZ2;
	CString		m_szCurrentPKZ3;
	enum {
		query_estudios=1,
		query_alemania,
		query_fuera,
		query_areas
	};
protected:
	CEstudiosView();           // protected constructor used by dynamic creation
	virtual ~CEstudiosView();

public:
	enum { IDD = IDD_ESTUDIOSVIEW };
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

	//estudios
	CEditXP m_EPKZ;
	CEditXP m_ECarrera;
	CEditXP m_EUniversidad;
	CDateTimeCtrl m_DTComienzo1;
	CDateTimeCtrl m_DTFin1;
	CDateTimeCtrl m_EComienzo2;
	CDateTimeCtrl m_DTFin2;
	CEditXP m_ERevista;

	// estudios en alemania.
	CEditXP m_EAPregrado;
	CEditXP m_EAMaestria;
	CEditXP m_EADoctorado;
	CEditXP m_EAOtros;
	CEditXP m_EAPUniversidad;
	CEditXP m_EAMUniversidad;
	CEditXP m_EADUniversidad;
	CEditXP m_EAOUniversidad;
	// estudios fuera de alemania
	CEditXP m_EFPregrado;
	CEditXP m_EFMaestria;
	CEditXP m_EFDoctorado;
	CEditXP m_EFOtros;
	CEditXP m_EFPUniversidad;
	CEditXP m_EFMUniversidad;
	CEditXP m_EFDUniversidad;
	CEditXP m_EFOUniversidad;
	CEditXP m_EFPPais;
	CEditXP m_EFMPais;
	CEditXP m_EFDPais;
	CEditXP m_EFOPais;

	// boton de cancelar
	CButtonXP m_BCancelar;
public:
	afx_msg void OnEnKillfocusEpkz();
public:
	afx_msg void OnBnClickedButton1();
	LRESULT OnMySQLMessage(WPARAM wParam, LPARAM lParam);
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
public:
	afx_msg void OnDestroy();
	LRESULT OnInsertPKZ(WPARAM wParam, LPARAM lParam);
public:
	CButtonXP m_BAceptar;
public:
	afx_msg void OnBnClickedBok();
	void Clean();
public:
	CButton m_BBecario;
	CInfobarCtrl m_SFallecido;
	void OnSize(UINT nType, int cx, int cy);
	LRESULT OnShowFallecido(WPARAM wParam, LPARAM lParam);
	CUsuario *m_pUsuario;
	LRESULT OnQueryUsuario(WPARAM wParam, LPARAM lParam);
public:
	CEditXP m_ETipo1;
public:
	CEditXP m_ETipo2;
public:
	CEditXP m_EBecarioOtra;
public:
	CEditXP m_EAPostGrado;
public:
	CEditXP m_EAPostUniversidad;
public:
	CEditXP m_EOPostgrado;
public:
	CEditXP m_EOPostUniversidad;
public:
	CEditXP m_EOPostPais;
public:
	CComboBoxXP m_CBArea;
};

